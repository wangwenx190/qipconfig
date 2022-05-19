#include "networkadaptermodel.h"
#include <QtNetwork/qnetworkinterface.h>

NetworkAdapterModel::NetworkAdapterModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<NetworkAdapter>();
    qRegisterMetaType<AdapterRoles>();
}

NetworkAdapterModel::~NetworkAdapterModel() = default;

QHash<int, QByteArray> NetworkAdapterModel::roleNames() const
{
    static const QHash<int, QByteArray> names = {
        { NameRole, "name"_qba },
        { HardwareAddressRole, "hardwareAddress"_qba },
        { IPv4AddressRole, "ipv4Address"_qba },
        { IPv6AddressRole, "ipv6Address"_qba },
        { NetmaskRole, "netmask"_qba },
        { BroadcastAddressRole, "broadcastAddress"_qba }
    };
    return names;
}

int NetworkAdapterModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_adapters.count();
}

int NetworkAdapterModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    static const int count = roleNames().count();
    return count;
}

QVariant NetworkAdapterModel::data(const QModelIndex &index, const int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const int row = index.row();
    if ((row < 0) || (row >= rowCount())) {
        return {};
    }
    const int column = index.column();
    if ((column < 0) || (column >= columnCount())) {
        return {};
    }
    const NetworkAdapter adapter = m_adapters.at(row);
    switch (role) {
    case Qt::DisplayRole: {
        switch (column) {
        case 0:
            return adapter.Name;
        case 1:
            return adapter.HardwareAddress;
        case 2:
            return adapter.IPv4Address;
        case 3:
            return adapter.IPv6Address;
        case 4:
            return adapter.Netmask;
        case 5:
            return adapter.BroadcastAddress;
        default:
            return {};
        }
    }
    case NameRole:
        return adapter.Name;
    case HardwareAddressRole:
        return adapter.HardwareAddress;
    case IPv4AddressRole:
        return adapter.IPv4Address;
    case IPv6AddressRole:
        return adapter.IPv6Address;
    case NetmaskRole:
        return adapter.Netmask;
    case BroadcastAddressRole:
        return adapter.BroadcastAddress;
    default:
        return {};
    }
}

QVariant NetworkAdapterModel::headerData(const int section, const Qt::Orientation orientation, const int role) const
{
    if (role != Qt::DisplayRole) {
        return {};
    }
    if (orientation == Qt::Vertical) {
        return (((section < 0) || (section >= rowCount())) ? QString{} : QString::number(section));
    }
    if ((section < 0) || (section >= columnCount())) {
        return {};
    }
    switch (section) {
    case 0:
        return tr("Name");
    case 1:
        return tr("Hardware Address");
    case 2:
        return tr("IPv4 Address");
    case 3:
        return tr("IPv6 Address");
    case 4:
        return tr("Netmask");
    case 5:
        return tr("Broadcast Address");
    default:
        return {};
    }
}

void NetworkAdapterModel::populate()
{
    beginResetModel();
    m_adapters.clear();
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    if (!interfaces.isEmpty()) {
        for (auto &&interface : qAsConst(interfaces)) {
            if (!interface.isValid()) {
                continue;
            }
            if (interface.type() == QNetworkInterface::Loopback) {
                continue;
            }
            NetworkAdapter adapter = {};
            adapter.Name = interface.humanReadableName();
            adapter.HardwareAddress = interface.hardwareAddress();
            const QList<QNetworkAddressEntry> entries = interface.addressEntries();
            if (!entries.isEmpty()) {
                for (auto &&entry : qAsConst(entries)) {
                    const QHostAddress ip = entry.ip();
                    if (ip.isNull()) {
                        continue;
                    }
                    if (ip.isLoopback()) {
                        continue;
                    }
                    if (adapter.IPv4Address.isEmpty() && (ip.protocol() == QAbstractSocket::IPv4Protocol)) {
                        adapter.IPv4Address = ip.toString();
                    }
                    if (adapter.IPv6Address.isEmpty() && (ip.protocol() == QAbstractSocket::IPv6Protocol)) {
                        adapter.IPv6Address = QHostAddress(ip.toIPv6Address()).toString();
                    }
                    if (adapter.Netmask.isEmpty() && (entry.netmask().protocol() == QAbstractSocket::IPv4Protocol)) {
                        adapter.Netmask = entry.netmask().toString();
                    }
                    if (adapter.BroadcastAddress.isEmpty() && (entry.broadcast().protocol() == QAbstractSocket::IPv4Protocol)) {
                        adapter.BroadcastAddress = entry.broadcast().toString();
                    }
                }
            }
            m_adapters.append(adapter);
        }
    }
    endResetModel();
}
