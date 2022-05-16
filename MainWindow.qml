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
    width: 640
    height: 480
    visible: true
    title: Application.displayName

    GridView {
        id: grid
        anchors.fill: parent
        clip: true
        cellWidth: 400
        cellHeight: 300
        model: networkAdapterModel
        delegate: Rectangle {
            required property string name
            required property string description
            required property string hardwareAddress
            required property string ipv4Address
            required property string ipv6Address
            required property string netmask
            required property string broadcast

            id: delegate
            width: grid.cellWidth
            height: grid.cellHeight
            color: Qt.color("gray")

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 0

                Rectangle {
                    Layout.fillWidth: true
                    color: Qt.color("blue")
                    height: 20

                    Label {
                        anchors.fill: parent
                        color: Qt.color("white")
                        text: delegate.name
                    }
                }

                Label {
                    color: Qt.color("white")
                    font.bold: true
                    text: qsTr("Hardware Address")
                }

                Label {
                    Layout.fillWidth: true
                    color: Qt.color("white")
                    text: delegate.hardwareAddress
                }

                Label {
                    color: Qt.color("white")
                    font.bold: true
                    text: qsTr("IPv4 Address")
                }

                Label {
                    Layout.fillWidth: true
                    color: Qt.color("white")
                    text: delegate.ipv4Address
                }

                Label {
                    color: Qt.color("white")
                    font.bold: true
                    text: qsTr("IPv6 Address")
                }

                Label {
                    Layout.fillWidth: true
                    color: Qt.color("white")
                    text: delegate.ipv6Address
                }

                Label {
                    color: Qt.color("white")
                    font.bold: true
                    text: qsTr("Netmask")
                }

                Label {
                    Layout.fillWidth: true
                    color: Qt.color("white")
                    text: delegate.netmask
                }

                Label {
                    color: Qt.color("white")
                    font.bold: true
                    text: qsTr("Broadcast Address")
                }

                Label {
                    Layout.fillWidth: true
                    color: Qt.color("white")
                    text: delegate.broadcast
                }
            }
        }
    }
}
