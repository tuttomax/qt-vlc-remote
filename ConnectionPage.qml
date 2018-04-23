import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import app.model.ConnectionList 1.0

Page {


    signal getConnection(var connection)
    property int availableConnections: connections.rowCount()
    Component.onDestruction: connections.save()

    ConnectionList {
        id: connections
        onConnected: {
            getConnection(connection)
        }

		onConnectionError: {
			popupError.message = connection.name + ":" + connection.error
			popupError.open()
		}
    }

    ListView {
        anchors.topMargin: 50
        //anchors.rightMargin: 50
        Component.onCompleted: {
            connections.load()
            connectionListView.currentIndex = 0
        }
        Rectangle {
            property alias newName: nameInput.text
            property alias newIp: ipInput.text
            property alias newPort: portInput.text
            property alias newPassword: passwordInput.text

            id: editForm
            visible: false
            x: connectionListView.currentItem.x
            y: connectionListView.currentItem.y
            implicitHeight: connectionListView.currentItem.height
            implicitWidth: connectionListView.currentItem.width

            color: "white"

            ColumnLayout {
                anchors.left: parent.left
                TextInput {
                    id: nameInput
                    text: connectionListView.currentItem.currentConnection.name
                }
                RowLayout {

                    TextInput {
                        id: ipInput
                        inputMask: "000.000.000.000" //TODO: Change mask to 000.000.000.000:0000 so handle this input inside editButton.onClicked
                        inputMethodHints: "ImhDigitsOnly"
                        text: connectionListView.currentItem.currentConnection.ip
                    }
                    TextInput {
                        id: portInput
                        inputMethodHints: "ImhDigitsOnly"
                        maximumLength: 4
                        text: connectionListView.currentItem.currentConnection.port
                    }
                    TextInput {
                        id: passwordInput
                        passwordCharacter: '*'
                        echoMode: TextInput.PasswordEchoOnEdit
                        text: connectionListView.currentItem.currentConnection.password
                    }
                }
            }
        }

        id: connectionListView
        anchors.fill: parent
        model: connections

        delegate: ItemDelegate {
            property variant currentConnection: model
            id: rootDelegate
            anchors.left: parent.left
            anchors.leftMargin: 20
            implicitWidth: rootLayout.width
            implicitHeight: rootLayout.height
            highlighted: index == connectionListView.currentIndex
            RowLayout {
                id: rootLayout
                Image {
                    source: getStatusImage(connectionstate)
                    sourceSize.width: 25
                    sourceSize.height: 25
                    width: 25
                    height: 25
                }

                ColumnLayout {
                    id: column
                    Text {
                        text: name
                        font.bold: true
                    }
                    Text {
                        text: ip + ":" + port
                    }
                }
            }
            onClicked: {
                editForm.visible = false
                connectionListView.currentIndex = index
            }
            onDoubleClicked: {
                connections.connect(connectionListView.currentIndex)
            }

            function getStatusImage(v) {
                console.log("Status:" + v)
                switch (v) {
                case 0:
                    return "/images/lan-connect.png"
                case 1:
                    return "/images/lan-pending.png"
                case 2:
                    return "/images/lan-disconnect.png"
                }
            }
        }

        highlight: Rectangle {

            x: (parent.width - 150 < connectionListView.currentItem.width) ? connectionListView.currentItem.x + connectionListView.currentItem.width : parent.width - 150 //INFO: This give fixed position. No remove.
            RowLayout {
				ClickableImage {
					width: 25
					height: 25
					source: "/images/cast.png"
					sourceSize.width: 25
					sourceSize.height: 25
					onClicked: {
						editForm.visible = false
						var index = connectionListView.currentIndex
						connections.connect(index);
					}
				}

                ClickableImage {
                    width: 25
                    height: 25
                    source: "/images/delete.png"
                    sourceSize.height: 25
                    sourceSize.width: 25

                    onClicked: {
                        var index = connectionListView.currentIndex
                        connections.remove(index)

                        editForm.visible = false
                    }
                }
                ClickableImage {
                    id: editButton
                    width: 25
                    height: 25
                    source: !editForm.visible ? "/images/edit.png" : "/images/check.png"
                    sourceSize.height: 25
                    sourceSize.width: 25

                    onClicked: {
                        if (editForm.visible) {
                            connections.edit(connectionListView.currentIndex,
                                             editForm.newName, "name")
                            connections.edit(connectionListView.currentIndex,
                                             editForm.newIp, "ip")
                            connections.edit(connectionListView.currentIndex,
                                             editForm.newPort, "port")
                            connections.edit(connectionListView.currentIndex,
                                             editForm.newPassword, "password")
                        }
                        editForm.visible = !editForm.visible
                    }
                }
            }
        }

        Rectangle {
            id: floatButton

            Text {
                anchors.centerIn: parent
                color: Qt.lighter("blue")
                text: "+"
                scale: 2
            }

            width: 50
            height: 50
            radius: 25
            color: "lightgray"

            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottomMargin: 10
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("clicked")

                    connections.append("Undefined", "000.000.000.0000",
                                       "8080", "1234")
                    connectionListView.currentIndex = connectionListView.count - 1
                    editForm.visible = true
                }
            }
        }
        DropShadow {
            anchors.fill: floatButton
            cached: true
            horizontalOffset: 0
            verticalOffset: 2
            radius: 4
            samples: 8
            color: "#80000000"
            source: floatButton
        }

		Popup {
			property string message : ""
			id: popupError
			closePolicy: Popup.CloseOnPressOutside
			x: 0
			y: parent.height - height - bottomMargin
			enter: Transition {
					 NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
				 }
			exit: Transition {
					 NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
				 }

			background: Rectangle {
				color: "#606060"
			}
			contentItem: Text {
				id: textError
				text: popupError.message
			}
		}
    }
}
