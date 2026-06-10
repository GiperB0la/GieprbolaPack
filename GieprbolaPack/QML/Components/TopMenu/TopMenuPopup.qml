import QtQuick
import QtQuick.Controls

Popup {
    id: root

    default property alias content: contentColumn.data

    modal: false
    focus: false
    closePolicy: Popup.CloseOnPressOutside

    width: 180
    padding: 6

    background: Rectangle {
        radius: 8
        color: "#2A2A2A"

        border.width: 1
        border.color: "#404040"
    }

    Column {
        id: contentColumn

        width: parent.width
        spacing: 2
    }
}