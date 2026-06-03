import QtQuick
import QtQuick.Layouts

Window {
    id: root

    width: 320
    height: 140
    minimumWidth: 320
    minimumHeight: 140

    visible: false
    modality: Qt.ApplicationModal

    flags: Qt.Window
         | Qt.WindowTitleHint
         | Qt.WindowSystemMenuHint
         | Qt.WindowCloseButtonHint
         | Qt.WindowStaysOnTopHint

    title: "Confirm"

    property string message: "Are you sure?"

    signal accepted()
    signal rejected()

    function open_dialog() {
        root.show()
        root.raise()
        root.requestActivate()
    }

    component ConfirmButton: Rectangle {
        id: buttonRoot

        property alias text: label.text

        property color normalColor: "#2B2B2B"
        property color hoverColor: "#343434"
        property color pressedColor: "#404040"

        signal clicked()

        width: 90
        height: 32
        radius: 5

        color: mouse.pressed ? pressedColor
             : mouse.containsMouse ? hoverColor
             : normalColor

        border.width: 1
        border.color: mouse.pressed ? "#707070"
                     : mouse.containsMouse ? "#5A5A5A"
                     : "#454545"

        Text {
            id: label

            anchors.centerIn: parent

            color: "#EAEAEA"
            font.pixelSize: 14
            font.bold: true
        }

        MouseArea {
            id: mouse

            anchors.fill: parent

            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor

            onClicked: buttonRoot.clicked()
        }

        Behavior on color {
            ColorAnimation {
                duration: 80
            }
        }

        Behavior on border.color {
            ColorAnimation {
                duration: 80
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "#232323"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 14

            Text {
                text: root.message

                Layout.fillWidth: true

                color: "#E0E0E0"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
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

                ConfirmButton {
                    text: "Delete"

                    normalColor: "#8A2D2D"
                    hoverColor: "#A33A3A"
                    pressedColor: "#6F2424"

                    onClicked: {
                        root.accepted()
                        root.close()
                    }
                }

                ConfirmButton {
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