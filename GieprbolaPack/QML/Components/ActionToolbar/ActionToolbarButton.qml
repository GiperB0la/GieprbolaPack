import QtQuick
import QtQuick.Controls

Rectangle {
    id: root

    property alias text: label.text
    property alias imageSource: icon.source

    property string tooltipText: ""

    signal clicked()

    width: 72
    height: 72

    radius: 8

    color:
        mouse.pressed ? "#404040" :
        mouse.containsMouse ? "#2E2E2E" :
        "#262626"

    border.width:
        mouse.pressed ? 1 :
        mouse.containsMouse ? 1 :
        1

    border.color:
        mouse.pressed ? "#6A6A6A" :
        mouse.containsMouse ? "#5A5A5A" :
        "#3A3A3A"

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

    Column {
        anchors.centerIn: parent

        spacing: 4

        Image {
            id: icon

            anchors.horizontalCenter: parent.horizontalCenter

            width: 48
            height: 48

            fillMode: Image.PreserveAspectFit

            smooth: true
            mipmap: true
        }

        Text {
            id: label

            width: root.width - 8

            horizontalAlignment: Text.AlignHCenter

            color: "#E0E0E0"

            font.pixelSize: 12

            elide: Text.ElideRight
        }
    }

    MouseArea {
        id: mouse

        anchors.fill: parent

        hoverEnabled: true

        cursorShape: Qt.PointingHandCursor

        onClicked: {
            root.clicked()
        }
    }

    Rectangle {
        id: tooltip

        visible: mouse.containsMouse && tooltipText.length > 0
        opacity: visible ? 1.0 : 0.0

        z: 999

        anchors.bottom: parent.top
        anchors.bottomMargin: 8

        x: (parent.width - width) / 3

        radius: 6
        color: "#2B2B2B"

        border.color: "#4A4A4A"
        border.width: 1

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