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
