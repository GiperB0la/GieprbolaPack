import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../../Styles"

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

            text: FileSystemModel.currentPath

            onAccepted: {
                FileSystemModel.currentPath = text
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