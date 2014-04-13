import QtQuick 2.2
import QtQuick.Controls 1.1
import FancyControls 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("Streamer Test")

    MjpegStreamerClient {
        id: streamer
        anchors.centerIn: parent
        width: 640
        height: 480
        url: "tcp://192.168.1.18:2001"//"epgm://eth0;239.1.1.1:2001" //"tcp://192.168.1.18:2001"
        running: true
    }

    Text {
        id: fpsText
        anchors.right: streamer.right
        anchors.bottom: streamer.bottom
        text: streamer.fps
        font.pixelSize: parent.height * 0.05
    }

    Text {
        id: timeText
        anchors.left: streamer.left
        anchors.bottom: streamer.bottom
        text: Qt.formatTime(streamer.time, "hh:mm:ss:zzz")
        font.pixelSize: parent.height * 0.05
    }
}
