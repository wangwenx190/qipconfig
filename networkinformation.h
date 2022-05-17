#pragma once

#include <QtCore/qobject.h>
#include <QtQml/qqmlregistration.h>

class NetworkInformation : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_DISABLE_COPY_MOVE(NetworkInformation)

    Q_PROPERTY(QString localIPv4Address READ localIPv4Address CONSTANT FINAL)
    Q_PROPERTY(QString localIPv6Address READ localIPv6Address CONSTANT FINAL)
    Q_PROPERTY(QString internetIPv4Address READ internetIPv4Address CONSTANT FINAL)
    Q_PROPERTY(QString internetIPv6Address READ internetIPv6Address CONSTANT FINAL)

public:
    enum class Type { IPv4, IPv6 };
    Q_ENUM(Type)

    explicit NetworkInformation(QObject *parent = nullptr);
    ~NetworkInformation() override;

    [[nodiscard]] Q_INVOKABLE QString getLocalIPAddress(const Type type) const;
    [[nodiscard]] Q_INVOKABLE QString getInternetIPAddress(const Type type) const;

    [[nodiscard]] QString localIPv4Address() const;
    [[nodiscard]] QString localIPv6Address() const;
    [[nodiscard]] QString internetIPv4Address() const;
    [[nodiscard]] QString internetIPv6Address() const;
};

