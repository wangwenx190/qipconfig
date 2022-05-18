#pragma once

#include <QtCore/qobject.h>
#include <QtGui/qcolor.h>
#include <QtQml/qqmlregistration.h>

class Theme : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(Theme)

    Q_PROPERTY(ThemeType theme READ theme NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor accentColor READ accentColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor labelColor READ labelColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(qreal titleFontSize READ titleFontSize CONSTANT FINAL)
    Q_PROPERTY(qreal contentFontSize READ contentFontSize CONSTANT FINAL)

public:
    enum class ThemeType { Light, Dark };
    Q_ENUM(ThemeType)

    explicit Theme(QObject *parent = nullptr);
    ~Theme() override;

    [[nodiscard]] ThemeType theme() const;
    [[nodiscard]] QColor accentColor() const;
    [[nodiscard]] QColor labelColor() const;
    [[nodiscard]] qreal titleFontSize() const;
    [[nodiscard]] qreal contentFontSize() const;

public Q_SLOTS:
    void refresh();

protected:
    [[nodiscard]] bool eventFilter(QObject *object, QEvent *event) override;

Q_SIGNALS:
    void themeChanged();

private:
    bool m_dark = false;
};

