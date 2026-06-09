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

        TextField {
            id: pathField

            Layout.fillWidth: true
            Layout.preferredHeight: 40

            text: FileSystemModel.currentPath

            color: "#f1f1f1"
            selectedTextColor: "#ffffff"
            selectionColor: "#4e8cff"

            placeholderText: "Enter folder path..."
            placeholderTextColor: "#777777"

            font.pixelSize: 14
            font.family: "Consolas"

            leftPadding: 42
            rightPadding: 12

            selectByMouse: true
            persistentSelection: true

            horizontalAlignment: TextInput.AlignLeft
            verticalAlignment: TextInput.AlignVCenter

            Keys.onEscapePressed: {
                text = FileSystemModel.currentPath
                focus = false
            }

            onAccepted: {
                root.commit_path()
            }

            onActiveFocusChanged: {
                if (activeFocus) {
                    cursorPosition = length
                }
            }

            background: Rectangle {
                radius: 8
                color: pathField.activeFocus ? "#202020" : "#1b1b1b"

                border.width: 1
                border.color: pathField.activeFocus
                    ? "#4e8cff"
                    : pathField.hovered
                        ? "#505050"
                        : "#383838"
            }

            Button {
                id: chooseFolderButton

                width: 28
                height: 28

                anchors.left: parent.left
                anchors.leftMargin: 6
                anchors.verticalCenter: parent.verticalCenter

                z: 10

                background: Rectangle {
                    radius: 6
                    color: chooseFolderButton.hovered ? "#303030" : "transparent"
                }

                Image {
                    anchors.centerIn: parent

                    source: "qrc:/qt/qml/gieprbolapack/Resources/FileExplorer.svg"

                    width: 22
                    height: 22

                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    mipmap: true

                    opacity: chooseFolderButton.enabled ? 1.0 : 0.5
                }

                onClicked: {
                    const path = FileSystemModel.select_folder(FileSystemModel.currentPath)

                    if (path.length > 0) {
                        pathField.text = path
                        FileSystemModel.currentPath = path
                    }
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