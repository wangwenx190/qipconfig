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
import org.wangwenx190.FramelessHelper
import org.wangwenx190.QtAcrylicMaterial
import QIPConfig

FramelessWindow {
    id: window
    color: Theme.windowColor

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
}
