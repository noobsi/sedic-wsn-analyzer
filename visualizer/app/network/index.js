export function generateSigmaNetwork({
                                       container, height,
                                       width, range,
                                       GRID_HEIGHT,
                                       GRID_WIDTH, V, onDeleteNodes
                                     }) {

  console.log(range);
  if (!height || !width || !range || !V || !GRID_WIDTH || !GRID_HEIGHT) {
    alert("Invalid input");
    return;
  }


  const nodes = [];
  const edges = [];
  const node2coord = {};

  let nextId = 0;
  for (let i = 0; i < GRID_HEIGHT; i++) {
    for (let j = 0; j < GRID_WIDTH; j++) {
      const numNodes = Math.floor(V / GRID_HEIGHT / GRID_WIDTH);
      const hfrom = (height / GRID_HEIGHT) * i;
      const wfrom = (width / GRID_WIDTH) * j;


      for (let k = 0; k < numNodes; k++) {
        const x = Math.random() * (width / GRID_WIDTH) + wfrom;
        const y = Math.random() * (height / GRID_HEIGHT) + hfrom;
        node2coord[nextId] = {x, y};
        nodes.push({
          id: nextId,
          label: `Node ${nextId} (${x.toFixed(2)}, ${(height - y).toFixed(2)})`,
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
    const x = Math.random() * width;
    const y = Math.random() * height;
    node2coord[nextId] = {x, y};
    nodes.push({
      id: nextId,
      label: `Node ${nextId} (${x.toFixed(2)}, ${(height - y).toFixed(2)})`,
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
      if ((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) <= range * range) {
        edges.push({
          id: 'e' + nextId,
          source: i,
          target: j,
          color: '#ccc'
        });
        nextId++;
      }
    }
  }

  return sigmaNetwork({nodes, edges, container, width, height, onDeleteNodes, range});
}


export function normalEdgesFromNodes(nodes, range) {
  let edges = [];
  let nextId = 1;
  for (let i = 0; i < nodes.length - 1; i++) {
    for (let j = i + 1; j < nodes.length; j++) {
      let node1 = nodes[i];
      let node2 = nodes[j];
      if (distance(node1, node2) <= range) {
        edges.push({
          id: 'e' + nextId,
          source: node1.id,
          target: node2.id,
          color: '#ccc'
        });
        nextId++;
      }
    }
  }
  return edges;
}

function distance(node1, node2) {
  return Math.sqrt((node1.x - node2.x) * (node1.x - node2.x) + (node1.y - node2.y) * (node1.y - node2.y));
}

export function RNGEdgesFromNodes(nodes, range) {
  let edges = [];
  let adj = {};
  let id2n = {};
  for (let node of nodes) {
    id2n[node.id] = node;
    adj[node.id] = [];
  }
  for (let i = 0; i < nodes.length - 1; i++) {
    for (let j = i + 1; j < nodes.length; j++) {
      let node1 = nodes[i];
      let node2 = nodes[j];

      if (distance(node1, node2) <= range) {
        adj[node1.id].push(node2.id);
        adj[node2.id].push(node1.id);
      }
    }
  }

  let marked = {};
  let nextId = 0;
  for (let node of nodes) {
    for (let u of adj[node.id]) {
      let ok = true;
      for (let v of adj[node.id]){
        if (u === v) continue;
        let unode = id2n[u];
        let vnode = id2n[v];
        if (distance(node, unode) > Math.max(distance(node, vnode), distance(vnode, unode))) {
          ok = false;
        }
      }
      if (ok) {
        let i = Math.min(u, node.id);
        let j = Math.max(u, node.id);
        if (!marked[[i, j]]) {
          marked[[i, j]] = true;
          edges.push({
            id: 'e' + nextId++,
            source: i,
            target: j,
            color: '#ccc'
          })
        }
      }
    }
  }

  return edges;
}

export function sigmaNetwork({nodes, edges, range, container, width, height, onDeleteNodes}) {
  let sigmaInstance = new sigma({
    graph: {
      nodes, edges
    },
    settings: {
      borderSize: 2,
      outerBorderSize: 1, // stroke size of active nodes
      minNodeSize: 2,
      maxNodeSize: 4,
      labelThreshold: 6,
      nodeActiveBorderSize: 0,
      nodeActiveOuterBorderSize: 0,
      labelAlignment: 'top',
      defaultNodeBorderColor: '#fff',
      defaultNodeOuterBorderColor: 'rgb(236, 81, 72)', // stroke color of active nodes
      nodeHaloColor: 'rgba(236, 81, 72, 0.2)',
      nodeHaloSize: 5,
    },

    renderer: {
      container: container,
      type: 'canvas'
    },
  });


  sigmaInstance.width = width;
  sigmaInstance.height = height;
  sigmaInstance.range = range;
  sigmaInstance.showEdges = true;
  sigmaInstance.planar = false;
  sigmaInstance.pickedNodes = [];

  $('#show-edges-option').val(sigmaInstance.showEdges);

  let activeState = sigma.plugins.activeState(sigmaInstance);

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
    if (sigmaInstance) {
      sigmaInstance.renderers[0].halo({
        nodes: [
          ...activeState.nodes(),
          ...sigmaInstance.pickedNodes,
        ]
      });
    }
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


    onDeleteNodes();
  });



  sigmaInstance.switchEdgesMode = () => {
    sigmaInstance.showEdges = !sigmaInstance.showEdges;
    if (sigmaInstance) {
      sigmaInstance.settings('drawEdges', sigmaInstance.showEdges);
      sigmaInstance.refresh();
    }
  };
  sigmaInstance.switchPlanarMode = () => {
    sigmaInstance.planar = !sigmaInstance.planar;

    let nodes = sigmaInstance.graph.nodes();
    sigmaInstance.graph.clear();
    let edges;
    if (sigmaInstance.planar) {
      edges = RNGEdgesFromNodes(nodes, sigmaInstance.range);
    } else {
      edges = normalEdgesFromNodes(nodes, sigmaInstance.range);
    }
    sigmaInstance.graph.read({nodes, edges});
    sigmaInstance.refresh();
  };


  return sigmaInstance;
}
