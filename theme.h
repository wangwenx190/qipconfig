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

