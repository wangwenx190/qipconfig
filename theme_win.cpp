#include "theme.h"
#include <QtCore/qt_windows.h>
#include <dwmapi.h>

QColor Theme::accentColor() const
{
    DWORD color = 0;
    BOOL opaque = FALSE;
    const HRESULT hr = DwmGetColorizationColor(&color, &opaque);
    if (FAILED(hr)) {
        return QColorConstants::DarkGray;
    }
    return QColor::fromRgba(color);
}
