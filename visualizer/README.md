
## WSN visualizer tool

* Install (if you don't have them):
    * [Node.js](http://nodejs.org) Make sure nodejs and npm is installed
    * [Brunch](http://brunch.io): `npm install -g brunch`
    * Brunch plugins and app dependencies: `npm install`
* Run:
    * `npm start` — watches the project with continuous rebuild. This will also launch HTTP server with [pushState](https://developer.mozilla.org/en-US/docs/Web/Guide/API/DOM/Manipulating_the_browser_history).
    * `npm run build` — builds minified project for production
    * Go to [localhost](http://localhost:3333/) for visualizer web tool
    
* Info
    * This project use [sigma.js](http://sigmajs.org/) with various plugins from
    [Linkurious](https://github.com/Linkurious/linkurious.js) to generate and manipulate sensor graphs
    * v0.0.1 Client requests for routing result between two chosen nodes by sending a json with 
     following format: 
     ```
      {
        "nodes": [
          {
            "id": "1", "x": 0, "y": 0
          },
          {
            "id": "2", "x": 0, "y": 40
          },
          {
            "id": "3", "x": 40, "y": 40
          },
          {
            "id": "4", "x": 40, "y": 80
          }	
        ],
        "width": 80,
        "height": 80,
        "source": "1",
        "destination": "4"
      }
     ```
     The result has the format: 
     ```
     {
         "path": ["1", "2", "3", "4"]
     }
     ```
     
