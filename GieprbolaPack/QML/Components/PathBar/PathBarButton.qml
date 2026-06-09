import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property alias text: label.text
    property alias imageSource: icon.source
    property string tooltipText: ""

    signal clicked()

    width: 40
    height: 40

    radius: 8

    color:
        mouse.pressed ? "#303030" :
        mouse.containsMouse ? "#252525" :
        "#1B1B1B"

    border.width: 1

    border.color:
        mouse.pressed ? "#707070" :
        mouse.containsMouse ? "#505050" :
        "#383838"

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

    Image {
        id: icon

        anchors.centerIn: parent

        width: 20
        height: 20

        visible: source != ""

        fillMode: Image.PreserveAspectFit

        smooth: true
        mipmap: true
    }

    Text {
        id: label

        anchors.centerIn: parent

        visible: icon.source == ""

        color: "#E0E0E0"

        font.pixelSize: 18
        font.bold: true

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouse

        anchors.fill: parent

        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onClicked: root.clicked()
    }

    Rectangle {
        id: tooltip

        visible: mouse.containsMouse && tooltipText.length > 0
        opacity: visible ? 1.0 : 0.0

        z: 999

        anchors.left: parent.right
        anchors.leftMargin: 8

        anchors.bottom: parent.top
        anchors.bottomMargin: 4

        radius: 6

        color: "#2B2B2B"

        border.width: 1
        border.color: "#4A4A4A"

        width: tooltipLabel.implicitWidth + 20
        height: tooltipLabel.implicitHeight + 12

        Text {
            id: tooltipLabel

            anchors.centerIn: parent

            text: tooltipText

            color: "#F0F0F0"
            font.pixelSize: 12
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 120
            }
        }
    }
}