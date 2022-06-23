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
#include <QtGui/qpa/qplatformtheme.h>
#include <QtGui/private/qguiapplication_p.h>

Theme::Theme(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<ThemeType>();
    refresh();
}

Theme::~Theme() = default;

void Theme::setPreferredTheme(const ThemeType value)
{
    if (m_preferredTheme == value) {
        return;
    }
    m_preferredTheme = value;
}

Theme::ThemeType Theme::theme() const
{
    return (m_dark ? ThemeType::Dark : ThemeType::Light);
}

Theme::ThemeType Theme::preferredTheme() const
{
    return m_preferredTheme;
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
    static constexpr const int alpha = qRound(0.2 * 255.0);
    color.setAlpha(alpha);
    return color;
}

QSizeF Theme::windowSize() const
{
    return {1020, 800};
}

QSizeF Theme::infoPanelSize() const
{
    return {500, 430};
}

qreal Theme::contentsMargin() const
{
    return 10;
}

QSizeF Theme::aboutDialogSize() const
{
    return {610, 450};
}

qreal Theme::titleBarHeight() const
{
    return 32;
}

QString Theme::iconFontName() const
{
    static const QString result = u"Segoe Fluent Icons"_qs;
    return result;
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
        qDebug() << "Detected system theme change event.";
        refresh();
    } break;
    case QEvent::ApplicationPaletteChange: {
        qDebug() << "Detected application palette change event.";
        refresh();
    } break;
    default:
        break;
    }
    return QObject::eventFilter(object, event);
}

void Theme::refresh()
{
    m_dark = []() -> bool {
        if (m_preferredTheme == ThemeType::System) {
            if (const QPlatformTheme * const theme = QGuiApplicationPrivate::platformTheme()) {
                return (theme->appearance() == QPlatformTheme::Appearance::Dark);
            }
            return false;
        }
        return (m_preferredTheme == ThemeType::Dark);
    }();
    Q_EMIT themeChanged();
}

void Theme::setup(QWindow * const window)
{
    Q_ASSERT(window);
    if (!window) {
        return;
    }
    if (m_preferredTheme != ThemeType::System) {
        return;
    }
    window->installEventFilter(this);
}
