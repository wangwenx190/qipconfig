#include "themehelper.h"

Q_GLOBAL_STATIC(ThemeHelper, themeHelper)

ThemeHelper::ThemeHelper(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Theme>();
}

ThemeHelper::~ThemeHelper() = default;

ThemeHelper *ThemeHelper::instance()
{
    return themeHelper();
}

ThemeHelper::Theme ThemeHelper::getPreferredTheme() const
{
    return m_preferredTheme;
}

void ThemeHelper::setPreferredTheme(const Theme theme)
{
    if (m_preferredTheme == theme) {
        return;
    }
    m_preferredTheme = theme;
    Q_EMIT preferredThemeChanged();
}
