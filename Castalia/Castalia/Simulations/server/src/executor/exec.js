import fs from 'fs'
import cmd from 'node-cmd'
import Promise from 'bluebird'

const getAsync = Promise.promisify(cmd.get, {multiArgs: true, context: cmd});

// let file = fs.createWriteStream('../Simulations/customSimulation/haha');
// file.write("haha\n");
// file.write("hihi\n");
let count = 0;
let getNextFileName = () => {
  count++;
  return 'config' + count + '.ini';
};
export default function exec({
                               nodes, width, height,
                               source, destination
                             }, callback) {
  cmd.get(`rm -rf Castalia-Trace.txt`);
  let fileName = getNextFileName();

  let nextId = 0;
  let o2n = {}, n2o = {};

  getAsync(`
    cp template.ini ${fileName}
  `).then(() => {
    let writer = fs.createWriteStream(fileName, {'flags': 'a'});
    writer.write(`SN.numNodes = ${nodes.length}\n`);
    writer.write(`SN.field_x = ${width}\n`);
    writer.write(`SN.field_y = ${height}\n`);

    for (let node of nodes) {
      const {id, x, y} = node;
      o2n[id] = nextId;
      n2o[nextId] = id;

      writer.write(`SN.node[${nextId}].xCoor = ${x}\n`);
      writer.write(`SN.node[${nextId}].yCoor = ${y}\n`);

      nextId++;
    }

    writer.write(`SN.node[${o2n[source]}].Application.isSource = true\n`);
    writer.write(`SN.node[${o2n[source]}].Application.sink = "${o2n[destination]}"\n`);

    writer.end();

    return getAsync(`
      Castalia -i ${fileName} -c General
    `)
  }).then(() => {
    cmd.get(`mv ${fileName} archive/`);

    let lines = fs.readFileSync('Castalia-Trace.txt').toString().split('\n');
    let path = [source];

    for (let line of lines) {
      let regex = /RECEIVED (\d+)/gi;
      let match = regex.exec(line);

      if (match) {
        let nextNode = parseInt(match[1]);
        path.push(n2o[nextNode]);
      }
    }
    callback(null, path);
  }).catch(err => {
    callback(err);
  });
}

// #SN.node[0].Application.packet_rate = 0.2
// #SN.node[0].Application.maxData = 100
// #SN.node[0].Application.startupDelay = 10
// #SN.node[0].Application.nextRecipient = "2"




