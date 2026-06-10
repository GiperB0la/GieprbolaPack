import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

TextField {
    id: root

    property string startFolder: FileSystemModel.currentPath
    property string defaultFileName: ""
    property bool updateCurrentPath: false

    signal folderSelected(string folder)

    function build_path(folder) {
        if (defaultFileName.length === 0) {
            return folder
        }

        return folder + "/" + defaultFileName
    }

    Layout.fillWidth: true
    Layout.preferredHeight: 40

    color: "#f1f1f1"
    selectedTextColor: "#ffffff"
    selectionColor: "#4e8cff"

    font.pixelSize: 14
    font.family: "Consolas"

    leftPadding: 42
    rightPadding: 12

    selectByMouse: true
    persistentSelection: true

    horizontalAlignment: TextInput.AlignLeft
    verticalAlignment: TextInput.AlignVCenter

    background: Rectangle {
        radius: 8
        color: root.activeFocus ? "#202020" : "#1b1b1b"

        border.width: 1
        border.color: root.activeFocus
            ? "#4e8cff"
            : root.hovered
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
        }

        onClicked: {
            const folder = FileSystemModel.select_folder(FileSystemModel.currentPath)

            if (folder.length > 0) {
                root.text = root.build_path(folder)

                if (updateCurrentPath) {
                    FileSystemModel.currentPath = folder
                    root.focus = false
                }

                root.folderSelected(folder)
            }
        }
    }
}