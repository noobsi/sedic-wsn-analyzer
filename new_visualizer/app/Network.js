import inPolygon from 'point-in-polygon';
import Mousetrap from 'mousetrap';

const originPosition = ({x, y, svg}) => {
  let pt = svg.createSVGPoint();
  pt.x = x;
  pt.y = y;
  let svgP = pt.matrixTransform(svg.getScreenCTM().inverse());
  return {
    x: svgP.x, y: svgP.y
  }
};
const distance = (p1, p2) => {
  return Math.sqrt((p1.x - p2.x) ** 2 + (p1.y - p2.y) ** 2);
};
const ccwAngle = (a, b) => {
  // from a to b
  let r = b - a;
  if (r < 0) return r + 360;
  else return r;
};
const findCenters = ({x1, y1, x2, y2, radius: r}) => {
  let q = Math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2);
  // midpoint
  let y3 = (y1 + y2) / 2;
  let x3 = (x1 + x2) / 2;

  return [
    {
      x: x3 + Math.sqrt(r ** 2 - (q / 2) ** 2) * (y1 - y2) / q,
      y: y3 + Math.sqrt(r ** 2 - (q / 2) ** 2) * (x2 - x1) / q
    },
    {
      x: x3 - Math.sqrt(r ** 2 - (q / 2) ** 2) * (y1 - y2) / q,
      y: y3 - Math.sqrt(r ** 2 - (q / 2) ** 2) * (x2 - x1) / q
    }
  ]
};
const processNeighbors = ({nodes, range}) => {
  let V = nodes.length;
  for (let node of nodes) {
    node.neighbors = [];
    node.neighborIds = [];
  }

  for (let i = 0; i < V; i++) {
    for (let j = i + 1; j < V; j++) {
      if ((nodes[i].x - nodes[j].x) ** 2 + (nodes[i].y - nodes[j].y) ** 2 <= range ** 2) {
        nodes[i].neighbors.push(nodes[j]);
        nodes[i].neighborIds.push(j);
        nodes[j].neighbors.push(nodes[i]);
        nodes[j].neighborIds.push(i);
      }
    }
  }

  return nodes;
};
const angle = ({x: x1, y: y1}, {x: x2, y: y2}) => {
  let ag = Math.atan2(y2 - y1, x2 - x1) * 180 / Math.PI;
  if (ag < 0) {
    ag += 360;
  }

  return ag;
};
const findBallCenter = ({node, ballDiameter}) => {
  let candidates = [];
  let radius = ballDiameter / 2;
  for (let neighbor of node.neighbors) {
    let {x: x1, y: y1} = node;
    let {x: x2, y: y2} = neighbor;

    for (let center of findCenters({x1, y1, x2, y2, radius})) {
      let ok = true;
      for (let otherNeighbor of node.neighbors) {
        if (otherNeighbor === neighbor) continue;
        if (distance(center, otherNeighbor) < radius) {
          ok = false;
        }
      }

      if (ok) {
        candidates.push(center);
      }
    }
  }

  candidates.sort((center1, center2) => angle(node, center1) - angle(node, center2));
  if (candidates.length) {
    return candidates[0];
  } else {
    return null;
  }
};

function findCenterOnClick({nodes, ballDiameter, onFindCenter}) {
  nodes.forEach(node => {
    node.circle.off('click');
    node.circle.click((e) => {
      let center = findBallCenter({node, ballDiameter});
      if (center) {
        onFindCenter({center, node});
      }
    })
  });

  return nodes;
}


const NODE_CIRCLE_RADIUS = 2;

