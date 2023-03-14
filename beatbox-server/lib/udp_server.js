"use strict";
/*
 * Respond to commands over a websocket to relay UDP commands to a local program
 */

var socketio = require('socket.io');
var io;

var dgram = require('dgram');

const fs = require('fs');

// Info for connecting to the local process via UDP
var PORT = 12345;
var HOST = '192.168.7.2';

var client = dgram.createSocket('udp4');

exports.listen = function(server) {
	io = socketio.listen(server);
	io.set('log level 1');
	io.set('heartbeat interval', 1000);

	client.bind(12312);

	io.sockets.on('connection', function(socket) {
		handleCommand(socket);
	});

	client.on('listening', function () {
		var address = client.address();
		console.log('UDP Client: listening on ' + address.address + ":" + address.port);
	});
	// Handle an incoming message over the UDP from the local application.
	client.on("UDP Client: close", function() {
		console.log("closed");
	});
	client.on("UDP Client: error", function(err) {
		console.log("error: ",err);
	});
};

var timer;
var errorDetected = false;
function handleCommand(socket) {
	setInterval(() => { readFile('/proc/uptime', socket); }, 1000);
	socket.on('daUdpCommand', function(data) {
		console.log('daUdpCommand command: ' + data);
		var buffer = new Buffer(data);

		client.send(buffer, 0, buffer.length, PORT, HOST, function(err, bytes) {
			if (err) 
				throw err;
			console.log('UDP message sent to ' + HOST +':'+ PORT);
		});
		clearTimeout(timer);
		timer = setTimeout(() => {
			socket.emit("error", "Receiving no reply from C application");
			errorDetected = true;
		}, 1000);
	});
	client.on('message', function (message, remote) {
		clearTimeout(timer);
		if (errorDetected) {
			errorDetected = false;
			socket.emit("errorClear");
		}
		console.log("UDP Client: message Rx" + remote.address + ':' + remote.port +' - ' + message);

		var reply = message.toString('utf8')
		socket.emit('commandReply', reply);

		//client.close();

	});
};

function readFile(filePath, socket) {
	fs.readFile(filePath, 'utf8', (err, data) => {
		if (err) {
			console.error(err);
			return;
		}
		socket.emit('uptime', data);
	});
}