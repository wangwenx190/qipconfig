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

#include "theme.h"
#include <QtCore/qdebug.h>
#include <QtCore/qt_windows.h>
#include <QtCore/private/qsystemlibrary_p.h>
#include <dwmapi.h>

QColor Theme::accentColor() const
{
    static const auto pDwmGetColorizationColor =
        reinterpret_cast<decltype(&DwmGetColorizationColor)>(
            QSystemLibrary::resolve(u"dwmapi"_qs, "DwmGetColorizationColor"));
    if (!pDwmGetColorizationColor) {
        qWarning() << "Failed to resolve DwmGetColorizationColor() from DWMAPI.DLL.";
        return QColorConstants::DarkGray;
    }
    DWORD color = 0;
    BOOL opaque = FALSE;
    const HRESULT hr = pDwmGetColorizationColor(&color, &opaque);
    if (FAILED(hr)) {
        qDebug() << "DwmGetColorizationColor() failed with error code" << HRESULT_CODE(hr);
        return QColorConstants::DarkGray;
    }
    return QColor::fromRgba(color);
}
