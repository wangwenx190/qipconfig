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

import QtQml
import QtQuick
import QtQuick.Controls.Basic
import QIPConfig

Button {
    required property string iconCode
    required property string toolTip
    property bool isClose: false

    QtObject {
        property color backgroundColor: button.isClose ? Qt.color("red") : Theme.accentColor
        id: privateProps
    }

    id: button
    implicitHeight: Theme.titleBarHeight
    implicitWidth: implicitHeight * 1.5
    contentItem: Label {
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font {
            family: Theme.iconFontName
            pointSize: 7
        }
        color: button.hovered ? Qt.color("white") : Qt.color("black")
        text: button.iconCode
    }
    background: Rectangle {
        visible: button.hovered || button.down
        color: button.down ? Qt.lighter(privateProps.backgroundColor, 1.5) : Qt.lighter(privateProps.backgroundColor, 1.2)
    }

    ToolTip {
        delay: 0
        text: button.toolTip
        visible: button.hovered
    }
}
