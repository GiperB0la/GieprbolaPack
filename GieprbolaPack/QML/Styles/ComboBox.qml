import QtQuick
import QtQuick.Controls

ComboBox {
    id: root

    width: 220
    font.pixelSize: 12

    background: Rectangle {
        radius: 6

        color: root.pressed
            ? "#2a2a2a"
            : root.hovered
                ? "#282828"
                : "#232323"

        border.width: 1
        border.color: root.activeFocus
            ? "#4e8cff"
            : root.hovered
                ? "#505050"
                : "#404040"
    }

    contentItem: Text {
        text: root.displayText

        color: root.enabled ? "#f0f0f0" : "#777777"
        font: root.font

        verticalAlignment: Text.AlignVCenter

        leftPadding: 14
        rightPadding: 40

        elide: Text.ElideRight
    }

    indicator: Text {
        text: "▼"

        color: root.enabled ? "#cfcfcf" : "#777777"
        font.pixelSize: 12

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 12

        rotation: root.popup.visible ? 180 : 0

        Behavior on rotation {
            NumberAnimation {
                duration: 120
                easing.type: Easing.OutCubic
            }
        }
    }

    popup: Popup {
        y: root.height + 4
        width: root.width

        padding: 4

        background: Rectangle {
            radius: 6
            color: "#202020"

            border.width: 1
            border.color: "#454545"
        }

        contentItem: ListView {
            clip: true

            implicitHeight: contentHeight

            model: root.delegateModel
            currentIndex: root.highlightedIndex
        }
    }

    delegate: ItemDelegate {
        width: root.width - 8
        height: 28

        highlighted: root.highlightedIndex === index

        background: Rectangle {
            radius: 4

            color: highlighted
                ? "#33465f"
                : hovered
                    ? "#2b2b2b"
                    : "transparent"
        }

        contentItem: Text {
            text: modelData

            color: "#f0f0f0"
            font.pixelSize: 12

            verticalAlignment: Text.AlignVCenter
            leftPadding: 10
            rightPadding: 10

            elide: Text.ElideRight
        }
    }
}