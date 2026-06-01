import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "#232323"
    border.color: "#2d2d2d"
    border.width: 1
    height: 64

    RowLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        TextField {
            id: pathField

            Layout.fillWidth: true
            height: 40

            text: FileSystemModel.currentPath

            color: "#f1f1f1"
            font.pixelSize: 14

            leftPadding: 12
            rightPadding: 12

            selectByMouse: true

            onAccepted: {
                FileSystemModel.currentPath = text
            }

            background: Rectangle {
                radius: 8
                color: "#1b1b1b"

                border.width: 1
                border.color: pathField.activeFocus
                    ? "#4e8cff"
                    : "#383838"
            }
        }
    }

    Connections {
        target: FileSystemModel

        function onCurrent_path_changed() {
            pathField.text = FileSystemModel.currentPath
        }
    }
}