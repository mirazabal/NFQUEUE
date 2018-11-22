// content of index.js
const http = require('http')
const express = require('express')
const app = express()
const port = 3000


app.get('/', function (req, res, next) {

  var options = {
    root: __dirname, //  + '/public/',
    dotfiles: 'deny',
    headers: {
        'x-timestamp': Date.now(),
        'x-sent': true
    }
  };

  var fileName = 'hallo.txt';// req.params.name;
  res.sendFile(fileName, options, function (err) {
    if (err) {
      next(err);
    } else {
      console.log('Sent:', fileName);
    }
  });

});

//app.get('/', (request, response) => {
//	response.send('Hello from Express!')
//	console.log('Server hello from express sent!!' )
//})

app.listen(port, (err) => {
	if (err) {
	      return console.log('something bad happened', err)
        }

	console.log('server is listening on ${port}')
})


//const requestHandler = (request, response) => {
//  console.log(request.url)
//  response.end('Hello Node.js Server!')
//}

//const server = http.createServer(requestHandler)

//server.listen(port, (err) => {
//  if (err) {
//    return console.log('something bad happened', err)
//  }

//  console.log(`server is listening on ${port}`)
//})


