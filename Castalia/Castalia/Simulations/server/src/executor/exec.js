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
      nodes, nodeConfig: {routingAlgorithm, macInterface}
    },
    simulation: {
      traffic = [], timeLimit
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

    if (routingAlgorithm) {
      if (routingAlgorithm.toLowerCase() === 'greedy') {
        writer.write(`SN.node[*].Communication.RoutingProtocolName = "GreedyRouting"\n`)
      } else if (routingAlgorithm.toLowerCase() === 'gpsr') {
        writer.write(`SN.node[*].Communication.RoutingProtocolName = "GpsrRouting"\n`)
      } else if (routingAlgorithm.toLowerCase() === 'rollingball') {
        writer.write(`SN.node[*].Communication.RoutingProtocolName = "RollingBallRouting"\n`)
      } else if (routingAlgorithm.toLowerCase() === 'stable') {
        writer.write(`SN.node[*].Communication.RoutingProtocolName = "StableRouting"\n`)
      } else {
        writer.write(`SN.node[*].Communication.RoutingProtocolName = "GpsrRouting"\n`)
      }
    } else {
      writer.write(`SN.node[*].Communication.RoutingProtocolName = "GpsrRouting"\n`)
    }


    if (macInterface) {
      if (['TMAC', 'TunableMAC', 'Basic802154', 'BypassMAC', 'BaselineBANMac'].find((name) => name === macInterface)) {
        if (macInterface === 'TunableMAC') {
          writer.write(`SN.node[*].Communication.MACProtocolName = "TunableMAC"\n`);
          writer.write(`SN.node[*].Communication.MAC.dutyCycle = 1.0\n`);
          writer.write(`SN.node[*].Communication.MAC.randomTxOffset = 0\n`);
          writer.write(`SN.node[*].Communication.MAC.backoffType = 2\n`);
        } else {
          writer.write(`SN.node[*].Communication.MACProtocolName = "${macInterface}"\n`);
        }
      }
    } else {
      writer.write(`SN.node[*].Communication.MACProtocolName = "TunableMAC"\n`);
      writer.write(`SN.node[*].Communication.MAC.dutyCycle = 1.0\n`);
      writer.write(`SN.node[*].Communication.MAC.randomTxOffset = 0\n`);
      writer.write(`SN.node[*].Communication.MAC.backoffType = 2\n`);
    }

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
    const drawFileName = `logs/${sessionId}_draw.txt`;
    const logFileName = `logs/${sessionId}_log.txt`;
    let eventWriter = fs.createWriteStream(eventTraceFileName);
    let energyWriter = fs.createWriteStream(energyTraceFileName);
    let nodeWriter = fs.createWriteStream(nodeTraceFileName);
    let drawWriter = fs.createWriteStream(drawFileName);
    let logWriter = fs.createWriteStream(logFileName);

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
        } else if (type === 'SEND' || type === 'FORWARD' || type === 'RECEIVE' || type === 'DROP') {
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
        } else if (type === 'DRAW') {
          // console.log(line);
          let lineRegex = /LINE ([-\d\.]+) ([-\d\.]+) ([-\d\.]+) ([-\d\.]+) (\S+)/;
          let lineMatch = lineRegex.exec(line);
          if (lineMatch) {
            let x1 = parseFloat(lineMatch[1]);
            let y1 = parseFloat(lineMatch[2]);
            let x2 = parseFloat(lineMatch[3]);
            let y2 = parseFloat(lineMatch[4]);
            let color = lineMatch[5];
            drawWriter.write(`${JSON.stringify({type: 'line', x1, y1, x2, y2, color})}\n`)
          }


          let circleRegex = /CIRCLE ([\d\.]+) ([\d\.]+) ([\d\.]+) (\S+)/;
          let circleMatch = circleRegex.exec(line);
          if (circleMatch) {
            let centerX = parseFloat(circleMatch[1]);
            let centerY = parseFloat(circleMatch[2]);
            let radius = parseFloat(circleMatch[3]);
            let color = circleMatch[4];
            drawWriter.write(`${JSON.stringify({type: 'circle', centerX, centerY, radius, color})}\n`)
          }


          let pointRegex = /POINT ([\d\.]+) ([\d\.]+) (\S+)/;
          let pointMatch = pointRegex.exec(line);
          if (pointMatch) {
            let x = parseFloat(pointMatch[1]);
            let y = parseFloat(pointMatch[2]);
            let color = pointMatch[3];
            drawWriter.write(`${JSON.stringify({type: 'point', x, y, color})}\n`)
          }
        }
      } else if (/WSN_LOG/gi.test(line)){
        let regex = /WSN_LOG (.+)/;
        let match = regex.exec(line);
        if (match) {
          logWriter.write(`${match[1]}\n`);
        }
      }
    }

    energyWriter.end();
    eventWriter.end();
    nodeWriter.end();
    drawWriter.end();
    logWriter.end();
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



