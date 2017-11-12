import express from 'express';
import path from 'path';
import logger from 'morgan';
import bodyParser from 'body-parser';
import routes from './routes';
import cors from 'cors';

import executor from './executor/exec'

const app = express();

// View engine setup
app.set('views', path.join(__dirname, '../views'));
app.set('view engine', 'pug');

app.use(cors());
app.use(logger('dev', {
  skip: () => app.get('env') === 'test'
}));
app.use(bodyParser.json({limit: '50mb'}));
app.use(bodyParser.urlencoded({limit: '50mb', extended: false }));
app.use(express.static(path.join(__dirname, '../public')));

// Routes
app.use('/', routes);

// Catch 404 and forward to error handler
app.use((req, res, next) => {
  const err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// Error handler
app.use((err, req, res, next) => { // eslint-disable-line no-unused-vars
  res
    .status(err.status || 500)
    .render('error', {
      message: err.message
    });
});

export default app;
