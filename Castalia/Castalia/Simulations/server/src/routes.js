import {Router} from 'express';
import {register} from './executor/exec'
import fs from 'fs'
import path from 'path';
import session from './session'

const routes = Router();

/**
 *
 * GET home page
 */
/**
 * GET /list
 *
 * This is a sample route demonstrating
 * a simple approach to error handling and testing
 * the global error handler. You most certainly want to
 * create different/better error handlers depending on
 * your use case.
 */
routes.post('/exec', (req, res) => {
  register(req.body, (error, simulationResult) => {
    if (error) {
      res.json({
        status: "error",
        error: error.toString(),
      });
    } else {
      res.json(simulationResult);
    }
  })
});

routes.get('/status/:sessionId', (req, res) => {
  const sessionId = req.params.sessionId;
  const ss = session.getSession(sessionId) || {status: "error", error: "invalid session id"};
  if (!ss) {
    res.status(404).json(ss);
    return;
  }

  res.json(ss);
});

routes.get('/result/energy/:sessionId', (req, res, next) => {
  const sessionId = req.params.sessionId;
  const ss = session.getSession(sessionId) || {status: "error", error: "invalid session id"};
  if (ss.status !== 'completed') {
    res.status(404).json(ss);
    return;
  }

  const filePath = path.resolve(`logs/${sessionId}_energyTrace.txt`);

  res.sendFile(filePath, (error) => {
    if (error) {
      res.status(404).json({
        status: "error",
        error: "Invalid session id or simulation not completed"
      })
    }
  })
});

routes.get('/result/log/:sessionId', (req, res, next) => {
  const sessionId = req.params.sessionId;
  const ss = session.getSession(sessionId) || {status: "error", error: "invalid session id"};
  if (ss.status !== 'completed') {
    res.status(404).json(ss);
    return;
  }

  const filePath = path.resolve(`logs/${sessionId}_log.txt`);

  res.sendFile(filePath, (error) => {
    if (error) {
      res.status(404).json({
        status: "error",
        error: "Invalid session id or simulation not completed"
      })
    }
  })
});

routes.get('/result/draw/:sessionId', (req, res, next) => {
  const sessionId = req.params.sessionId;
  const ss = session.getSession(sessionId) || {status: "error", error: "invalid session id"};
  if (ss.status !== 'completed') {
    res.status(404).json(ss);
    return;
  }

  const filePath = path.resolve(`logs/${sessionId}_draw.txt`);

  res.sendFile(filePath, (error) => {
    if (error) {
      res.status(404).json({
        status: "error",
        error: "Invalid session id or simulation not completed"
      })
    }
  })
});

routes.get('/result/event/:sessionId', (req, res, next) => {
  const sessionId = req.params.sessionId;
  const ss = session.getSession(sessionId) || {status: "error", error: "invalid session id"};
  if (ss.status !== 'completed') {
    res.status(404).json(ss);
    return;
  }

  const filePath = path.resolve(`logs/${sessionId}_eventTrace.txt`);

  res.sendFile(filePath, (error) => {
    if (error) {
      res.status(404).json({
        status: "error",
        error: "Invalid session id or simulation not completed"
      })
    }
  })
});

routes.get('/result/node/:sessionId', (req, res, next) => {
  const sessionId = req.params.sessionId;
  const filePath = path.resolve(`logs/${sessionId}_nodeTrace.txt`);
  const ss = session.getSession(sessionId) || {status: "error", error: "invalid session id"};
  if (ss.status !== 'completed') {
    res.status(404).json(ss);
    return;
  }

  fs.readFile(filePath, (err, data) => {
    if (err) {
      res.status(404).json({
        status: "error",
        error: "Invalid session id or simulation not completed"
      })
    } else {
      const nodes = data
        .toString()
        .split('\n')
        .filter(_ => _ !== '')
        .map(_ => JSON.parse(_));

      res.json(nodes)
    }
  });
});

export default routes;
