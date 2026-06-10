import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root

    property Popup currentMenu: null

    function toggle_menu(menu, button) {
        if (currentMenu && currentMenu !== menu)
            currentMenu.close()

        if (menu.opened) {
            menu.close()
            currentMenu = null
            return
        }

        menu.x = button.x + 8
        menu.y = button.y + button.height + 2
        menu.open()

        currentMenu = menu
    }

    color: "#232323"

    border.color: "#323232"
    border.width: 1

    RowLayout {
        id: menuRow

        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8

        spacing: 4

        TopMenuButton {
            id: fileButton
            text: "File"

            onClicked: root.toggle_menu(fileMenu, fileButton)
        }

        TopMenuButton {
            id: settingsButton
            text: "Settings"

            onClicked: root.toggle_menu(settingsMenu, settingsButton)
        }

        TopMenuButton {
            id: referenceButton
            text: "Reference"

            onClicked: root.toggle_menu(referenceMenu, referenceButton)
        }

        Item {
            Layout.fillWidth: true
        }
    }

    TopMenuPopup {
        id: fileMenu

        TopMenuPopupButton {
            text: "Open file"
            onClicked: fileMenu.close()
        }

        TopMenuPopupButton {
            text: "Save"
            onClicked: fileMenu.close()
        }

        TopMenuPopupButton {
            text: "Save as..."
            onClicked: fileMenu.close()
        }

        Rectangle {
            width: parent.width
            height: 1
            color: "#404040"
        }

        TopMenuPopupButton {
            text: "Exit"

            onClicked: {
                fileMenu.close()
                Qt.quit()
            }
        }
    }

    TopMenuPopup {
        id: settingsMenu

        TopMenuPopupButton {
            text: "Preferences"
            onClicked: settingsMenu.close()
        }

        TopMenuPopupButton {
            text: "Theme"
            onClicked: settingsMenu.close()
        }
    }

    TopMenuPopup {
        id: referenceMenu

        TopMenuPopupButton {
            text: "Help"
            onClicked: referenceMenu.close()
        }

        TopMenuPopupButton {
            text: "About"
            onClicked: referenceMenu.close()
        }
    }
}