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
            text:
                qsTr("%1 folders • %2 files • %3   |   Selected: %4 folders, %5 files")
                    .arg(FileSystemModel.folderCount)
                    .arg(FileSystemModel.fileCount)
                    .arg(FileSystemModel.totalSizeText)
                    .arg(FileSystemModel.selectedFolderCount)
                    .arg(FileSystemModel.selectedFileCount)

            color: "#bdbdbd"
            font.pixelSize: 13
        }

        Item {
            Layout.fillWidth: true
        }

        Label {
            text: FileSystemModel.statusText
            color: "#bdbdbd"
            font.pixelSize: 13
        }
    }
}