import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import app.model.VLC 1.0
import "vlccontrollerjs.js" as VLC
import "pager.js" as PageManager

Page {
	property var browsepageFactory: Qt.createComponent("qrc:/BrowsePage.qml")
	property var connectionObject

	VLC {
		id: vlc_controller
	}

	Component.onCompleted: {
		if (connectionObject === undefined)
			console.error("connection is not defined")
		connectionObject.onErrorChanged.connect(function () {
			popupError.open()
		})

		clientTimer.start()
	}

	StackView {
		id: stack
		width: parent.width * 0.3
		height: parent.height
		z: 999
	}

	Timer {
		id: clientTimer
		interval: 5000 //1s
		onTriggered: VLC.connect(
						 ) //FIXME: Memory consumin,battery consumin, network consuming
		repeat: true
	}

	header: ColumnLayout {

		ColumnLayout {
			Layout.alignment: Qt.AlignVertical_Mask
			RowLayout {
				Layout.alignment: Qt.AlignLeft
				ClickableImage {
					source: connectionObject.status
							=== 0 ? "/images/cast-connected.png" : "/images/cast.png"
					onClicked: VLC.connect()
				}
				Text {
					text: connectionObject.name
				}
			}
			Text {
				anchors.left: parent.left
				text: connectionObject.context.information
					  === undefined ? "" : connectionObject.context.information.category.meta.filename
				elide: Text.ElideMiddle
				wrapMode: Text.WrapAnywhere
			}
		}

		RowLayout {
			Layout.leftMargin: 20
			spacing: 20
			ClickableImage {
				source: "/images/video.png"
				onClicked: {
					console.log("Clicked browse button")

					if (stack.depth === 0)
						stack.push(browsepageFactory.createObject(parent, {
																	  connectionObject: connectionObject,
																	  currentPath: "~"
																  }))
					else if (stack.depth === 1)
						stack.clear()
				}
			}

			ClickableImage {
				source: "/images/playlist.png"
			}
			ClickableImage {
				source: "/images/youtube.png"
			}
			ClickableImage {
				source: "/images/web.png"
			}
		}
	}

	ColumnLayout {
		anchors.fill: parent

		Item {
			Layout.fillHeight: true
		}

		Text {
			text: VLC.currentPosition() + "/" + VLC.maxLength()
		}

		Slider {
			anchors.right: parent.right
			anchors.left: parent.left
			id: seekSlider
			from: 0.000000000000000
			to: 1.000000000000000
			live: false
			value: connectionObject.context.position
			onValueChanged: {
				if (pressed) {
					seekSlider.value = value
					VLC.seek(value)
				}
			}
		}
		RowLayout {
			anchors.horizontalCenter: parent.horizontalCenter
			spacing: 20
			ClickableImage {
				source: VLC.repeat(
							) ? "/images/repeat-off.png" : "/images/repeat.png"
				onClicked: VLC.toogleLoop()
			}

			RowLayout {

				ClickableImage {
					source: "/images/skip-previous.png"
					onClicked: VLC.previous()
				}
				ClickableImage {
					source: VLC.isPlaying(
								) ? "/images/pause.png" : "/images/play.png"
					onClicked: {
						if (VLC.isPlaying()) {
							VLC.pause()
							clientTimer.stop()
						} else {
							VLC.resume()
							clientTimer.start()
						}
					}
				}
				ClickableImage {
					source: "/images/skip-next.png"
					onClicked: VLC.next()
				}
			}

			ClickableImage {
				source: VLC.isFullscreen(
							) ? "/images/fullscreen-exit.png" : "/images/fullscreen.png"
				onClicked: VLC.toogleFullscreen()
			}

			ClickableImage {
				source: "/images/volume.png"
				onClicked: volumeSlider.visible = !volumeSlider.visible

				Slider {
					visible: false
					id: volumeSlider
					orientation: "Vertical"
					z: 999999
					anchors.bottom: parent.top
					anchors.horizontalCenter: parent.horizontalCenter
					height: 150
					from: 1
					stepSize: 1
					to: 320 //MAX=125%
					live: false
					value: connectionObject.context.volume
					handle: Item {
						visible: false
					}
					onValueChanged: {
						if (pressed) {
							volumeSlider.value = value
							VLC.volume(value)
						}
					}
				}
			}

			ClickableImage {
				source: "/images/stop.png"
				onClicked: VLC.stop()
			}
		}

		Popup {

			id: popupError
			closePolicy: Popup.CloseOnPressOutside
			background: Rectangle {
				color: "#606060"
			}
			Text {
				text: connectionObject.error
			}
		}
	}
}
