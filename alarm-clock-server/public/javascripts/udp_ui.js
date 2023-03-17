"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();

$(document).ready(function() {
	sendCommandViaUDP("get/clock/time_zone");
	sendCommandViaUDP("get/clock/time");
	sendCommandViaUDP("get/alarm/timeout");
	sendCommandViaUDP("get/alarm/time");

	setInterval(() => {
		sendCommandViaUDP("get/clock/time");
	}, 1000);
	
	$('#stop-program-button').click(function(){
		sendCommandViaUDP("stop");
	});

	socket.on('commandReply', function(result) {
		handleReply(result);
	});
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
};

function handleReply(result) {
	if (result.indexOf("clock/time_zone") >= 0) {
		$('#beat-value').text(result.substring("beat ".length));
	} else if (result.indexOf("clock/time") >= 0) {
		const data = JSON.parse(result.split("=")[1]);
		const hours = (data["hours"]).toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false});
		const minutes = (data["minutes"]).toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false});
		const seconds = (data["seconds"]).toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false});
		const text = `${hours}:${minutes}:${seconds}`;
		$('#clock-display').text(text);
		if (!$('#clock-set-time-container').is(":visible")) {
			$('#clock-set-hours').val(hours);
			$('#clock-set-minutes').val(minutes);
		}
	} else if (result.indexOf("alarm/timeout") >= 0) {
		$('#beat-value').text(result.substring("beat ".length));
	} else if (result.indexOf("alarm/time") >= 0) {
		const data = JSON.parse(result.split("=")[1]);
		const hours = (data["hours"]).toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false});
		const minutes = (data["minutes"]).toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false});
		const text = `Next alarm at: ${hours}:${minutes}`;
		$('#alarm-display').text(text);
	}
}

function editClock() {
	$('#edit-clock').hide();
	$('#clock-edit-confirm-cancel').show();
	$('#clock-display').hide();
	$('#clock-set-time-container').show();
}

function confirmClockEdit() {
	const hours = $('#clock-set-hours').val();
	const minutes = $('#clock-set-minutes').val();
	sendCommandViaUDP(`set/clock/time={${hours},${minutes}}`);
	$('#clock-set-time-container').hide();
	$('#clock-edit-confirm-cancel').hide();
	$('#clock-display').show();
	$('#edit-clock').show();
}

function cancelClockEdit() {
	$('#clock-set-time-container').hide();
	$('#clock-edit-confirm-cancel').hide();
	$('#clock-display').show();
	$('#edit-clock').show();
}