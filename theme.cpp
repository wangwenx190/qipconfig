#include "theme.h"
#include "themehelper.h"
#include <QtGui/qpa/qplatformtheme.h>
#include <QtGui/private/qguiapplication_p.h>

#ifdef Q_OS_WINDOWS
extern void setupWin32ThemeChangeNotifier();
#endif

Theme::Theme(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<ThemeType>();
    const ThemeHelper * const themeHelper = ThemeHelper::instance();
    connect(themeHelper, &ThemeHelper::preferredThemeChanged, this, &Theme::preferredThemeChanged);
    if (preferredTheme() == ThemeType::System) {
#ifdef Q_OS_WINDOWS
        setupWin32ThemeChangeNotifier();
#endif
        connect(themeHelper, &ThemeHelper::systemThemeChanged, this, &Theme::refresh);
    }
    refresh();
}

Theme::~Theme() = default;

Theme::ThemeType Theme::theme() const
{
    return (m_dark ? ThemeType::Dark : ThemeType::Light);
}

Theme::ThemeType Theme::preferredTheme() const
{
    switch (ThemeHelper::instance()->getPreferredTheme()) {
    case ThemeHelper::Theme::Light:
        return ThemeType::Light;
    case ThemeHelper::Theme::Dark:
        return ThemeType::Dark;
    case ThemeHelper::Theme::System:
        return ThemeType::System;
    }
    return ThemeType::System;
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

qreal Theme::largeTitleFontSize() const
{
    return 12;
}

qreal Theme::largeContentFontSize() const
{
    return 16;
}

qreal Theme::separatorSize() const
{
    return 5;
}

QColor Theme::separatorColor() const
{
    return QColorConstants::DarkGray;
}

QColor Theme::windowColor() const
{
    return QColorConstants::Transparent;
}

QColor Theme::infoPanelColor() const
{
    QColor color = (m_dark ? QColorConstants::Black : QColorConstants::White);
    static constexpr const auto alpha = int(qRound(0.2 * 255.0));
    color.setAlpha(alpha);
    return color;
}

QSizeF Theme::windowSize() const
{
    return {1000, 800};
}

QSizeF Theme::infoPanelSize() const
{
    return {500, 380};
}

bool Theme::eventFilter(QObject *object, QEvent *event)
{
    Q_ASSERT(object);
    Q_ASSERT(event);
    if (!object || !event) {
        return false;
    }
    switch (event->type()) {
    case QEvent::ThemeChange: {
        qDebug() << "Detected theme change event.";
        Q_EMIT themeChanged();
    } break;
    case QEvent::ApplicationPaletteChange: {
        qDebug() << "Detected application palette change event.";
        Q_EMIT themeChanged();
    } break;
    default:
        break;
    }
    return QObject::eventFilter(object, event);
}

void Theme::refresh()
{
    const bool dark = [this]() -> bool {
        const ThemeType preferred = preferredTheme();
        if (preferred == ThemeType::System) {
            if (const QPlatformTheme * const theme = QGuiApplicationPrivate::platformTheme()) {
                return (theme->appearance() == QPlatformTheme::Appearance::Dark);
            }
            return false;
        }
        return (preferred == ThemeType::Dark);
    }();
    if (m_dark == dark) {
        return;
    }
    m_dark = dark;
    Q_EMIT themeChanged();
}
