var http = require("http");
var url = require("url");
var fs = require("fs");
var path = require("path");
var socketio = require("socket.io");
var MongoClient = require('mongodb').MongoClient;
var MongoServer = require('mongodb').Server;
var mimeTypes = { "html": "text/html", "jpeg": "image/jpeg", "jpg": "image/jpeg", "png": "image/png", "js": "text/javascript", "css": "text/css", "swf": "application/x-shockwave-flash"};

// Desplegar servicio HTTP para IOT que simula una casa
var iot = http.createServer(
	function(request, response) {
      // Seleccionar cliente o sensores iot
		var uri = url.parse(request.url).pathname;
      if (uri == "/iot") 
         uri = "/iot.html";
      else if (uri == "/cliente")
         uri = "/cliente.html";
      
      // Servir página, controlando errores
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

var t = new Date();
var medidas = {temperatura: 0, luminosidad: 0, tiempo: t};
// MongoDB para almacenar un histórico del cambio en las medidas
MongoClient.connect("mongodb://localhost:27017/", function(err, db) {
   iot.listen(8080);
   var io = socketio.listen(iot);
   console.log("Servicio IOT iniciado");

	var dbo = db.db("P4");
	dbo.createCollection("IOT", function(err, collection){
      // Envío y recepción de mensajes con el servicio IOT
    	io.sockets.on('connection',
		function(client) {
         console.log('Nueva conexión del cliente ' + client.request.connection.remoteAddress + ':' + client.request.connection.remotePort);
         
         // Mostrar medidas e históricos a todos los clientes IOT
         io.sockets.emit('medidas-casa', medidas);
         collection.find().toArray(function(err, results){
            client.emit('obtener-medidas', results);
         });
         
         // Los sensores envían datos, actualizar BD con histórico de medidas
         client.on('sensores', function (data) {
            medidas = data;
            collection.insert(data, {safe:true}, function(err, result) {});

            var alerta = "Las medidas están en unos valores correctos";

            // AGENTE: Detecta cuando se sobrepasa un cierto umbral
            if (data.temperatura > 100) {
               if (data.luminosidad > 100) {
                  alerta = "ALERTA: Temperatura y luminosidad demasiado altas. Cerrando persiana y encendiendo aire acondicionado.";
                  io.sockets.emit('cierra-persiana');
               } else if (data.luminosidad < 0) {
                  alerta = "ALERTA: Temperatura demasiado alta y luminosidad demasiado baja. Abriendo persiana y encendiendo aire acondicionado.";
                  io.sockets.emit('abre-persiana');
               } else {
                  alerta = "ALERTA: Temperatura demasiado alta. Encendiendo aire acondicionado.";
               }

               io.sockets.emit('enciende-aire');
            } else if (data.temperatura < -100) {
               if (data.luminosidad > 100) {
                  alerta = "ALERTA: Temperatura demasiado baja y luminosidad demasiado alta. Cerrando persiana y apagando aire acondicionado.";
                  io.sockets.emit('cierra-persiana');
               } else if (data.luminosidad < 0) {
                  alerta = "ALERTA: Temperatura demasiado y luminosidad demasiado bajas. Abriendo persiana y apagando aire acondicionado.";
                  io.sockets.emit('abre-persiana');
               } else {
                  alerta = "ALERTA: Temperatura demasiado baja. Apagando aire.";
               }

               io.sockets.emit('apaga-aire');
            } else if (data.luminosidad > 100) {
               alerta = "ALERTA: Luminosidad demasiado alta. Cerrando persiana.";
               io.sockets.emit('cierra-persiana');
            } else if (data.temperatura < 0) {
               alerta = "ALERTA: Luminosidad demasiado baja. Abriendo persiana.";
               io.sockets.emit('abre-persiana');
            }

            // Se envía a los clientes el mensaje de alerta
            io.sockets.emit("alerta", alerta, data);

            // Se envía a los clientes las nuevas medidas que nos dan los sensores
            io.sockets.emit('medidas-casa', data);
         });

         // Apagar el aire = +20 grados
         client.on('apaga-aire-cliente', function() {
            io.sockets.emit('apaga-aire');
         });

         // Encender el aire = -20 grados
         client.on('enciende-aire-cliente', function() {
            io.sockets.emit('enciende-aire');
         });

         // Cerrar la persiana = -20 luminosidad
         client.on('cierra-persiana-cliente', function() {
            io.sockets.emit('cierra-persiana');
         });

         // Abrir la persiana = +20 luminosidad
         client.on('abre-persiana-cliente', function() {
            io.sockets.emit('abre-persiana');
         });
         
			client.on('obtener-medidas', function (data) {
				collection.find().toArray(function(err, results){
					client.emit('obtener-medidas', results);
				});
         });
         
         client.on('disconnect', function() {
			   console.log('El cliente ' + client.request.connection.remoteAddress + ' se ha desconectado');
		   });
		});
   });
});

console.log("Servicio MongoDB iniciado");