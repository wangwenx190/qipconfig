#pragma once

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

struct NetworkAdapter
{
    QString Name = {};
    QString HardwareAddress = {};
    QString IPv4Address = {};
    QString IPv6Address = {};
    QString Netmask = {};
    QString BroadcastAddress = {};
};

Q_DECLARE_METATYPE(NetworkAdapter)
