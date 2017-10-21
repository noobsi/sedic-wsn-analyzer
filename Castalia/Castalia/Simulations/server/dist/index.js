'use strict';

var _app = require('./app');

var _app2 = _interopRequireDefault(_app);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

const { PORT = 8080 } = process.env;
_app2.default.listen(PORT, () => console.log(`Listening on port ${PORT}`)); // eslint-disable-line no-console
//# sourceMappingURL=index.js.map