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
		const data = JSON.parse(result.split("=")[1]);
		$('#time-zone-select').val(data["hours"]);
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
		const data = JSON.parse(result.split("=")[1]);
		$('#alarm-timeout-select').val(data["minutes"]);
	} else if (result.indexOf("alarm/time") >= 0) {
		const data = JSON.parse(result.split("=")[1]);
		const hours = (data["hours"]).toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false});
		const minutes = (data["minutes"]).toLocaleString('en-US', {minimumIntegerDigits: 2, useGrouping:false});
		const text = `${hours}:${minutes}`;
		$('#alarm-display').text(text);
		if (!$('#alarm-set-time-container').is(":visible")) {
			$('#alarm-set-hours').val(hours);
			$('#alarm-set-minutes').val(minutes);
		}
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
	const time_zone = $('#time-zone-select').val();
	if ($('#time-zone-select').val() == "-") {
		sendCommandViaUDP(`set/clock/time={${hours},${minutes}}`);
	} else {
		sendCommandViaUDP(`set/clock/time_zone=${time_zone}`);
	}
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

function editAlarm() {
	$('#edit-alarm').hide();
	$('#alarm-edit-confirm-cancel').show();
	$('#alarm-display').hide();
	$('#alarm-set-time-container').show();
}

function confirmAlarmEdit() {
	const hours = $('#alarm-set-hours').val();
	const minutes = $('#alarm-set-minutes').val();
	const timeout = Number($('#alarm-timeout-select').val());
	sendCommandViaUDP(`set/alarm/time={${hours},${minutes}}`);
	sendCommandViaUDP(`set/alarm/timeout=${timeout}`);
	$('#alarm-set-time-container').hide();
	$('#alarm-edit-confirm-cancel').hide();
	$('#alarm-display').show();
	$('#edit-alarm').show();
}

function cancelAlarmEdit() {
	$('#alarm-set-time-container').hide();
	$('#alarm-edit-confirm-cancel').hide();
	$('#alarm-display').show();
	$('#edit-alarm').show();
}
