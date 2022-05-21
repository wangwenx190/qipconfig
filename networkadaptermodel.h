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
