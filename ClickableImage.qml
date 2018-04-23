import QtQuick 2.0

Image {
    signal clicked()

    id:root

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
