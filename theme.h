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

    Q_PROPERTY(Type theme READ theme NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor accentColor READ accentColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(QColor labelColor READ labelColor NOTIFY themeChanged FINAL)
    Q_PROPERTY(qreal titleFontSize READ titleFontSize CONSTANT FINAL)
    Q_PROPERTY(qreal contentFontSize READ contentFontSize CONSTANT FINAL)

public:
    enum class Type { Light, Dark };
    Q_ENUM(Type)

    explicit Theme(QObject *parent = nullptr);
    ~Theme() override;

    [[nodiscard]] Type theme() const;
    [[nodiscard]] QColor accentColor() const;
    [[nodiscard]] QColor labelColor() const;
    [[nodiscard]] qreal titleFontSize() const;
    [[nodiscard]] qreal contentFontSize() const;

private:
    void refresh();

Q_SIGNALS:
    void themeChanged();

private:
    bool m_dark = false;
};

