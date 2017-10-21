'use strict';

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.default = exec;

var _fs = require('fs');

var _fs2 = _interopRequireDefault(_fs);

var _nodeCmd = require('node-cmd');

var _nodeCmd2 = _interopRequireDefault(_nodeCmd);

var _bluebird = require('bluebird');

var _bluebird2 = _interopRequireDefault(_bluebird);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

const getAsync = _bluebird2.default.promisify(_nodeCmd2.default.get, { multiArgs: true, context: _nodeCmd2.default });

// let file = fs.createWriteStream('../Simulations/customSimulation/haha');
// file.write("haha\n");
// file.write("hihi\n");
let count = 0;
let getNextFileName = () => {
  count++;
  return 'config' + count + '.ini';
};
function exec({
  nodes, width, height,
  source, destination
}, callback) {
  _nodeCmd2.default.get(`rm -rf Castalia-Trace.txt`);
  let fileName = getNextFileName();

  let nextId = 0;
  let o2n = {},
      n2o = {};

  getAsync(`
    cp template.ini ${fileName}
  `).then(() => {
    let writer = _fs2.default.createWriteStream(fileName, { 'flags': 'a' });
    writer.write(`SN.numNodes = ${nodes.length}\n`);
    writer.write(`SN.field_x = ${width}\n`);
    writer.write(`SN.field_y = ${height}\n`);

    for (let node of nodes) {
      const { id, x, y } = node;
      o2n[id] = nextId;
      n2o[nextId] = id;

      writer.write(`SN.node[${nextId}].xCoor = ${x}\n`);
      writer.write(`SN.node[${nextId}].yCoor = ${y}\n`);

      nextId++;
    }
    console.log(o2n, "old to new");
    console.log(n2o, "new to old");

    writer.write(`SN.node[${o2n[source]}].Application.isSource = true\n`);
    writer.write(`SN.node[${o2n[source]}].Application.sink = "${o2n[destination]}"\n`);

    writer.end();

    return getAsync(`
      Castalia -i ${fileName} -c General
    `);
  }).then(() => {
    _nodeCmd2.default.get(`mv ${fileName} archive/`);

    let lines = _fs2.default.readFileSync('Castalia-Trace.txt').toString().split('\n');
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
//# sourceMappingURL=exec.js.map