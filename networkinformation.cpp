/*
 * MIT License
 *
 * Copyright (C) 2022 by wangwenx190 (Yuhang Zhao)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "networkinformation.h"
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>
#include <QtNetwork/qhostinfo.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qnetworkinformation.h>

static constexpr const int MAX_RETRY_TIMES = 30;

NetworkInformation::NetworkInformation(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<AddressType>();
    qRegisterMetaType<NetworkStatus>();
    qRegisterMetaType<TransportMedium>();
    m_networkManager.reset(new QNetworkAccessManager(this));
    m_networkManager->setAutoDeleteReplies(true);
    m_networkManager->setRedirectPolicy(QNetworkRequest::ManualRedirectPolicy);
    m_networkManager->setStrictTransportSecurityEnabled(true);
    // We need to set a timeout value explicitly because the default setting
    // of QNetworkAccessManager is zero which means unlimited timeout.
    m_networkManager->setTransferTimeout(QNetworkRequest::DefaultTransferTimeoutConstant);
    connect(m_networkManager.get(), &QNetworkAccessManager::finished, this, [this](QNetworkReply *reply){
        Q_ASSERT(reply);
        if (!reply) {
            return;
        }
        if (m_retryTimes >= MAX_RETRY_TIMES) {
            m_retryTimes = 0;
            qWarning() << "Reached the limitation of the maximum retry times.";
            return;
        }
        ++m_retryTimes;
        QJsonParseError jsonErr = {};
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll(), &jsonErr);
        if (jsonErr.error != QJsonParseError::NoError) {
            qWarning() << jsonErr.errorString();
            return;
        }
        if (!jsonDoc.isObject()) {
            qWarning() << "The JSON document is not valid.";
            return;
        }
        const QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains(u"ip"_qs)) {
            qWarning() << "The JSON object does not contain the expected data.";
            return;
        }
        const QJsonValue jsonVal = jsonObj.value(u"ip"_qs);
        if (!jsonVal.isString()) {
            qWarning() << "The JSON value is not valid.";
            return;
        }
        const QString ip = jsonVal.toString();
        if (ip.isEmpty()) {
            qWarning() << "The received string is empty.";
            return;
        }
        const QHostAddress address(ip);
        if (address.isNull() || address.isLoopback()) {
            qWarning() << "Received null or loopback IP address.";
            return;
        }
        if (address.protocol() == QAbstractSocket::IPv4Protocol) {
            m_internetAddressIPv4 = address.toString();
            Q_EMIT internetAddressChanged();
            m_retryTimes = 0;
            return;
        }
#if 0
        else if (address.protocol() == QAbstractSocket::IPv6Protocol) {
            m_internetAddressIPv6 = QHostAddress(address.toIPv6Address()).toString();
            Q_EMIT internetAddressChanged();
            m_retryTimes = 0;
            return;
        }
#endif
        tryFetchInternetAddress();
    });
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
    m_retryTimes = 0;
    tryFetchInternetAddress();
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
    static const QString result = QHostInfo::localHostName();
    return result;
}

QString NetworkInformation::localDomainName() const
{
    static const QString result = QHostInfo::localDomainName();
    return result;
}

QString NetworkInformation::internetAddress() const
{
    if (networkStatus() != NetworkStatus::Online) {
        return tr("NOT AVAILABLE");
    }
    if (m_internetAddressIPv4.isEmpty()) {
        return tr("WAITING");
    }
    return m_internetAddressIPv4;
}

QString NetworkInformation::localAddress() const
{
    static const QString result = []() -> QString {
        const QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
        if (info.error() != QHostInfo::NoError) {
            qWarning() << info.errorString();
            return tr("NOT AVAILABLE");
        }
        const QList<QHostAddress> addresses = info.addresses();
        if (addresses.isEmpty()) {
            return tr("NOT AVAILABLE");
        }
        for (auto &&address : qAsConst(addresses)) {
            if (address.isNull() || address.isLoopback()) {
                continue;
            }
            if (address.protocol() == QAbstractSocket::IPv4Protocol) {
                return address.toString();
            }
#if 0
            else if (address.protocol() == QAbstractSocket::IPv6Protocol) {
                return QHostAddress(address.toIPv6Address()).toString();
            }
#endif
        }
        return tr("NOT AVAILABLE");
    }();
    return result;
}

void NetworkInformation::tryFetchInternetAddress()
{
    if (networkStatus() != NetworkStatus::Online) {
        qWarning() << "Cannot fetch the Internet address due to there is no Internet access.";
        return;
    }
    m_networkManager->get(QNetworkRequest(QUrl(u"https://api64.ipify.org?format=json"_qs)));
}
