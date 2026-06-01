import QtQuick
import QtQuick.Layouts

Rectangle {
    color: "#232323"

    border.color: "#323232"
    border.width: 1

    RowLayout {
        anchors.fill: parent

        anchors.leftMargin: 8
        anchors.rightMargin: 8

        spacing: 4

        TopMenuButton {
            text: "File"
        }

        TopMenuButton {
            text: "Settings"
        }

        TopMenuButton {
            text: "Reference"
        }

        Item {
            Layout.fillWidth: true
        }
    }
}