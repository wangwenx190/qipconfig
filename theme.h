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

#pragma once

#include <QtCore/qobject.h>
#include <QtCore/qsize.h>
#include <QtGui/qcolor.h>
#include <QtQml/qqmlregistration.h>

class Theme : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(Theme)

    Q_PROPERTY(ThemeType theme READ theme NOTIFY themeChanged FINAL)
    Q_PROPERTY(ThemeType preferredTheme READ preferredTheme NOTIFY preferredThemeChanged FINAL)
    Q_PROPERTY(QColor accentColor READ accentColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor labelColor READ labelColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(qreal titleFontSize READ titleFontSize CONSTANT FINAL)
    Q_PROPERTY(qreal contentFontSize READ contentFontSize CONSTANT FINAL)
    Q_PROPERTY(qreal largeTitleFontSize READ largeTitleFontSize CONSTANT FINAL)
    Q_PROPERTY(qreal largeContentFontSize READ largeContentFontSize CONSTANT FINAL)
    Q_PROPERTY(qreal separatorSize READ separatorSize CONSTANT FINAL)
    Q_PROPERTY(QColor separatorColor READ separatorColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor windowColor READ windowColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor infoPanelColor READ infoPanelColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(QSizeF windowSize READ windowSize CONSTANT FINAL)
    Q_PROPERTY(QSizeF infoPanelSize READ infoPanelSize CONSTANT FINAL)
    Q_PROPERTY(qreal contentsMargin READ contentsMargin CONSTANT FINAL)

public:
    enum class ThemeType { Light, Dark, System, Default = Dark };
    Q_ENUM(ThemeType)

    explicit Theme(QObject *parent = nullptr);
    ~Theme() override;

    [[nodiscard]] ThemeType theme() const;
    [[nodiscard]] ThemeType preferredTheme() const;
    [[nodiscard]] QColor accentColor() const;
    [[nodiscard]] QColor labelColor() const;
    [[nodiscard]] qreal titleFontSize() const;
    [[nodiscard]] qreal contentFontSize() const;
    [[nodiscard]] qreal largeTitleFontSize() const;
    [[nodiscard]] qreal largeContentFontSize() const;
    [[nodiscard]] qreal separatorSize() const;
    [[nodiscard]] QColor separatorColor() const;
    [[nodiscard]] QColor windowColor() const;
    [[nodiscard]] QColor infoPanelColor() const;
    [[nodiscard]] QSizeF windowSize() const;
    [[nodiscard]] QSizeF infoPanelSize() const;
    [[nodiscard]] qreal contentsMargin() const;

public Q_SLOTS:
    void refresh();

protected:
    [[nodiscard]] bool eventFilter(QObject *object, QEvent *event) override;

Q_SIGNALS:
    void themeChanged();
    void preferredThemeChanged();

private:
    bool m_dark = false;
};
