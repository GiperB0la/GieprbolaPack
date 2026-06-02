import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: root

    width: 360
    height: 160
    minimumWidth: 320
    minimumHeight: 150

    visible: false
    modality: Qt.ApplicationModal
    flags: Qt.Window | Qt.WindowTitleHint | Qt.WindowSystemMenuHint | Qt.WindowCloseButtonHint | Qt.WindowStaysOnTopHint

    title: "Create archive"

    property alias archiveName: archiveNameField.text

    signal accepted(string archiveName)
    signal rejected()

    function open_dialog() {
        archiveNameField.text = "archive.zip"
        root.show()
        root.raise()
        root.requestActivate()
        archiveNameField.forceActiveFocus()
        archiveNameField.selectAll()
    }

    Rectangle {
        anchors.fill: parent
        color: "#232323"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            Text {
                text: "Create archive"
                color: "#E0E0E0"
                font.pixelSize: 18
                font.bold: true
            }

            TextField {
                id: archiveNameField

                Layout.fillWidth: true
                text: "archive.zip"
                placeholderText: "Archive name"
                selectByMouse: true

                onAccepted: okButton.clicked()
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Item {
                    Layout.fillWidth: true
                }

                ArchiveDialogButton {
                    text: "OK"

                    normalColor: "#1F4E79"
                    hoverColor: "#29608F"
                    pressedColor: "#184364"

                    onClicked: {
                        root.accepted(archiveNameField.text)
                        root.close()
                    }
                }

                ArchiveDialogButton {
                    text: "Cancel"

                    onClicked: {
                        root.rejected()
                        root.close()
                    }
                }
            }
        }
    }

    onClosing: function(close) {
        root.rejected()
    }
}