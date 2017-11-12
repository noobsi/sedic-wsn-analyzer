import fs from 'fs'
import cmd from 'node-cmd'
import Promise from 'bluebird'
import session from '../session'

const getAsync = Promise.promisify(cmd.get, {multiArgs: true, context: cmd});

const randomSessionId = (length) => {
  const possible = "ABCDEFGHIJ" +
    "KLMNOPQRSTUVWXYZabcdef" +
    "ghijklmnopqrstuvwxyz0123456789";

  let text = "";
  for (let i = 0; i < length; i++)
    text += possible.charAt(Math.floor(Math.random() * possible.length));

  return text;
};

let queue = [];

export function register(config, callback) {
  let sessionId = randomSessionId(5);
  queue.push({sessionId, config});
  session.markStatus(sessionId, "enqueued", "Simulation is registered, waiting to be executed");
  callback(null, {
    status: "OK",
    sessionId: sessionId,
    error: null,
  });

  if (queue.length === 1) {
    const {config, sessionId} = queue.shift();
    exec({config, sessionId});
  }
}




function exec({config, sessionId}) {
  const {
    network: {
      fieldWidth, fieldHeight,
      nodes, nodeConfig: {routingAlgorithm}
    },
    simulation: {
      traffic, timeLimit
    }
  } = config;
  cmd.get(`rm -rf Castalia-Trace.txt`);

  let fileName = `${sessionId}.ini`;
  const requestFileName = `logs/${sessionId}_request.txt`;
  fs.writeFile(requestFileName, JSON.stringify(config), () => {});



  let nextId = 0;
  let o2n = {}, n2o = {};

  getAsync(`
    cp template.ini ${fileName}
  `).then(() => {
    let writer = fs.createWriteStream(fileName, {'flags': 'a'});


    writer.write(`SN.numNodes = ${nodes.length}\n`);
    writer.write(`SN.field_x = ${fieldWidth}\n`);
    writer.write(`SN.field_y = ${fieldHeight}\n`);

    for (let node of nodes) {
      const {id, x, y} = node;
      // o2n[id] = nextId;
      // n2o[nextId] = id;
      o2n[id] = id;
      n2o[id] = id;

      writer.write(`SN.node[${nextId}].xCoor = ${x}\n`);
      writer.write(`SN.node[${nextId}].yCoor = ${y}\n`);

      nextId++;
    }

    for (let pair of traffic) {
      const {source, destination} = pair;
      writer.write(`SN.node[${o2n[source]}].Application.isSource = true\n`);
      writer.write(`SN.node[${o2n[source]}].Application.sink = "${o2n[destination]}"\n`);
    }

    writer.end();
    session.markStatus(sessionId, "running", "Simulation is running...");


    return getAsync(`
      Castalia -i ${fileName} -c General
    `)
  }).then(() => {
    cmd.get(`mv ${fileName} archive/`);

    const eventTraceFileName = `logs/${sessionId}_eventTrace.txt`;
    const energyTraceFileName = `logs/${sessionId}_energyTrace.txt`;
    const nodeTraceFileName = `logs/${sessionId}_nodeTrace.txt`;
    let eventWriter = fs.createWriteStream(eventTraceFileName);
    let energyWriter = fs.createWriteStream(energyTraceFileName);
    let nodeWriter = fs.createWriteStream(nodeTraceFileName);

    let lines = fs.readFileSync('Castalia-Trace.txt').toString().split('\n');
    for (let line of lines) {
      if (/WSN_EVENT/gi.test(line)) {
        const time = getTime(line);
        const type = getType(line);

        if (type === 'ENERGY') {
          let regex = /id:(\d+) energy:([\d\.]+)/gi;
          let match = regex.exec(line);
          if (match) {
            let id = parseInt(match[1]);
            let energy = parseFloat(match[2]);
            let event = {"e": "E", "id": id, "time": time, "energy": energy};
            energyWriter.write(`${JSON.stringify(event)}\n`);
          }
        } else if (type === 'FINAL') {
          let regex = /id:(\d+) x:([\d\.]+) y:([\d\.]+)/gi;
          let match = regex.exec(line);
          if (match) {
            let id = parseInt(match[1]);
            let x = parseFloat(match[2]);
            let y = parseFloat(match[3]);
            let node = {"id": id, "x": x, "y": y, "d_time": -1} ;
            nodeWriter.write(`${JSON.stringify(node)}\n`);
          }
        } else if (type === 'SEND' || type === 'FORWARD' || type === 'RECEIVE') {
          console.log(line);
          let regex = /packetId:(\d+) source:(\d+) destination:(\d+) current:(\d+)/;
          let match = regex.exec(line);
          if (match) {
            let packetId = parseInt(match[1]);
            let source = parseInt(match[2]);
            let destination = parseInt(match[3]);
            let current = parseInt(match[4]);
            let event = {"e": type[0], "p_id": packetId, "p_type": "normal",
              "source": source, "dest": destination, "cur": current, "layer": "network", "time": time};
            eventWriter.write(`${JSON.stringify(event)}\n`);
          }
        } else if (type === 'DEBUG') {

        }
      }
    }

    energyWriter.end();
    eventWriter.end();
    nodeWriter.end();
    session.markStatus(sessionId, "completed", "Simulation completed");
    if (queue.length >= 1)  {
      const {config, sessionId} = queue.shift();
      exec({config, sessionId});
    }
  }).catch(err => {
    session.markStatus(sessionId, "error", err.toString());
    if (queue.length >= 1)  {
      const {config, sessionId} = queue.shift();
      exec({config, sessionId});
    }
  });
}

const getType = (line) => {
  let regex = /WSN_EVENT (\S+)/gi;
  let match = regex.exec(line);

  if (match) {
    return match[1];
  }
  return null;
};

const getTime = (line) => {
  let regex = /^(\d+\.\d+)/gi;
  let match = regex.exec(line);

  if (match) {
    return parseFloat(match[1]);
  }

  return 0;
};



