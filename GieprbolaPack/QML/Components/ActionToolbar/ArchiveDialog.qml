import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../../Styles"

Window {
    id: root

    minimumWidth: 400
    minimumHeight: 200

    visible: false
    modality: Qt.ApplicationModal

    flags: Qt.Window
         | Qt.WindowTitleHint
         | Qt.WindowSystemMenuHint
         | Qt.WindowCloseButtonHint
         | Qt.WindowStaysOnTopHint

    title: "Create archive"

    property alias archiveName: archiveNameField.text

    signal accepted(string archiveName, int archiveType, int compressionMode)
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

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "Archive Name:"
                    color: "#E0E0E0"
                    font.pixelSize: 14
                }

                TextField {
                    id: archiveNameField

                    Layout.fillWidth: true

                    text: "archive.zip"
                    placeholderText: "archive.zip"

                    onAccepted: okButton.clicked()
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: "Archive Type:"
                            color: "#E0E0E0"
                            font.pixelSize: 14
                        }

                        ComboBox {
                            id: archiveTypeBox

                            Layout.fillWidth: true
                            Layout.preferredHeight: 28

                            model: [
                                "ZIP",
                                "RAR"
                            ]
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: "Compression Method:"
                            color: "#E0E0E0"
                            font.pixelSize: 14
                        }

                        ComboBox {
                            id: compressionMethodBox

                            Layout.fillWidth: true
                            Layout.preferredHeight: 28

                            model: [
                                "Store",
                                "Deflate"
                            ]
                        }
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 8

                Item {
                    Layout.fillWidth: true
                }

                ArchiveDialogButton {
                    id: okButton

                    text: "OK"

                    normalColor: "#1F4E79"
                    hoverColor: "#29608F"
                    pressedColor: "#184364"

                    onClicked: {
                        root.accepted(
                            archiveNameField.text,
                            archiveTypeBox.currentIndex,
                            compressionMethodBox.currentIndex
                        )

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