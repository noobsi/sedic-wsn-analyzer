
## Server for visualizer tool
* Serves as an endpoint for visualizer client to send simulation request
* Basically, the server is responsible for:
  * Accepting simulation requests via HTTP RESTful API 
  * v0.0.1 only `/exec` endpoint - Simple Pair Application
  * Creating ini file from simulation request
  * Calling Castalia to execute simulation
  * Parsing trace file for result and send back to the client
  * Server is in Castalia/Simulations/ directory so we could run Castalia using command line 
* Install (if you don't have them):
    * [Node.js](http://nodejs.org) Make sure nodejs and npm is installed
    * Brunch plugins and app dependencies: `npm install`
* Run:
    * `npm run dev` — watches the project with continuous rebuild and run using nodemon
    * `npm run build` — builds minified project for production
    * NOTE: only using `npm run dev` can we trigger rebuild and have the newest version, 
    using `npm start` will only serve the last-built version

