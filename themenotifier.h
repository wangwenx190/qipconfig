#pragma once

#include <QtCore/qobject.h>

class ThemeNotifier : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ThemeNotifier)

public:
    explicit ThemeNotifier(QObject *parent = nullptr);
    ~ThemeNotifier() override;

    [[nodiscard]] static ThemeNotifier *instance();

Q_SIGNALS:
    void themeChanged();
};

