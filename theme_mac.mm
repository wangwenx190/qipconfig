#include "theme.h"
#include <AppKit/AppKit.h>

QT_BEGIN_NAMESPACE
[[nodiscard]] Q_GUI_EXPORT QColor qt_mac_toQColor(const NSColor *color);
QT_END_NAMESPACE

QColor Theme::accentColor() const
{
    return qt_mac_toQColor([NSColor controlAccentColor]);
}
