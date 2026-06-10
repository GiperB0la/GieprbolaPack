import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../../Styles"

Rectangle {
    id: root

    color: "#232323"
    border.color: "#2d2d2d"
    border.width: 1
    height: 64

    function commit_path() {
        const path = pathField.text.trim()

        if (path.length === 0) {
            pathField.text = FileSystemModel.currentPath
            return
        }

        FileSystemModel.currentPath = path
        pathField.focus = false
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        PathBarButton {
            Layout.preferredWidth: 40
            Layout.preferredHeight: 40

            imageSource: "qrc:/qt/qml/gieprbolapack/Resources/Arrow.png"
            tooltipText: "Go to parent folder"

            onClicked: {
                FileSystemModel.go_parent()
            }
        }

        FolderPathField {
            id: pathField

            text: FileSystemModel.currentPath
            placeholderText: "Enter folder path..."

            updateCurrentPath: true

            onAccepted: {
                root.commit_path()
            }

            Keys.onEscapePressed: {
                text = FileSystemModel.currentPath
                focus = false
            }

            onActiveFocusChanged: {
                if (activeFocus) {
                    cursorPosition = length
                }
            }
        }
    }

    Connections {
        target: FileSystemModel

        function onCurrent_path_changed() {
            if (!pathField.activeFocus) {
                pathField.text = FileSystemModel.currentPath
            }
        }
    }
}