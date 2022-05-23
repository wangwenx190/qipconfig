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

#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqmlregistration.h>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class NetworkInformation : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(NetworkInformation)

    Q_PROPERTY(NetworkStatus networkStatus READ networkStatus NOTIFY networkStatusChanged FINAL)
    Q_PROPERTY(TransportMedium transportMedium READ transportMedium NOTIFY transportMediumChanged FINAL)
    Q_PROPERTY(bool behindCaptivePortal READ isBehindCaptivePortal NOTIFY behindCaptivePortalChanged FINAL)
    Q_PROPERTY(bool metered READ isMetered NOTIFY meteredChanged FINAL)
    Q_PROPERTY(QString localHostName READ localHostName CONSTANT FINAL)
    Q_PROPERTY(QString localDomainName READ localDomainName CONSTANT FINAL)
    Q_PROPERTY(QString internetAddress READ internetAddress NOTIFY internetAddressChanged FINAL)
    Q_PROPERTY(QString localAddress READ localAddress CONSTANT FINAL)

public:
    enum class AddressType { IPv4, IPv6 };
    Q_ENUM(AddressType)

    enum class NetworkStatus { Unknown, Online, Offline };
    Q_ENUM(NetworkStatus)

    enum class TransportMedium { Unknown, Ethernet, Cellular, WiFi, Bluetooth };
    Q_ENUM(TransportMedium)

    explicit NetworkInformation(QObject *parent = nullptr);
    ~NetworkInformation() override;

    [[nodiscard]] NetworkStatus networkStatus() const;
    [[nodiscard]] TransportMedium transportMedium() const;
    [[nodiscard]] bool isBehindCaptivePortal() const;
    [[nodiscard]] bool isMetered() const;
    [[nodiscard]] QString localHostName() const;
    [[nodiscard]] QString localDomainName() const;
    [[nodiscard]] QString internetAddress() const;
    [[nodiscard]] QString localAddress() const;

private:
    void tryFetchInternetAddress();

Q_SIGNALS:
    void networkStatusChanged();
    void transportMediumChanged();
    void behindCaptivePortalChanged();
    void meteredChanged();
    void internetAddressChanged();

private:
    QScopedPointer<QNetworkAccessManager> m_networkManager;
    QString m_internetAddressIPv4 = {};
    QString m_internetAddressIPv6 = {};
    int m_retryTimes = 0;
};
