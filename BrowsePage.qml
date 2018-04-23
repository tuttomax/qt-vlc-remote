import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import "vlccontrollerjs.js" as VLC
import app.model.VLC 1.0


Page {
	id: rootPage
	Component.onDestruction:  console.log("Destruction")
	background: Rectangle {
		anchors.fill: parent
		color: "#F1F1F1"
	}

	VLC { id: vlc_controller }

	property var connectionObject
	property string currentPath

	function nodesChanged(){
		model_inodes.clear()
		for (var i = 0; i < connectionObject.inodes.length; i++) {
			var name = connectionObject.inodes[i].name
			var type = connectionObject.inodes[i].type
			var uri = connectionObject.inodes[i].uri
			var path = connectionObject.inodes[i].path
			var obj = {
				name: name,
				type: type,
				uri: uri,
				path: path
			}
			model_inodes.append(obj)
		}
	}


	Component.onCompleted: {
		VLC.browse(currentPath)
		connectionObject.onInodesChanged.connect(nodesChanged)

	}

	ListModel {
		id: model_inodes
	}

	ListView {
		id: browseList
		anchors.fill: parent
		model: model_inodes
		header: Text {
			text: currentPath
		}
		clip: true
		delegate: RowLayout {
			Image {
				source: type == "dir" ? "/images/folder.png" : "/images/file.png"
			}
			Text {
				text: name
			}
			MouseArea {
				anchors.fill: parent
				onDoubleClicked: {
					if (type == "dir") {
						currentPath = model_inodes.get(index).path
						VLC.browse(currentPath)
					} else {

						VLC.play(model_inodes.get(index).uri)
					}
				}
			}
		}
	}
}
