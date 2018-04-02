import Network from "./Network";

require('svg.draggable.js');
require('svg.panzoom.js');
import SVG from 'svg.js';
import Mousetrap from 'mousetrap';
import inPolygon from 'point-in-polygon';
import convexHull from 'convex-hull';
import FileSaver from 'file-saver';
import SVGtoPNG from 'save-svg-as-png';
import distanceToLineSegment from 'distance-to-line-segment'

const generateNodes = ({width, height, GRID_HEIGHT, GRID_WIDTH, V}) => {
  let nodes = [];
  let nextId = 0;
  for (let i = 0; i < GRID_HEIGHT; i++) {
    for (let j = 0; j < GRID_WIDTH; j++) {
      const numNodes = Math.floor(V / GRID_HEIGHT / GRID_WIDTH);
      const hfrom = (height / GRID_HEIGHT) * i;
      const wfrom = (width / GRID_WIDTH) * j;
      for (let k = 0; k < numNodes; k++) {
        const x = Math.random() * (width / GRID_WIDTH) + wfrom;
        const y = Math.random() * (height / GRID_HEIGHT) + hfrom;
        nodes.push({x, y, id: nextId});
        nextId++;
      }
    }
  }
  while (nextId < V) {
    const x = Math.random() * width;
    const y = Math.random() * height;
    nodes.push({x, y, id: nextId});
    nextId++;
  }
  return nodes;
};


$('#generate-btn').click(function () {
  const height = parseInt($('#height-input').val());
  const width = parseInt($('#width-input').val());
  const range = parseInt($('#range-input').val());
  const GRID_HEIGHT = parseInt($('#grid-height-input').val());
  const GRID_WIDTH = parseInt($('#grid-width-input').val());
  const V = parseInt($('#v-input').val());
  let nodes = generateNodes({width, height, GRID_WIDTH, GRID_HEIGHT, V});
  init({nodes, width, height, range});
});


let file = null;
$('#file-input').change(function () {
  if (this.files.length) {
    file = this.files[0];
    let reader = new FileReader();
    if (file) {

      reader.onload = function (event) {
        let {nodes, width, height, range} = JSON.parse(event.target.result);
        init({nodes, width, height, range})
      };

      reader.readAsText(file);
    }
  }
});

$('#generate-file-btn').click(function () {
  $('#file-input').trigger("click");
});


let network;

function init({nodes, width, height, range}) {
  if (network) {
    network.remove();
    $('#save-btn').off('click');
    $('#export-btn').off('click');
    $('#submit-btn').off('click');
    $('#reset-btn').off('click');
  }

  network = new Network({container: 'graph-container', nodes, width, height, range});
  let displayResult = (data) => {
    let shapes = data.split('\n').filter(_ => _ !== '').map(_ => JSON.parse(_));
    shapes.forEach(shape => {
      if (shape.type === 'circle') {
        let {centerX, centerY, radius, color} = shape;
        network.drawCircle({centerX, centerY, radius, style: {color, width: 0.5}})
      } else if (shape.type === 'point') {
        let {x, y, color} = shape;
        network.drawPoint({x, y, color});
      } else if (shape.type === 'line') {
        let {x1, y1, x2, y2, color} = shape;
        network.drawLine({x1, y1, x2, y2, style: {color, width: '0.5'}})
      }
    });
  };


  $('#submit-btn').click(() => {
    let data = {
      network: {
        fieldWidth: width,
        fieldHeight: height,
        nodes: network.nodes.map(({x, y}) => ({x, y})),
        nodeConfig: {
          routingAlgorithm: 'stable'
        }
      },
      simulation: {
        traffic: [],
        timeLimit: 1000,
      }
    };
    $.ajax({
      url: 'http://localhost:8080/exec',
      type: 'POST',
      contentType: 'application/json',
      data: JSON.stringify(data),
      success({sessionId}) {
        console.log(sessionId);
        const interval = setInterval(() => {
          $.ajax({
            url: `http://localhost:8080/status/${sessionId}`,
            success({status}) {
              if (status === 'completed') {
                clearInterval(interval);
                $.ajax({
                  url: `http://localhost:8080/result/draw/${sessionId}`,
                  success: displayResult,
                });
                $.ajax({
                  url: `http://localhost:8080/result/log/${sessionId}`,
                  success: (data) => console.log(data)
                });
              }
            }
          })
        }, 400);
      }
    });
  });

  $('#save-btn').click(() => {
    let blob = new Blob([JSON.stringify({width, height, range, nodes: network.nodes.map(({x, y, id}) => ({x, y, id}))})]);
    FileSaver.saveAs(blob, "network.json");
  });

  $('#reset-btn').click(() => {
    init({nodes: network.nodes, width, height, range})
  })

}

