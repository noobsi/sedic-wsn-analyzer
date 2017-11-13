// UI setting
require('utils/uiSetting');

import env from './env'
import {generateSigmaNetwork, sigmaNetwork, normalEdgesFromNodes} from './network'
import {getRandomColor} from "./utils/index";


let sigmaInstance;
$('#generate-btn').click(function () {
  if (sigmaInstance) {
    sigmaInstance.kill();
  }

  const height = parseInt($('#height-input').val());
  const width = parseInt($('#width-input').val());
  const range = parseInt($('#range-input').val());
  const GRID_HEIGHT = parseInt($('#grid-height-input').val());
  const GRID_WIDTH = parseInt($('#grid-width-input').val());
  const V = parseInt($('#v-input').val());

  sigmaInstance = generateSigmaNetwork({
    height,
    width,
    range,
    GRID_WIDTH,
    GRID_HEIGHT,
    container: document.getElementById('graph-container'),
    V,
    onDeleteNodes: onDeleteNodes
  });
});

const onDeleteNodes = () => {
  let nodes = [];
  let nextId = 0;
  for (let node of sigmaInstance.graph.nodes()) {
    let newNode = Object.assign({}, node);
    let {x, y} = node;
    newNode.id = nextId;
    newNode.label = `Node ${nextId} (${x.toFixed(2)}, ${sigmaInstance.height - y.toFixed(2)})`;
    nodes.push(newNode);
    nextId++;
  }

  sigmaInstance.graph.clear();
  let edges = normalEdgesFromNodes(nodes, sigmaInstance.range);
  sigmaInstance.graph.read({nodes, edges});
  sigmaInstance.refresh();

};

$('#planar-option').change(function() {
  if (!sigmaInstance) return;
  sigmaInstance.switchPlanarMode();
});

$('#show-edges-option').change(function () {
  if (!sigmaInstance) return;

  sigmaInstance.switchEdgesMode();
  $('#show-edges-option').val(sigmaInstance.showEdges);
});

let file = null;
$('#file-input').change(function(){
  if (this.files.length) {
    file = this.files[0];
  }
});

$('#generate-file-btn').click(function(){
  let reader = new FileReader();
  if (file) {

    reader.onload = function(event) {
      let {nodes} = JSON.parse(event.target.result);
      nodes = nodes.map(node => {
        return {
          x: node.x,
          y: node.y,
          id: node.id,
          label: `Node ${node.id} (${node.x.toFixed(2)}, ${1000 - node.y.toFixed(2)})`,
        }
      });
      if (sigmaInstance) sigmaInstance.kill();
      sigmaInstance = sigmaNetwork({
        nodes,
        edges: normalEdgesFromNodes(nodes, 40),
        range: 40,
        width: 1000,
        container: document.getElementById('graph-container'),
        height: 1000,
        onDeleteNodes
      });
    };

    reader.readAsText(file);
  }
});


$('#submit-btn').click(() => {

  const traffic = $('#traffic-input').val();
  if (!traffic || !sigmaInstance) {
    return;
  }
  let edges = [...sigmaInstance.graph.edges()];
  for (let edge of edges) {
    let id = edge.id;
    if (id.substring(0, 2) === 'ex') {
      sigmaInstance.graph.dropEdge(id);
    }
  }

  const routingAlgorithm = $('#ra-input').val();
  const pairs = traffic.split('\n').map(str => {
    const regex = /(\d+)->(\d+)/;
    const match = regex.exec(str);
    if (match)
      return {
        source: parseInt(match[1]),
        destination: parseInt(match[2]),
        numPacket: 1,
        rate: 1000,
      };
    else {
      return null;
    }
  }).filter(_ => _ !== null);

  sigmaInstance.pickedNodes = pairs.reduce((acc, {source, destination}) =>
    [...acc,
      sigmaInstance.graph.nodes(source),
      sigmaInstance.graph.nodes(destination),
    ], []);

  let width = sigmaInstance.width;
  let height = sigmaInstance.height;

  let nodes = sigmaInstance.graph.nodes().map(node => ({
    id: node.id,
    x: node.x,
    y: height - node.y
  }));

  $.ajax({
    url: env.ENDPOINT + '/exec',
    type: 'POST',
    contentType: 'application/json',
    data: JSON.stringify({
      network: {
        fieldWidth: width,
        fieldHeight: height,
        nodes,
        nodeConfig: {routingAlgorithm}
      },
      simulation: {
        traffic: pairs,
        timeLimit: 1000,
      }
    }),
    success({sessionId}) {
      showResult(sessionId);
    }
  })
});

const showResult = (sessionId) => {
  const interval = setInterval(() => {
    $.ajax({
      url: env.ENDPOINT + `/status/${sessionId}`,
      success({status}) {
        if (status === 'completed') {
          clearInterval(interval);
          $.ajax({
            url: env.ENDPOINT + `/result/event/${sessionId}`,
            success(data) {

              if (!sigmaInstance) return;
              const events = data.split('\n').filter(_ => _ !== "")
                .map(_ => JSON.parse(_));
              console.log(events);

              const latest = {};
              const color = {};
              const marked = {};
              for (let event of events) {
                const type = event['e'];
                const packetId = event['p_id'];
                const current = event['cur'];

                if (type === 'S') {
                  latest[packetId] = current;
                  color[packetId] = getRandomColor();
                } else {
                  if (!marked[[packetId, latest[packetId], current]]) {
                    sigmaInstance.graph.addEdge({
                      id: `ex:${packetId}:${latest[packetId]}:${current}`,
                      source: latest[packetId],
                      target: current,
                      size: 20,
                      color: color[packetId],
                    });
                    marked[[packetId, latest[packetId], current]] = true;
                  }

                  latest[packetId] = current;
                }
              }
              sigmaInstance.refresh();
            }
          })
        }
      }
    })
  }, 400);

};

