import QtQuick

Rectangle {
    id: root

    property alias text: label.text

    signal clicked()

    width: parent ? parent.width : 160
    height: 30
    radius: 5

    color:
        mouse.pressed ? "#404040" :
        mouse.containsMouse ? "#353535" :
        "#2A2A2A"

    Text {
        id: label

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10

        color: "#E0E0E0"
        font.pixelSize: 14
    }

    MouseArea {
        id: mouse

        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onClicked: root.clicked()
    }

    Behavior on color {
        ColorAnimation {
            duration: 70
        }
    }
}