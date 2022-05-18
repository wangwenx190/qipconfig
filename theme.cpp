#include "theme.h"
#include "themenotifier.h"
#include <QtGui/qpa/qplatformtheme.h>
#include <QtGui/private/qguiapplication_p.h>

#ifdef Q_OS_WINDOWS
extern void setupWin32EventFilter();
#endif

Theme::Theme(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<ThemeType>();
#ifdef Q_OS_WINDOWS
    setupWin32EventFilter();
#endif
    connect(ThemeNotifier::instance(), &ThemeNotifier::themeChanged, this, &Theme::refresh);
    refresh();
}

Theme::~Theme() = default;

Theme::ThemeType Theme::theme() const
{
    return (m_dark ? ThemeType::Dark : ThemeType::Light);
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
