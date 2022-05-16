#include "networkadaptermodel.h"
#include <QtNetwork/qnetworkinterface.h>

NetworkAdapterModel::NetworkAdapterModel(QObject *parent) : QAbstractListModel(parent)
{
}

NetworkAdapterModel::~NetworkAdapterModel() = default;

QHash<int, QByteArray> NetworkAdapterModel::roleNames() const
{
    QHash<int, QByteArray> names = {};
    names.insert(NameRole, "name"_qba);
    names.insert(DescriptionRole, "description"_qba);
    names.insert(HardwareAddressRole, "hardwareAddress"_qba);
    names.insert(IPv4AddressRole, "ipv4Address"_qba);
    names.insert(IPv6AddressRole, "ipv6Address"_qba);
    names.insert(NetmaskRole, "netmask"_qba);
    names.insert(BroadcastRole, "broadcast"_qba);
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
    return 7;
}

QVariant NetworkAdapterModel::data(const QModelIndex &index, const int role) const
{
    if (!index.isValid()) {
        return {};
    }
    const int row = index.row();
    if ((row < 0) || row >= m_adapters.count()) {
        return {};
    }
    const NetworkAdapter adapter = m_adapters.at(row);
    switch (role) {
    case Qt::DisplayRole: {
        switch (index.column()) {
        case 0:
            return adapter.Name;
        case 1:
            return adapter.Description;
        case 2:
            return adapter.HardwareAddress;
        case 3:
            return adapter.IPv4Address;
        case 4:
            return adapter.IPv6Address;
        case 5:
            return adapter.Netmask;
        case 6:
            return adapter.Broadcast;
        default:
            return {};
        }
    }
    case NameRole:
        return adapter.Name;
    case DescriptionRole:
        return adapter.Description;
    case HardwareAddressRole:
        return adapter.HardwareAddress;
    case IPv4AddressRole:
        return adapter.IPv4Address;
    case IPv6AddressRole:
        return adapter.IPv6Address;
    case NetmaskRole:
        return adapter.Netmask;
    case BroadcastRole:
        return adapter.Broadcast;
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
        return QString::number(section);
    }
    switch (section) {
    case 0:
        return tr("Name");
    case 1:
        return tr("Description");
    case 2:
        return tr("Hardware Address");
    case 3:
        return tr("IPv4 Address");
    case 4:
        return tr("IPv6 Address");
    case 5:
        return tr("Netmask");
    case 6:
        return tr("Broadcast");
    }
    return {};
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
            //adapter.Description = u""_qs;
            adapter.HardwareAddress = interface.hardwareAddress();
            const QList<QNetworkAddressEntry> entries = interface.addressEntries();
            if (!entries.isEmpty()) {
                for (auto &&entry : qAsConst(entries)) {
                    const QHostAddress address = entry.ip();
                    if (adapter.IPv4Address.isEmpty() && (address.protocol() == QAbstractSocket::IPv4Protocol)) {
                        adapter.IPv4Address = address.toString();
                    }
                    if (adapter.IPv6Address.isEmpty() && (address.protocol() == QAbstractSocket::IPv6Protocol)) {
                        adapter.IPv6Address = QHostAddress(address.toIPv6Address()).toString();
                    }
                    if (adapter.Netmask.isEmpty() && (entry.netmask().protocol() == QAbstractSocket::IPv4Protocol)) {
                        adapter.Netmask = entry.netmask().toString();
                    }
                    if (adapter.Broadcast.isEmpty() && (entry.broadcast().protocol() == QAbstractSocket::IPv4Protocol)) {
                        adapter.Broadcast = entry.broadcast().toString();
                    }
                }
            }
            m_adapters.append(adapter);
        }
    }
    endResetModel();
}
