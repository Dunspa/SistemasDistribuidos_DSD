var http = require("http");
var httpServer = http.createServer(
	function(request, response) {
		console.log(request.headers);
		response.writeHead(200, {"Content-Type": "text/plain"});
		response.write("Hola mundo");
		response.end();
	}
);
httpServer.listen(8080);
console.log("Servicio HTTP iniciado");