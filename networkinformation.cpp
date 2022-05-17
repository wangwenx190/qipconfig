#include "networkinformation.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QtNetwork/qhostinfo.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>

NetworkInformation::NetworkInformation(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Type>();
}

NetworkInformation::~NetworkInformation() = default;

QString NetworkInformation::getLocalIPAddress(const Type type) const
{
    const QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    if (info.error() != QHostInfo::NoError) {
        qWarning() << info.errorString();
        return {};
    }
    const QList<QHostAddress> addresses = info.addresses();
    if (!addresses.isEmpty()) {
        for (auto &&address : qAsConst(addresses)) {
            if (address.isNull()) {
                continue;
            }
            if (address.isLoopback()) {
                continue;
            }
            if ((type == Type::IPv4) && (address.protocol() == QAbstractSocket::IPv4Protocol)) {
                return address.toString();
            }
            if ((type == Type::IPv6) && (address.protocol() == QAbstractSocket::IPv6Protocol)) {
                return QHostAddress(address.toIPv6Address()).toString();
            }
        }
    }
    return {};
}

QString NetworkInformation::getInternetIPAddress(const Type type) const
{
#if 0
    Q_UNUSED(type);
    const QFuture<QString> future = QtConcurrent::run([]() -> QString {
        QNetworkAccessManager manager;
        manager.setAutoDeleteReplies(true);
        const QPointer<QNetworkReply> reply = manager.get(QNetworkRequest(QUrl(u"123"_qs)));
        const QString data = QString::fromUtf8(reply->readAll());
        return data;
    });
    return future.result();
#else
    Q_UNUSED(type);
    return {};
#endif
}

QString NetworkInformation::localIPv4Address() const
{
    return getLocalIPAddress(Type::IPv4);
}

QString NetworkInformation::localIPv6Address() const
{
    return getLocalIPAddress(Type::IPv6);
}

QString NetworkInformation::internetIPv4Address() const
{
    return getInternetIPAddress(Type::IPv4);
}

QString NetworkInformation::internetIPv6Address() const
{
    return getInternetIPAddress(Type::IPv6);
}
