#include "utils.h"
#include <QtGui/qguiapplication.h>
#include <QtGui/qclipboard.h>

Utils::Utils(QObject *parent) : QObject(parent)
{
}

Utils::~Utils() = default;

void Utils::copyToClipboard(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }
    QGuiApplication::clipboard()->setText(text);
}
