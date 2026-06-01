import QtQuick
import QtQuick.Layouts
import "Components"
import "Components/TopMenu"
import "Components/ActionToolbar"
import "Components/PathBar"
import "Components/FileTable"
import "Components/BottomStatusBar"

Rectangle {
    color: "#1C1C1C"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopMenu {
            Layout.fillWidth: true
            Layout.preferredHeight: 46
        }

        ActionToolbar {
            Layout.fillWidth: true
            Layout.preferredHeight: 84
        }

        PathBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
        }

        FileTable {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        BottomStatusBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
        }
    }
}