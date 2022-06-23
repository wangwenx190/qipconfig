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
import QtQuick.Controls.Basic
import QIPConfig

Label {
    id: label

    QtObject {
        id: data

        property bool alternative: false
        readonly property real fontSizeDiff: 2
        readonly property string defaultToolTipText: qsTr("Click to copy it's text to the clipboard.")
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            if (label.font.bold) {
                data.alternative = true;
                label.font.pointSize += data.fontSizeDiff;
            } else {
                label.font.bold = true;
            }
        }
        onExited: {
            if (data.alternative) {
                data.alternative = false;
                label.font.pointSize -= data.fontSizeDiff;
            } else {
                label.font.bold = false;
            }
            toolTip.text = data.defaultToolTipText;
        }
        onClicked: {
            Utils.copyToClipboard(label.text);
            toolTip.text = qsTr("\"%1\" has been copied to the clipboard!").arg(label.text);
        }
    }

    ToolTip {
        id: toolTip
        delay: 0
        text: data.defaultToolTipText
        visible: mouseArea.containsMouse
    }
}
