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
import QIPConfig

Label {
    font {
        bold: true
        pointSize: Theme.contentFontSize
        italic: mouseArea.containsMouse
    }
    color: mouseArea.containsMouse ? Qt.lighter(Theme.accentColor, 1.5) : Theme.accentColor
    text: qsTr("%1 version %2 (%3)").arg(Application.name)
                                    .arg(Application.version)
                                    .arg(DeveloperInformation.appCommitHashShort)

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: cursorShape = Qt.PointingHandCursor
        onExited: cursorShape = Qt.ArrowCursor
        onClicked: Utils.openUrl("https://github.com/wangwenx190/qipconfig/tree/" + DeveloperInformation.appCommitHash)
    }

    ToolTip {
        delay: 0
        visible: mouseArea.containsMouse
        text: qsTr("Click to browse the repository at this point in the history.")
    }
}
