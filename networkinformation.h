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
    Q_PROPERTY(QString publicIPv4Address READ publicIPv4Address CONSTANT FINAL)

public:
    explicit NetworkInformation(QObject *parent = nullptr);
    ~NetworkInformation() override;

    [[nodiscard]] QString localIPv4Address() const;
    [[nodiscard]] QString publicIPv4Address() const;
};

