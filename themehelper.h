#pragma once

#include <QtCore/qobject.h>

class ThemeHelper : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(ThemeHelper)
    Q_PROPERTY(Theme preferredTheme READ getPreferredTheme WRITE setPreferredTheme NOTIFY preferredThemeChanged FINAL)

public:
    enum class Theme { Light, Dark, System, Default = Dark };
    Q_ENUM(Theme)

    explicit ThemeHelper(QObject *parent = nullptr);
    ~ThemeHelper() override;

    [[nodiscard]] static ThemeHelper *instance();

    [[nodiscard]] Theme getPreferredTheme() const;
    void setPreferredTheme(const Theme theme);

Q_SIGNALS:
    void preferredThemeChanged();
    void systemThemeChanged();

private:
    Theme m_preferredTheme = Theme::System;
};
