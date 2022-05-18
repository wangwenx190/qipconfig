#include "networkinformation.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QtNetwork/qhostinfo.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qnetworkinformation.h>

NetworkInformation::NetworkInformation(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<AddressType>();
    qRegisterMetaType<NetworkStatus>();
    qRegisterMetaType<TransportMedium>();
    if (QNetworkInformation::loadDefaultBackend()) {
        if (const QNetworkInformation * const ni = QNetworkInformation::instance()) {
            connect(ni, &QNetworkInformation::reachabilityChanged, this, [this](QNetworkInformation::Reachability){ Q_EMIT networkStatusChanged(); });
            connect(ni, &QNetworkInformation::transportMediumChanged, this, [this](QNetworkInformation::TransportMedium){ Q_EMIT transportMediumChanged(); });
            connect(ni, &QNetworkInformation::isBehindCaptivePortalChanged, this, [this](bool){ Q_EMIT behindCaptivePortalChanged(); });
            connect(ni, &QNetworkInformation::isMeteredChanged, this, [this](bool){ Q_EMIT meteredChanged(); });
        } else {
            qWarning() << "The QNetworkInformation instance is null.";
        }
    } else {
        qWarning() << "Failed to load the default QNetworkInformation backend.";
    }
}

NetworkInformation::~NetworkInformation() = default;

NetworkInformation::NetworkStatus NetworkInformation::networkStatus() const
{
    if (const QNetworkInformation * const ni = QNetworkInformation::instance()) {
        const QNetworkInformation::Reachability reachability = ni->reachability();
        if (reachability == QNetworkInformation::Reachability::Online) {
            return NetworkStatus::Online;
        }
        if (reachability != QNetworkInformation::Reachability::Unknown) {
            return NetworkStatus::Offline;
        }
    }
    return NetworkStatus::Unknown;
}

NetworkInformation::TransportMedium NetworkInformation::transportMedium() const
{
    if (const QNetworkInformation * const ni = QNetworkInformation::instance()) {
        switch (ni->transportMedium()) {
        case QNetworkInformation::TransportMedium::Ethernet:
            return TransportMedium::Ethernet;
        case QNetworkInformation::TransportMedium::Cellular:
            return TransportMedium::Cellular;
        case QNetworkInformation::TransportMedium::WiFi:
            return TransportMedium::WiFi;
        case QNetworkInformation::TransportMedium::Bluetooth:
            return TransportMedium::Bluetooth;
        default:
            break;
        }
    }
    return TransportMedium::Unknown;
}

bool NetworkInformation::isBehindCaptivePortal() const
{
    if (const QNetworkInformation * const ni = QNetworkInformation::instance()) {
        return ni->isBehindCaptivePortal();
    }
    return false;
}

bool NetworkInformation::isMetered() const
{
    if (const QNetworkInformation * const ni = QNetworkInformation::instance()) {
        return ni->isMetered();
    }
    return false;
}

QString NetworkInformation::getLocalIPAddress(const AddressType type) const
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
            if ((type == AddressType::IPv4) && (address.protocol() == QAbstractSocket::IPv4Protocol)) {
                return address.toString();
            }
            if ((type == AddressType::IPv6) && (address.protocol() == QAbstractSocket::IPv6Protocol)) {
                return QHostAddress(address.toIPv6Address()).toString();
            }
        }
    }
    return {};
}

QString NetworkInformation::getInternetIPAddress(const AddressType type) const
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
