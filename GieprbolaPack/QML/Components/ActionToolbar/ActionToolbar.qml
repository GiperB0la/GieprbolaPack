import QtQuick
import QtQuick.Layouts
import "../../Resources"

Rectangle {
    color: "#232323"

    border.color: "#323232"
    border.width: 1

    ArchiveDialog {
        id: archiveDialog

        onAccepted: function(archiveName) {
            FileSystemModel.create_archive(archiveName)
        }

        onRejected: {
            console.log("Create archive canceled")
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

        Item {
            Layout.fillWidth: true
        }
    }
}