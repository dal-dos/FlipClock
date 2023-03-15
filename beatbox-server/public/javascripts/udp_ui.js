"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();

var startTimer;
var serverErrorDetected;
var beatboxErrorDetected;
$(document).ready(function() {
	sendCommandViaUDP("get beat");
	sendCommandViaUDP("get volume");
	sendCommandViaUDP("get tempo");
	
	$('#stop-program-button').click(function(){
		sendCommandViaUDP("stop");
	});

	socket.on('commandReply', function(result) {
		clearTimeout(startTimer);
		serverErrorDetected = false;
		$('#error-container').hide();
		$('#error-text').text("");
		if (result.indexOf("beat ") >= 0) {
			$('#beat-value').text(result.substring("beat ".length));
		}
	});

	socket.on('uptime', function(result) {
		if (serverErrorDetected && !beatboxErrorDetected) {
			serverErrorDetected = false;
			$('#error-container').hide();
			$('#error-text').text("");
		}
		var times = result.split(" ");
		$('#uptime-value').text(new Date(times[0] * 1000).toISOString().slice(11, 19));
	});

	socket.on('error', function(result) {
		beatboxErrorDetected = true;
		$('#error-text').text(result);
		$('#error-container').show();
	});

	socket.on('errorClear', () => {
		beatboxErrorDetected = false;
		$('#error-container').hide();
		$('#error-text').text('');
	});
});

function sendCommandViaUDP(message) {
	clearTimeout(startTimer);
	startTimer = setTimeout(() => {
		$('#error-container').show();
		$('#error-text').text("No response from server.");
		serverErrorDetected = true;
	}, 1000);
	socket.emit('daUdpCommand', message);
};