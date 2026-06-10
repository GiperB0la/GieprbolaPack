import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../../Styles"

Window {
    id: root

    minimumWidth: 500
    minimumHeight: 180

    visible: false
    modality: Qt.ApplicationModal

    flags: Qt.Window
         | Qt.WindowTitleHint
         | Qt.WindowSystemMenuHint
         | Qt.WindowCloseButtonHint
         | Qt.WindowStaysOnTopHint

    title: "Extract archive"

    property alias extractPath: extractPathField.text

    signal accepted(string extractPath)
    signal rejected()

    function open_dialog() {
        extractPathField.text = FileSystemModel.currentPath

        root.show()
        root.raise()
        root.requestActivate()

        extractPathField.forceActiveFocus()
        extractPathField.selectAll()
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
                    text: "Extract to:"
                    color: "#E0E0E0"
                    font.pixelSize: 14
                }

                FolderPathField {
                    id: extractPathField

                    defaultFileName: ""
                    startFolder: FileSystemModel.currentPath
                    text: FileSystemModel.currentPath
                    placeholderText: "Select folder..."

                    onAccepted: okButton.clicked()
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
                        root.accepted(extractPathField.text)
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