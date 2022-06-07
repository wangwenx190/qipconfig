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
import QIPConfig

AcrylicWindow {
    id: window
    width: Theme.aboutDialogSize.width
    height: Theme.aboutDialogSize.height
    visible: false
    title: qsTr("About")

    property font labelFont: Qt.font({ bold:true, pointSize:Theme.contentFontSize })

    function bringWindowToFront() {
        FramelessHelper.bringWindowToFront();
    }

    FramelessHelper.onReady: {
        FramelessHelper.windowFixedSize = true;
        FramelessHelper.titleBarItem = titleBar;
        FramelessHelper.setSystemButton(titleBar.closeButton, FramelessHelperConstants.Close);
        FramelessHelper.moveWindowToDesktopCenter();
    }

    ColumnLayout {
        anchors {
            top: titleBar.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: Theme.contentsMargin
        }
        spacing: Theme.contentsMargin

        QtLogoButton {
            Layout.alignment: Qt.AlignHCenter
            onClicked: Utils.openUrl("https://www.qt.io/")
        }

        ClickableAppTitle {
            Layout.alignment: Qt.AlignHCenter
            onClicked: Utils.openUrl("https://github.com/wangwenx190/qipconfig/tree/" + DeveloperInformation.appCommitHash)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("Qt version (run-time): %1").arg(DeveloperInformation.qtRunTimeVersion)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("Qt version (compile-time): %1").arg(DeveloperInformation.qtCompileTimeVersion)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("Qt ABI: %1").arg(DeveloperInformation.qtAbiInfo)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("Qt Quick graphics API: %1").arg(DeveloperInformation.graphicsApi)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("Compiler: %1").arg(DeveloperInformation.compilerInfo)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("Compile date and time: %1").arg(DeveloperInformation.compileDateTime)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("Operating system: %1").arg(DeveloperInformation.systemInfo)
        }

        Label {
            font: window.labelFont
            color: Theme.labelColor
            text: qsTr("CPU architecture: %1").arg(DeveloperInformation.cpuArchitecture)
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    StandardTitleBar {
        id: titleBar
        anchors {
            top: window.topBorderBottom
            left: parent.left
            right: parent.right
        }
        hideWhenClose: true
        minimizeButton.visible: false
        maximizeButton.visible: false
    }
}
