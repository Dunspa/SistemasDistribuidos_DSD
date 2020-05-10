var http = require("http");
var url = require("url");
var fs = require("fs");
var path = require("path");
var socketio = require("socket.io");
var mimeTypes = { "html": "text/html", "jpeg": "image/jpeg", "jpg": "image/jpeg", "png": "image/png", "js": "text/javascript", "css": "text/css", "swf": "application/x-shockwave-flash"};

var httpServer = http.createServer(
	function(request, response) {
		var uri = url.parse(request.url).pathname;
		if (uri=="/") uri = "/connections.html";
		var fname = path.join(process.cwd(), uri);
		fs.exists(fname, function(exists) {
			if (exists) {
				fs.readFile(fname, function(err, data){
					if (!err) {
						var extension = path.extname(fname).split(".")[1];
						var mimeType = mimeTypes[extension];
						response.writeHead(200, mimeType);
						response.write(data);
						response.end();
					}
					else {
						response.writeHead(200, {"Content-Type": "text/plain"});
						response.write('Error de lectura en el fichero: '+uri);
						response.end();
					}
				});
			}
			else{
				console.log("Peticion invalida: "+uri);
				response.writeHead(200, {"Content-Type": "text/plain"});
				response.write('404 Not Found\n');
				response.end();
			}
		});
	}
);
httpServer.listen(8080);
var io = socketio.listen(httpServer);

var allClients = new Array();
io.sockets.on('connection',
	function(client) {
		allClients.push({address:client.request.connection.remoteAddress, port:client.request.connection.remotePort});
		console.log('New connection from ' + client.request.connection.remoteAddress + ':' + client.request.connection.remotePort);
		io.sockets.emit('all-connections', allClients);
		client.on('output-evt', function (data) {
         client.emit('output-evt', 'Hola Cliente!');
		});
		client.on('disconnect', function() {
			console.log("El cliente "+client.request.connection.remoteAddress+" se va a desconectar");
			console.log(allClients);

			var index = -1;
			for(var i = 0; i<allClients.length;i++){
				//console.log("Hay "+allClients[i].port);
				if(allClients[i].address == client.request.connection.remoteAddress
					&& allClients[i].port == client.request.connection.remotePort){
					index = i;
				}			
			}

			if (index != -1) {
				allClients.splice(index, 1);
				io.sockets.emit('all-connections', allClients);
			}else{
				console.log("EL USUARIO NO SE HA ENCONTRADO!")
			}
			console.log('El usuario '+client.request.connection.remoteAddress+' se ha desconectado');
		});
	}
);

console.log("Servicio Socket.io iniciado");

