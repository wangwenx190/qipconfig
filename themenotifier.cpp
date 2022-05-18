#include "themenotifier.h"

Q_GLOBAL_STATIC(ThemeNotifier, themeNotifier)

ThemeNotifier::ThemeNotifier(QObject *parent) : QObject(parent)
{
}

ThemeNotifier::~ThemeNotifier() = default;

ThemeNotifier *ThemeNotifier::instance()
{
    return themeNotifier();
}
