import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../../Styles"

Window {
    id: root

    minimumWidth: 500
    minimumHeight: 300

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

                FolderPathField {
                    id: archiveNameField

                    defaultFileName: "archive.zip"
                    startFolder: FileSystemModel.currentPath
                    text: FileSystemModel.currentPath + "/archive.zip"
                    placeholderText: "archive.zip"

                    onAccepted: okButton.clicked()
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40

                    color: "#2B2B2B"
                    radius: 6
                    border.color: "#3A3A3A"
                    border.width: 1

                    GridLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12

                        columns: 3
                        columnSpacing: 0

                        Text {
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter

                            text: "Folders: " + FileSystemModel.selectedFolderCount
                            color: "#E0E0E0"
                            font.pixelSize: 14
                        }

                        Text {
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter

                            text: "Files: " + FileSystemModel.selectedFileCount
                            color: "#E0E0E0"
                            font.pixelSize: 14
                        }

                        Text {
                            Layout.fillWidth: true
                            horizontalAlignment: Text.AlignHCenter

                            text: "Size: " + FileSystemModel.selectedSizeText
                            color: "#E0E0E0"
                            font.pixelSize: 14
                        }
                    }
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
                                "Deflate",
                                "Store"
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

                ActionDialogButton {
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

                ActionDialogButton {
                    text: "Cancel"

                    onClicked: {
                        root.rejected()
                        root.close()
                    }
                }
            }
        }
    }
}