class Network {
  constructor({container, nodes, width, height, range}) {
    this.draw = SVG(container).size("100%", "100%").panZoom();
    this.nodes = nodes;
    this.width = width;
    this.height = height;
    this.range = range;
    // this.draw.flip('y');

    this.svg = this.draw.node;
    this.selectLayer = this.draw.group();
    this.haloLayer = this.draw.group();
    this.edgeLayer = this.draw.group();
    this.nodeLayer = this.draw.group();

    this.nodes.forEach(node => {
      node.circle = this.nodeLayer
        .circle(NODE_CIRCLE_RADIUS * 2)
        .center(node.x, node.y)
        .fill('#9f9f9f');
      node.circle.mouseover(() => {
        console.log(node.id);
      })
    });

    processNeighbors({nodes, range});
    findCenterOnClick({
      nodes, ballDiameter: range, onFindCenter: ({center}) => {
        this.haloLayer
          .circle(range)
          .center(center.x, center.y)
          .fill('none')
          .stroke({color: '#32ffc2', width: 0.5})
      }
    });


    let state = 'normal';
    let isSeleting = false;
    let polylines = [];

    this.draw.mousedown((e) => {
      if (state === 'deleting') {
        isSeleting = true;
        let {x, y} = originPosition({x: e.clientX, y: e.clientY, svg: this.svg});
        let polyline = this.selectLayer.polyline([x, y]).fill('#ffccca').stroke({width: 0.5});
        polylines.push(polyline);
      }
    });

    this.draw.mousemove((e) => {
      if (state === 'normal') {
      } else if (state === 'deleting') {
        if (isSeleting) {
          let {x, y} = originPosition({x: e.clientX, y: e.clientY, svg: this.svg});
          let polyline = polylines[polylines.length - 1];
          polyline.plot(polyline.array().value.concat([[x, y]])).fill('#ffccca').stroke({width: 0.5});
        }
      }

    });

    this.draw.mouseup((e) => {
      if (state === 'deleting') {
        if (isSeleting) {
          isSeleting = false;
          state = 'normal';
          this.draw.panZoom();

          for (let node of this.nodes) {
            let {x, y} = node;
            for (let polyline of polylines) {
              if (inPolygon([x, y], polyline.array().value)) {
                node.halo = this.haloLayer
                  .circle(4 * NODE_CIRCLE_RADIUS)
                  .center(x, y)
                  .fill('#ff6262')
              }
            }
          }
        }
      }
    });

    Mousetrap.bind(['command+d', 'ctrl+shift+d'], () => {
      if (state === 'normal') {
        state = 'deleting';
        this.draw.panZoom(false)
      }
    });

    Mousetrap.bind(['del', 'backspace'], () => {
      this.nodes
        .filter(node => node.halo)
        .forEach(node => {
          node.halo.remove();
          node.circle.remove();
        });
      this.nodes = this.nodes
        .filter(node => !node.halo)
        .map((node, i) => {
          node.id = i
          return node;
        });
      this.nodes.forEach(node => {
        node.circle.off('mouseover');
        node.circle.mouseover(() => {
          console.log(node.id);
        })
      });
      polylines.forEach(p => p.remove());
    });

    Mousetrap.bind('esc', () => {
      state = 'normal';
      this.nodes
        .filter(node => node.halo)
        .forEach(node => {
          node.halo.remove();
          delete node.halo;
        });
      polylines.forEach(p => p.remove());
      polylines = [];
    });

    this.indicatorLayer = this.draw.group().move(width + 20, 20);
    this.currentY = 40;

  }

  remove() {
    this.draw.remove();
  }

  addIndicator({color, name}) {
    let currentY = this.currentY;
    this.indicatorLayer.line(0, currentY, 100, currentY).stroke({width: 0.5, color});
    this.indicatorLayer.text(name).font({family: "Menlo"}).move(110, currentY - 8);
    this.currentY += 20;
  };


  drawLine({x1, y1, x2, y2, style}) {
    return this.edgeLayer.line(x1, y1, x2, y2).stroke(style);
  }

  drawCircle({centerX, centerY, radius, style}) {
    return this.haloLayer
      .circle(radius * 2)
      .center(centerX, centerY)
      .fill('none')
      .stroke(style);
  }

  drawPoint({x, y, color}) {
    this.nodeLayer
      .circle(NODE_CIRCLE_RADIUS * 4)
      .center(x, y)
      .fill(color);
  }
}


export default Network;