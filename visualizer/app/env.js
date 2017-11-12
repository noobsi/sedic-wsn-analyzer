let ENDPOINT = "";

if (process.env.NODE_ENV === 'development') {
  ENDPOINT = 'http://localhost:8080';
} else {
  ENDPOINT = "";
}

export default {
  ENDPOINT
};