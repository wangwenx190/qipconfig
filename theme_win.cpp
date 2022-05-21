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
#include "themehelper.h"
#include <QtCore/qdebug.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qabstractnativeeventfilter.h>
#include <QtCore/private/qsystemlibrary_p.h>
#include <QtCore/qt_windows.h>
#include <dwmapi.h>

class ThemeWin32EventFilter : public QAbstractNativeEventFilter
{
    Q_DISABLE_COPY_MOVE(ThemeWin32EventFilter)

public:
    explicit ThemeWin32EventFilter() : QAbstractNativeEventFilter() {}
    ~ThemeWin32EventFilter() override = default;

    [[nodiscard]] bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override
    {
        if ((eventType != "windows_generic_MSG"_qba) || !message || !result) {
            return false;
        }
        const auto msg = static_cast<LPMSG>(message);
        if (!msg->hwnd) {
            return false;
        }
        if ((msg->message == WM_SETTINGCHANGE) && (msg->lParam != 0) &&
               (std::wcscmp(reinterpret_cast<LPCWSTR>(msg->lParam), L"ImmersiveColorSet") == 0)) {
            qDebug() << "Detected system theme change event.";
            Q_EMIT ThemeHelper::instance()->systemThemeChanged();
        }
        return false;
    }
};

Q_GLOBAL_STATIC(QScopedPointer<ThemeWin32EventFilter>, themeWin32EventFilter)

void setupWin32ThemeChangeNotifier()
{
    if (!themeWin32EventFilter()->isNull()) {
        return;
    }
    themeWin32EventFilter()->reset(new ThemeWin32EventFilter);
    qApp->installNativeEventFilter(themeWin32EventFilter()->get());
}

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
        qDebug() << "DwmGetColorizationColor() failed.";
        return QColorConstants::DarkGray;
    }
    return QColor::fromRgba(color);
}
