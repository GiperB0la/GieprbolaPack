import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "#181818"

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Label {
            text: "Folders: " + FileSystemModel.folderCount +
                  " | Files: " + FileSystemModel.fileCount +
                  " | Size: " + FileSystemModel.totalSizeText +
                  " | Selected folders: " + FileSystemModel.selectedFolderCount +
                  " | Selected files: " + FileSystemModel.selectedFileCount

            color: "#cccccc"
        }

        Item {
            Layout.fillWidth: true
        }

        Label {
            text: FileSystemModel.statusText
            color: "#cccccc"
        }
    }
}