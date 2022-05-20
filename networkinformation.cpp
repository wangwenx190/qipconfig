#include "networkinformation.h"
#include <QtConcurrent/qtconcurrentrun.h>
#include <QtNetwork/qhostinfo.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qnetworkinformation.h>

static constexpr const int MAX_TRANSFER_TIMEOUT = 1000 * 10; // 10s
static constexpr const int MAX_RETRY_TIMES = 10;

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

QString NetworkInformation::localHostName() const
{
    return QHostInfo::localHostName();
}

QString NetworkInformation::localDomainName() const
{
    return QHostInfo::localDomainName();
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
    if (networkStatus() != NetworkStatus::Online) {
        return tr("NOT AVAILABLE");
    }
    const QFuture<QString> future = QtConcurrent::run([type]() -> QString {
        QNetworkAccessManager manager;
        manager.setAutoDeleteReplies(true);
        manager.setRedirectPolicy(QNetworkRequest::ManualRedirectPolicy);
        manager.setStrictTransportSecurityEnabled(true);
        // We need to set a timeout value explicitly because the default setting
        // of QNetworkAccessManager is zero which means unlimited timeout.
        manager.setTransferTimeout(MAX_TRANSFER_TIMEOUT);
        int triedTimes = 0;
        Q_FOREVER {
            if (triedTimes >= MAX_RETRY_TIMES) {
                return tr("ERROR");
            }
            ++triedTimes;
            const QPointer<QNetworkReply> reply = manager.get(QNetworkRequest(QUrl(u"https://api64.ipify.org"_qs)));
            const QString data = QString::fromUtf8(reply->readAll());
            if (data.isEmpty()) {
                qWarning() << "The received data is empty.";
                continue;
            }
            const QHostAddress address(data);
            if (address.isNull() || address.isLoopback()) {
                qWarning() << "Skipping null and loopback IP addresses ...";
                continue;
            }
            if ((type == AddressType::IPv4) && (address.protocol() == QAbstractSocket::IPv4Protocol)) {
                return address.toString();
            }
            if ((type == AddressType::IPv6) && (address.protocol() == QAbstractSocket::IPv6Protocol)) {
                return QHostAddress(address.toIPv6Address()).toString();
            }
        }
    });
    return future.result();
}
