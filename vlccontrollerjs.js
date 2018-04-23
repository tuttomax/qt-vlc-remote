function isPlaying() {
	return connectionObject.context.state === "playing"
}
function isPaused() {
	return connectionObject.context.state === "paused"
}
function isStopped() {
	return connectionObject.context.state === "stopped"
}

function connect() {
	vlc_controller._connect(connectionObject)
}

function resume() {
	vlc_controller.resume(connectionObject)
}

function pause() {
	vlc_controller.pause(connectionObject)
}
function stop() {
	vlc_controller.stop(connectionObject)
}
function next() {
	vlc_controller.next(connectionObject)
}
function previous() {
	vlc_controller.previous(connectionObject)
}

function isFullscreen() {
	return connectionObject.context.fullscreen
}
function toogleFullscreen() {
	vlc_controller.fullscreen(connectionObject)
}
function repeat() {
	return connectionObject.context.loop
}

function toogleLoop() {
	vlc_controller.loop(connectionObject)
}
function volume(value) {
	vlc_controller.volume(connectionObject, value)
}
function seek(value) {
	vlc_controller.seek(connectionObject,
						Math.floor(value * connectionObject.context.length))
}

function browse(path) {
	return vlc_controller.browse(connectionObject, path)
}

function play(uri) {
	vlc_controller.play(connectionObject, uri)
	vlc_controller._connect(connectionObject)
}

function currentPosition() {
	return secondsToTime(
				connectionObject.context.position * connectionObject.context.length)
}
function maxLength() {
	return secondsToTime(connectionObject.context.length)
}

function secondsToTime(seconds) {
	var h = Math.floor(seconds / 3600)
	var m = Math.floor((seconds - h * 3600) / 60)
	var s = Math.floor(seconds - m * 60 - h * 3600)
	var final = []
	//    if (h>0)
	if (h < 10)
		final.push("0" + h.toString())
	else
		final.push(h.toString())

	//    if (m>0)
	if (m < 10)
		final.push("0" + m.toString())
	else
		final.push(m.toString())

	//    if (s>0)
	if (s < 10)
		final.push("0" + s.toString())
	else
		final.push(s.toString())
	return final.join(":")
}
