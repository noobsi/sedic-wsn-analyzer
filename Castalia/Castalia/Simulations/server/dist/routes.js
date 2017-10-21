'use strict';

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _express = require('express');

var _exec = require('./executor/exec');

var _exec2 = _interopRequireDefault(_exec);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

const routes = (0, _express.Router)();

/**
 * GET home page
 */
routes.get('/', (req, res) => {
  res.render('index', { title: 'Express Babel' });
});

/**
 * GET /list
 *
 * This is a sample route demonstrating
 * a simple approach to error handling and testing
 * the global error handler. You most certainly want to
 * create different/better error handlers depending on
 * your use case.
 */
routes.post('/exec', (req, res, next) => {
  // executor({
  //   nodes: [
  //     {
  //       id: 1,
  //       x: 0,
  //       y: 0
  //     },
  //     {
  //       id: 2,
  //       x: 0,
  //       y: 40
  //     },
  //     {
  //       id: 3,
  //       x: 40,
  //       y: 40
  //     }
  //   ],
  //   width: 50, height: 50,
  //   source: 1,
  //   destination: 3
  // }, () => {
  //   res.json({
  //     path: "1 2 3 5 6"
  //   })
  // });

  (0, _exec2.default)(req.body, (error, path) => {
    res.json({
      path: path
    });
  });
});

exports.default = routes;
//# sourceMappingURL=routes.js.map