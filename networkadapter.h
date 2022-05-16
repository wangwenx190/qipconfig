#pragma once

#include <QtCore/qstring.h>

struct NetworkAdapter
{
    QString Name = {};
    QString Description = {};
    QString HardwareAddress = {};
    QString IPv4Address = {};
    QString IPv6Address = {};
    QString Netmask = {};
    QString Broadcast = {};
};
