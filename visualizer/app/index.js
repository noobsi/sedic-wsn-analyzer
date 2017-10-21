// Prevent space scroll
window.addEventListener('keydown', function (e) {
  if (e.keyCode === 32) {
    e.preventDefault();
  }
});

let sigmaInstance;
let showEdges = true;
let activeState;

$('#generate-btn').click(function () {
  if (sigmaInstance) {
    sigmaInstance.kill();
  }

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
  let nextId = 0;
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
          id: nextId,
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

  while (nextId < V) {
    const x = Math.floor(Math.random() * width);
    const y = Math.floor(Math.random() * height);
    node2coord[nextId] = {x, y};
    nodes.push({
      id: nextId,
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
  for (let i = 0; i < V; i++) {
    for (let j = i + 1; j < V; j++) {
      const p1 = node2coord[i];
      const p2 = node2coord[j];
      if (!p1 || !p2) {
        console.log(i, j);
      }
      if ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) <= range * range) {
        edges.push({
          id: 'e' + nextId,
          source: i,
          target: j,
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
      outerBorderSize: 1, // stroke size of active nodes
      labelThreshold: 4,
      nodeActiveBorderSize: 0,
      nodeActiveOuterBorderSize: 0,
      labelAlignment: 'top',
      defaultNodeBorderColor: '#fff',
      defaultNodeOuterBorderColor: 'rgb(236, 81, 72)', // stroke color of active nodes
      nodeHaloColor: 'rgba(236, 81, 72, 0.2)',
      nodeHaloSize: 5,
    },

    renderer: {
      container: document.getElementById('graph-container'),
      type: 'canvas'
    },
  });
  sigmaInstance.width = width;
  sigmaInstance.height = height;
  activeState = sigma.plugins.activeState(sigmaInstance);

  let keyboard = sigma.plugins.keyboard(sigmaInstance, sigmaInstance.renderers[0]);
  let select = sigma.plugins.select(sigmaInstance, activeState);
  let lasso = new sigma.plugins.lasso(sigmaInstance, sigmaInstance.renderers[0], {
    'strokeStyle': 'rgb(236, 81, 72)',
    'lineWidth': 2,
    'fillWhileDrawing': true,
    'fillStyle': 'rgba(236, 81, 72, 0.2)',
    'cursor': 'crosshair'
  });


  select.bindKeyboard(keyboard);
  select.bindLasso(lasso);


  lasso.bind('selectedNodes', (e) => {
    setTimeout(function () {
      lasso.deactivate();
      sigmaInstance.refresh({skipIdexation: true});
    }, 0);
  });

  sigmaInstance.renderers[0].bind('render', (e) => {
    sigmaInstance.renderers[0].halo({
      nodes: activeState.nodes()
    });
  });


  //"spacebar" + "s" keys pressed binding for the lasso tool
  keyboard.bind('32+83 17+83', () => {
    if (lasso.isActive) {
      lasso.deactivate();
    } else {
      lasso.activate();
    }
  });

  keyboard.bind('27', () => {
    if (lasso.isActive) {
      lasso.deactivate();
    }
  });
  // "spacebar + d" delete active nodes
  keyboard.bind('32+68 17+68', () => {
    activeState.nodes().forEach(node => {
      sigmaInstance.graph.dropNode(node.id);
    });
    sigmaInstance.refresh();
  });


});

$('#show-edges-option').change(function () {
  showEdges = !showEdges;
  if (sigmaInstance) {
    sigmaInstance.settings('drawEdges', showEdges);
    sigmaInstance.refresh();
  }

  $('#show-edges-option').val(showEdges);
});

$('#submit-btn').click(() => {

  let edges = [...sigmaInstance.graph.edges()];
  for (let edge of edges) {
    let id = edge.id;
    if (id.substring(0, 2) === 'ex') {
      sigmaInstance.graph.dropEdge(id);
    }
  }
  const source = parseInt($('#source-input').val());
  const destination = parseInt($('#destination-input').val());

  if (!source || !destination || !sigmaInstance) {
    return;
  }

  // console.log(sigmaInstance.graph.nodes());
  let nodes = sigmaInstance.graph.nodes().map(node => ({
    id: node.id,
    x: node.x,
    y: node.y
  }));
  let width = sigmaInstance.width;
  let height = sigmaInstance.height;

  $.ajax({
    url: 'http://localhost:8080/exec',
    type: 'POST',
    contentType: 'application/json',
    data: JSON.stringify({
      nodes, width, height, source, destination
    }),
    success({path}) {
      for (let i = 1; i < path.length; i++) {
        let from = path[i - 1];
        let to = path[i];
        sigmaInstance.graph.addEdge({
          id: 'ex' + from + ':' + to,
          source: from,
          target: to,
          size: 10,
          color: '#cc4049'
        })
      }

      sigmaInstance.refresh();
    }
  })
});


