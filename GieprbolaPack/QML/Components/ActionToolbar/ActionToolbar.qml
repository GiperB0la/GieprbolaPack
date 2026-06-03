import QtQuick
import QtQuick.Layouts
import "../../Resources"
import "../../Styles"

Rectangle {
    color: "#232323"

    border.color: "#323232"
    border.width: 1

    ArchiveDialog {
        id: archiveDialog

        onAccepted: function(archiveName, archiveType, compressionMode) {
            FileSystemModel.create_archive(archiveName, archiveType, compressionMode)
        }

        onRejected: {
            console.log("Create archive canceled")
        }
    }

    ConfirmDialog {
        id: deleteDialog

        message: "Delete selected files?"

        onAccepted: {
            FileSystemModel.delete_selected()
        }

        onRejected: {
            console.log("Delete canceled")
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 32
        anchors.rightMargin: 8
        spacing: 16

        ActionToolbarButton {
            text: "Add"
            imageSource: "qrc:/qt/qml/gieprbolapack/Resources/Add.png"
            tooltipText: "Add files to archive"

            onClicked: {
                archiveDialog.open_dialog()
            }
        }

        ActionToolbarButton {
            text: "Extract"
            imageSource: "qrc:/qt/qml/gieprbolapack/Resources/Extract.png"
            tooltipText: "Extract files from archive"

            onClicked: {
                console.log("Extract")
            }
        }

        ActionToolbarButton {
            text: "Delete"
            imageSource: "qrc:/qt/qml/gieprbolapack/Resources/Delete.png"
            tooltipText: "Delete Files"

            onClicked: {
                deleteDialog.open_dialog()
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}