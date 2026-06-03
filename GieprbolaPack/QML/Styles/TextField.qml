import QtQuick
import QtQuick.Controls

TextField {
    id: root

    height: 40

    color: "#f1f1f1"
    placeholderTextColor: "#777777"
    font.pixelSize: 14

    leftPadding: 12
    rightPadding: 12

    selectByMouse: true

    background: Rectangle {
        radius: 8
        color: "#1b1b1b"

        border.width: 1
        border.color: root.activeFocus
            ? "#4e8cff"
            : "#383838"
    }
}