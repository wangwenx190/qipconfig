#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqmlregistration.h>

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

    [[nodiscard]] Q_INVOKABLE QString getLocalIPAddress(const AddressType type) const;
    [[nodiscard]] Q_INVOKABLE QString getInternetIPAddress(const AddressType type) const;

Q_SIGNALS:
    void networkStatusChanged();
    void transportMediumChanged();
    void behindCaptivePortalChanged();
    void meteredChanged();
};

