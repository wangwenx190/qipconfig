import QtQuick
import QtQuick.Controls
import QIPConfig

Button {
    id: button

    contentItem: Item {
        implicitWidth: 50
        implicitHeight: 50

        Image {
            anchors.fill: parent
            smooth: true
            mipmap: true
            source: Theme.theme === Theme.Dark ? "qrc:///images/refresh_dark.svg" : "qrc:///images/refresh_light.svg"
        }
    }

    background: Rectangle {
        color: Theme.infoPanelColor
        visible: button.hovered
    }

    ToolTip {
        delay: 0
        text: qsTr("Click to re-detect the network configuration.")
        visible: button.hovered
    }
}
