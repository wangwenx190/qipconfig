#include "theme.h"
#include <QtGui/qpa/qplatformtheme.h>
#include <QtGui/private/qguiapplication_p.h>

Theme::Theme(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Type>();
    refresh();
}

Theme::~Theme() = default;

Theme::Type Theme::theme() const
{
    return (m_dark ? Type::Dark : Type::Light);
}

QColor Theme::labelColor() const
{
    return (m_dark ? QColorConstants::White : QColorConstants::Black);
}

qreal Theme::titleFontSize() const
{
    return 13;
}

qreal Theme::contentFontSize() const
{
    return 11;
}

void Theme::refresh()
{
    const bool dark = []() -> bool {
        if (const QPlatformTheme * const theme = QGuiApplicationPrivate::platformTheme()) {
            return (theme->appearance() == QPlatformTheme::Appearance::Dark);
        }
        return false;
    }();
    if (m_dark == dark) {
        return;
    }
    m_dark = dark;
    Q_EMIT themeChanged();
}
