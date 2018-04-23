import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.2
import "pager.js" as PageManager
ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("VLC Remote")

    SwipeView {
        anchors.fill: parent
        id: pageView
        onCountChanged: pageView.incrementCurrentIndex()
        ConnectionPage {
            onGetConnection: {
				PageManager.createClientPage(pageView,connection)
            }
        }
    }

    footer: PageIndicator {


        id: indicator
        currentIndex: pageView.currentIndex


        count: pageView.count

    }
}
