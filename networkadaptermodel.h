#pragma once

#include <QtCore/qabstractitemmodel.h>
#include "networkadapter.h"

class NetworkAdapterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(NetworkAdapterModel)

public:
    enum AdapterRoles
    {
        NameRole = Qt::UserRole + 1,
        HardwareAddressRole,
        IPv4AddressRole,
        IPv6AddressRole,
        NetmaskRole,
        BroadcastAddressRole
    };
    Q_ENUM(AdapterRoles)

    explicit NetworkAdapterModel(QObject *parent = nullptr);
    ~NetworkAdapterModel() override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;
    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, const int role = Qt::DisplayRole) const override;
    [[nodiscard]] QVariant headerData(const int section, const Qt::Orientation orientation, const int role = Qt::DisplayRole) const override;

public Q_SLOTS:
    void populate();

private:
    QList<NetworkAdapter> m_adapters = {};
};

