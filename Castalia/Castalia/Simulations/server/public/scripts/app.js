// sigmaInstance = new sigma({
//   graph: g,
//   renderer: {
//     container: 'graph-container',
//     type: 'canvas'
//   },
//   settings: {
//     maxNodeSize: 2,
//     borderSize: 2,
//     outerBorderSize: 3, // stroke size of active nodes
//     defaultNodeBorderColor: '#fff',
//     defaultNodeOuterBorderColor: 'rgb(236, 81, 72)' // stroke color of active nodes
//   }
// });

let sigmaInstance;
let showEdges = true;

$('#generate-btn').click(function() {
  if (sigmaInstance) sigmaInstance.kill();

  showEdges = true;
  $('#show-edges-option').val(showEdges);

  const height = parseInt($('#height-input').val());
  const width = parseInt($('#width-input').val());
  const range = parseInt($('#range-input').val());
  const GRID_HEIGHT = parseInt($('#grid-height-input').val());
  const GRID_WIDTH = parseInt($('#grid-width-input').val());
  const V = parseInt($('#v-input').val());

  if (!height || !width || !range || !V || !GRID_WIDTH || !GRID_HEIGHT) {
    alert("Invalid input");
    return;
  }


  const nodes = [];
  const edges = [];
  const node2coord = {};

  // grid 16 x 16
  let nextId = 1;
  for (let i = 0; i < GRID_HEIGHT; i++) {
    for (let j = 0; j < GRID_WIDTH; j++) {
      const numNodes = Math.floor(V / GRID_HEIGHT / GRID_WIDTH);
      const hfrom = (height / GRID_HEIGHT) * i;
      const wfrom = (width / GRID_WIDTH) * j;


      for (let k = 0; k < numNodes; k++) {
        const x = Math.floor(Math.random() * (width / GRID_WIDTH) + wfrom);
        const y = Math.floor(Math.random() * (height / GRID_HEIGHT) + hfrom);
        node2coord[nextId] = {x, y};
        nodes.push({
          id: 'n' + nextId,
          label: 'Node ' + nextId,
          x: x,
          y: y,
          size: 1,
          color: '#000'
        });
        nextId++;
      }
    }
  }

  while (nextId <= V) {
    const x = Math.floor(Math.random() * width);
    const y = Math.floor(Math.random() * height);
    node2coord[nextId] = {x, y};
    nodes.push({
      id: 'n' + nextId,
      label: 'Node ' + nextId,
      x: x,
      y: y,
      size: 1,
      color: '#000'
    });
    nextId++;
  }

  // edges
  nextId = 1;
  for (let i = 1; i <= V; i++) {
    for (let j = i + 1; j <= V; j++) {
      const p1 = node2coord[i];
      const p2 = node2coord[j];
      if (!p1 || !p2) {
        console.log(i, j);
      }
      if ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) <= range * range) {
        edges.push({
          id: 'e' + nextId,
          source: 'n' + i,
          target: 'n' + j,
          size: 1,
          color: '#ccc'
        });
        nextId++;
      }
    }
  }

  sigmaInstance = new sigma({
    graph: {
      nodes, edges
    },
    settings: {
      maxNodeSize: 3,
      borderSize: 2,
      outerBorderSize: 3, // stroke size of active nodes
      defaultNodeBorderColor: '#fff',
      defaultNodeOuterBorderColor: 'rgb(236, 81, 72)' // stroke color of active nodes
    },
    container: 'graph-container'
  });

});

$('#show-edges-option').change(function(){
  console.log(showEdges);
  showEdges = !showEdges;
  if (sigmaInstance) {
    sigmaInstance.settings('drawEdges', showEdges);
    sigmaInstance.refresh();
  }

  $('#show-edges-option').val(showEdges);
});
