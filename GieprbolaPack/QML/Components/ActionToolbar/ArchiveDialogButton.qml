import QtQuick

Rectangle {
    id: root

    property alias text: label.text

    property color normalColor: "#2B2B2B"
    property color hoverColor: "#343434"
    property color pressedColor: "#404040"

    signal clicked()

    width: 90
    height: 32
    radius: 5

    color: mouse.pressed ? pressedColor
         : mouse.containsMouse ? hoverColor
         : normalColor

    border.width: 1
    border.color: mouse.pressed ? "#707070"
                 : mouse.containsMouse ? "#5A5A5A"
                 : "#454545"

    Text {
        id: label

        anchors.centerIn: parent

        color: "#EAEAEA"
        font.pixelSize: 14
        font.bold: true
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
            duration: 80
        }
    }

    Behavior on border.color {
        ColorAnimation {
            duration: 80
        }
    }
}