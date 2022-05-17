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
    height: 700
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
                left: parent.left
            }
            //width: 500
            spacing: 5

            Rectangle {
                width: 5
                height: parent.height
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
                    text: NetworkInformation.internetIPv4Address
                }
            }

            Rectangle {
                width: 5
                height: parent.height
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
                    text: NetworkInformation.localIPv4Address
                }
            }
        }
    }

    GridView {
        id: grid
        anchors {
            top: hostInfoPanel.bottom
            bottom: parent.bottom
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
            width: grid.cellWidth
            height: grid.cellHeight
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
                            font.pointSize: Theme.titleFontSize
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
    }
}
