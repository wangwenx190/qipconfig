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
import QIPConfig

Window {
    id: window
    width: 1000
    height: 800
    visible: true
    //color: Qt.color("transparent")
    title: Application.displayName

    Rectangle {
        id: hostInfoPanel
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 80
        color: Qt.color("transparent")

        RowLayout {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }
            spacing: 5

            Rectangle {
                Layout.fillHeight: true
                width: 5
                color: Qt.color("darkGray")
            }

            ColumnLayout {
                Label {
                    font.pointSize: 12
                    color: Theme.labelColor
                    text: qsTr("Internet Address")
                }

                ClickableLabel {
                    Layout.fillWidth: true
                    font.pointSize: 16
                    color: Theme.labelColor
                    text: NetworkInformation.getInternetIPAddress(NetworkInformation.IPv4)
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: 5
                color: Qt.color("darkGray")
            }

            ColumnLayout {
                Label {
                    font.pointSize: 12
                    color: Theme.labelColor
                    text: qsTr("Local Address")
                }

                ClickableLabel {
                    Layout.fillWidth: true
                    font.pointSize: 16
                    color: Theme.labelColor
                    text: NetworkInformation.getLocalIPAddress(NetworkInformation.IPv4)
                }
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
        }
        clip: true
        cellWidth: 500
        cellHeight: 380
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
            color: Qt.color("transparent")

            Rectangle {
                anchors {
                    fill: parent
                    margins: 10
                }
                color: Qt.rgba(0, 0, 0, 0.5)

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 5

                    Rectangle {
                        Layout.fillWidth: true
                        color: Theme.accentColor
                        height: 35

                        Label {
                            anchors {
                                fill: parent
                                leftMargin: 10
                            }
                            font {
                                bold: true
                                pointSize: Theme.titleFontSize
                            }
                            color: Theme.labelColor
                            text: delegate.name
                            verticalAlignment: Text.AlignVCenter
                        }
                    }

                    Label {
                        Layout.leftMargin: 10
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("Hardware Address")
                    }

                    ClickableLabel {
                        Layout.fillWidth: true
                        Layout.leftMargin: 10
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.hardwareAddress
                    }

                    Label {
                        Layout.leftMargin: 10
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("IPv4 Address")
                    }

                    ClickableLabel {
                        Layout.fillWidth: true
                        Layout.leftMargin: 10
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.ipv4Address
                    }

                    Label {
                        Layout.leftMargin: 10
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("IPv6 Address")
                    }

                    ClickableLabel {
                        Layout.fillWidth: true
                        Layout.leftMargin: 10
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.ipv6Address
                    }

                    Label {
                        Layout.leftMargin: 10
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("Netmask")
                    }

                    ClickableLabel {
                        Layout.fillWidth: true
                        Layout.leftMargin: 10
                        font.pointSize: Theme.contentFontSize
                        color: Theme.labelColor
                        text: delegate.netmask
                    }

                    Label {
                        Layout.leftMargin: 10
                        color: Theme.labelColor
                        font {
                            bold: true
                            pointSize: Theme.titleFontSize
                        }
                        text: qsTr("Broadcast Address")
                    }

                    ClickableLabel {
                        Layout.fillWidth: true
                        Layout.leftMargin: 10
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
            left: parent.left
            right: parent.right
        }
        height: 80

        RowLayout {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }
            spacing: 5

            Rectangle {
                Layout.fillHeight: true
                width: 5
                color: Qt.color("darkGray")
            }

            ColumnLayout {
                Label {
                    font.pointSize: 12
                    color: Theme.labelColor
                    text: qsTr("Network Status")
                }

                Label {
                    Layout.fillWidth: true
                    font.pointSize: 16
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
                width: 5
                color: Qt.color("darkGray")
            }

            ColumnLayout {
                Label {
                    font.pointSize: 12
                    color: Theme.labelColor
                    text: qsTr("Transport Medium")
                }

                Label {
                    Layout.fillWidth: true
                    font.pointSize: 16
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
                width: 5
                color: Qt.color("darkGray")
            }

            ColumnLayout {
                Label {
                    font.pointSize: 12
                    color: Theme.labelColor
                    text: qsTr("Behind Captive Portal")
                }

                Label {
                    Layout.fillWidth: true
                    font.pointSize: 16
                    color: Theme.labelColor
                    text: NetworkInformation.behindCaptivePortal ? qsTr("YES") : qsTr("NO")
                }
            }

            Rectangle {
                Layout.fillHeight: true
                width: 5
                color: Qt.color("darkGray")
            }

            ColumnLayout {
                Label {
                    font.pointSize: 12
                    color: Theme.labelColor
                    text: qsTr("Metered")
                }

                Label {
                    Layout.fillWidth: true
                    font.pointSize: 16
                    color: Theme.labelColor
                    text: NetworkInformation.metered ? qsTr("YES") : qsTr("NO")
                }
            }
        }
    }
}
