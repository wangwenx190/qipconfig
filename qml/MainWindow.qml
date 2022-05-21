/*
 * MIT License
 *
 * Copyright (C) 2022 by wangwenx190 (Yuhang Zhao)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.wangwenx190.FramelessHelper
import org.wangwenx190.QtAcrylicMaterial
import QIPConfig

FramelessWindow {
    id: window
    width: Theme.windowSize.width
    height: Theme.windowSize.height
    visible: false
    color: Theme.windowColor
    title: Application.displayName
    onClosing: Utils.saveGeometry(window)

    FramelessHelper.onReady: {
        FramelessHelper.titleBarItem = titleBar;
        FramelessHelper.setSystemButton(titleBar.minimizeButton, FramelessHelperConstants.Minimize);
        FramelessHelper.setSystemButton(titleBar.maximizeButton, FramelessHelperConstants.Maximize);
        FramelessHelper.setSystemButton(titleBar.closeButton, FramelessHelperConstants.Close);
        if (!Utils.restoreGeometry(window)) {
            FramelessHelper.moveWindowToDesktopCenter();
        }
        window.visible = true;
    }

    DesktopWallpaper {
        id: wallpaper
        anchors {
            top: window.topBorderBottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        visible: false
    }

    AcrylicMaterial {
        id: acrylic
        anchors {
            top: window.topBorderBottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        source: wallpaper
        theme: {
            switch (Theme.preferredTheme) {
            case Theme.Light:
                return AcrylicMaterial.Light;
            case Theme.Dark:
                return AcrylicMaterial.Dark;
            case Theme.System:
                return AcrylicMaterial.System;
            }
        }
    }

    Rectangle {
        id: hostInfoPanel
        anchors {
            top: titleBar.bottom
            left: parent.left
            leftMargin: Theme.contentsMargin
            right: parent.right
            rightMargin: Theme.contentsMargin
        }
        height: 80
        color: window.color

        RowLayout {
            anchors.fill: parent
            spacing: Theme.contentsMargin

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Internet Address")
                }

                ClickableLabel {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: NetworkInformation.internetAddress
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Local Address")
                }

                ClickableLabel {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: NetworkInformation.localAddress
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Local Host")
                }

                ClickableLabel {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: NetworkInformation.localHostName
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Local Domain")
                }

                ClickableLabel {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: NetworkInformation.localDomainName
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    GridView {
        id: networkAdapterGrid
        anchors {
            top: hostInfoPanel.bottom
            bottom: networkIndicatorPanel.top
            left: parent.left
            right: parent.right
            margins: Theme.contentsMargin
        }
        clip: true
        cellWidth: Theme.infoPanelSize.width
        cellHeight: Theme.infoPanelSize.height
        model: networkAdapterModel
        delegate: Rectangle {
            required property string name
            required property string hardwareAddress
            required property string ipv4Address
            required property string ipv6Address
            required property string netmask
            required property string broadcastAddress

            id: delegate
            width: networkAdapterGrid.cellWidth
            height: networkAdapterGrid.cellHeight
            color: window.color

            Rectangle {
                anchors {
                    fill: parent
                    margins: Theme.contentsMargin
                }
                color: Theme.infoPanelColor

                ColumnLayout {
                    anchors.fill: parent
                    spacing: Theme.contentsMargin

                    Rectangle {
                        Layout.fillWidth: true
                        color: Theme.accentColor
                        height: 35

                        Label {
                            anchors {
                                fill: parent
                                leftMargin: Theme.contentsMargin
                            }
                            font {
                                bold: true
                                pointSize: Theme.titleFontSize
                            }
                            color: Qt.color("white")
                            text: delegate.name
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Label {
                        Layout.leftMargin: Theme.contentsMargin
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("Hardware Address")
                    }

                    ClickableLabel {
                        Layout.leftMargin: Theme.contentsMargin
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.hardwareAddress
                    }

                    Label {
                        Layout.leftMargin: Theme.contentsMargin
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("IPv4 Address")
                    }

                    ClickableLabel {
                        Layout.leftMargin: Theme.contentsMargin
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.ipv4Address
                    }

                    Label {
                        Layout.leftMargin: Theme.contentsMargin
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("IPv6 Address")
                    }

                    ClickableLabel {
                        Layout.leftMargin: Theme.contentsMargin
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.ipv6Address
                    }

                    Label {
                        Layout.leftMargin: Theme.contentsMargin
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("Netmask")
                    }

                    ClickableLabel {
                        Layout.leftMargin: Theme.contentsMargin
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.netmask
                    }

                    Label {
                        Layout.leftMargin: Theme.contentsMargin
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("Broadcast Address")
                    }

                    ClickableLabel {
                        Layout.leftMargin: Theme.contentsMargin
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.broadcastAddress
                    }

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }
        Component.onCompleted: networkAdapterModel.populate()
    }

    Rectangle {
        id: networkIndicatorPanel
        anchors {
            bottom: parent.bottom
            bottomMargin: Theme.contentsMargin
            left: parent.left
            leftMargin: Theme.contentsMargin
            right: parent.right
            rightMargin: Theme.contentsMargin
        }
        height: 80
        color: window.color

        RowLayout {
            anchors.fill: parent
            spacing: Theme.contentsMargin

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Network Status")
                }

                Label {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: {
                        switch (NetworkInformation.networkStatus) {
                        case NetworkInformation.Unknown:
                            return qsTr("UNKNOWN");
                        case NetworkInformation.Online:
                            return qsTr("ONLINE");
                        case NetworkInformation.Offline:
                            return qsTr("OFFLINE");
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Transport Medium")
                }

                Label {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: {
                        switch (NetworkInformation.transportMedium) {
                        case NetworkInformation.Unknown:
                            return qsTr("UNKNOWN");
                        case NetworkInformation.Ethernet:
                            return qsTr("ETHERNET");
                        case NetworkInformation.Cellular:
                            return qsTr("CELLULAR");
                        case NetworkInformation.WiFi:
                            return qsTr("WIFI");
                        case NetworkInformation.Bluetooth:
                            return qsTr("BLUETOOTH");
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Behind Captive Portal")
                }

                Label {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: NetworkInformation.behindCaptivePortal ? qsTr("YES") : qsTr("NO")
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: Theme.separatorSize
                color: Theme.separatorColor
            }

            ColumnLayout {
                Layout.fillHeight: true

                Label {
                    font.pointSize: Theme.largeTitleFontSize
                    color: Theme.labelColor
                    text: qsTr("Metered")
                }

                Label {
                    font {
                        pointSize: Theme.largeContentFontSize
                        bold: true
                    }
                    color: Theme.labelColor
                    text: NetworkInformation.metered ? qsTr("YES") : qsTr("NO")
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RefreshButton {
                onClicked: networkAdapterModel.populate()
            }
        }
    }

    StandardTitleBar {
        id: titleBar
        anchors {
            top: window.topBorderBottom
            left: parent.left
            right: parent.right
        }
        titleLabel.visible: false
        useAlternativeBackground: true
        color: window.color
    }
}
