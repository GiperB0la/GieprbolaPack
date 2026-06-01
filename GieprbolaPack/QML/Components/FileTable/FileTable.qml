import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "#1e1e1e"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // =========================
        // TABLE HEADER
        // =========================
        Rectangle {
            Layout.fillWidth: true
            height: 42

            color: "#252526"

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                height: 1
                color: "#3a3a3a"
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                spacing: 0

                Item {
                    Layout.preferredWidth: 42
                    Layout.fillHeight: true
                }

                HeaderCell {
                    text: "Name"
                    Layout.preferredWidth: 400
                }

                HeaderCell {
                    text: "Type"
                    Layout.preferredWidth: 120
                }

                HeaderCell {
                    text: "Size"
                    Layout.preferredWidth: 120
                }

                HeaderCell {
                    text: "Changed"
                    Layout.fillWidth: true
                }
            }
        }

        // =========================
        // TABLE BODY
        // =========================
        ListView {
            id: filesView

            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            model: FileSystemModel

            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                id: row

                width: filesView.width
                height: 38

                color: fileChecked
                    ? "#33465a"
                    : mouseArea.containsMouse
                        ? "#303030"
                        : index % 2 === 0
                            ? "#232323"
                            : "#282828"

                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    height: 1
                    color: "#303030"
                    opacity: 0.7
                }

                MouseArea {
                    id: mouseArea

                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        FileSystemModel.toggle_checked(index)
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 12
                    anchors.rightMargin: 12
                    spacing: 0

                    CheckBox {
                        id: checkBox

                        checked: fileChecked

                        Layout.preferredWidth: 42
                        Layout.preferredHeight: 38

                        padding: 0
                        spacing: 0

                        indicator: Rectangle {
                            width: 18
                            height: 18
                            radius: 4

                            x: 12
                            y: checkBox.height / 2 - height / 2

                            color: checkBox.checked
                                ? "#4ea1ff"
                                : "#1b1b1b"

                            border.width: 1
                            border.color: checkBox.checked
                                ? "#4ea1ff"
                                : checkBox.hovered
                                    ? "#b0b0b0"
                                    : "#666666"

                            Text {
                                anchors.centerIn: parent

                                text: "✓"
                                visible: checkBox.checked

                                color: "#ffffff"
                                font.pixelSize: 13
                                font.bold: true
                            }
                        }

                        background: Item {}

                        onClicked: {
                            FileSystemModel.set_checked(index, checked)
                        }
                    }

                    TableCell {
                        text: fileName
                        color: "#f0f0f0"
                        Layout.preferredWidth: 400
                    }

                    TableCell {
                        text: fileType
                        color: "#c8c8c8"
                        Layout.preferredWidth: 120
                    }

                    TableCell {
                        text: fileSize
                        color: "#c8c8c8"
                        Layout.preferredWidth: 120
                    }

                    TableCell {
                        text: fileDate
                        color: "#c8c8c8"
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }

    // =========================
    // HEADER CELL COMPONENT
    // =========================
    component HeaderCell: Rectangle {
        property alias text: label.text

        Layout.fillHeight: true

        color: "transparent"

        Rectangle {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            width: 1
            color: "#3a3a3a"
            opacity: 0.9
        }

        Label {
            id: label

            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12

            verticalAlignment: Text.AlignVCenter

            color: "#dcdcdc"
            font.pixelSize: 13
            font.bold: true

            elide: Text.ElideRight
        }
    }

    // =========================
    // BODY CELL COMPONENT
    // =========================
    component TableCell: Label {
        verticalAlignment: Text.AlignVCenter

        leftPadding: 12
        rightPadding: 12

        font.pixelSize: 13
        elide: Text.ElideRight
    }
}