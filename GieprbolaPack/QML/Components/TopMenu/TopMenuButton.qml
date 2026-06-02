import QtQuick

Rectangle {
    id: root

    property alias text: label.text
    property color normalColor: "#232323"

    signal clicked()

    width: label.implicitWidth + 24
    height: 30
    radius: 5

    color: mouse.pressed ? "#3A3A3A"
         : mouse.containsMouse ? "#2E2E2E"
         : normalColor

    border.width: mouse.containsMouse || mouse.pressed ? 1 : 0
    border.color: mouse.pressed ? "#5A5A5A" : "#454545"

    Text {
        id: label
        anchors.centerIn: parent
        color: "#E0E0E0"
        font.pixelSize: 15
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }

    Behavior on color {
        ColorAnimation { duration: 70 }
    }

    Behavior on border.color {
        ColorAnimation { duration: 70 }
    }
}