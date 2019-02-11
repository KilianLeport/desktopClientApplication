/**
 * Monitoring Program
 * Copyright (C) 2018 Kilian Leport
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "deviceitem.h"

DeviceItem::DeviceItem(const Device &dev)
{
    this->setFlags(flags() & ~Qt::ItemIsEditable);
    this->setData(QVariant(dev.getAddress()), Qt::EditRole);
    this->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    nameItem = new QStandardItem();
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    nameItem->setData(QVariant(dev.getName()), Qt::EditRole);
    nameItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    typeItem = new QStandardItem();
    typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
    typeItem->setData(QVariant(dev.getType()), Qt::EditRole);
    typeItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    valueItem = new QStandardItem();
    valueItem->setFlags(valueItem->flags() & ~Qt::ItemIsEditable);
    valueItem->setData(QVariant(dev.getValue()), Qt::EditRole);
    valueItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    stateItem = new QStandardItem();
    stateItem->setFlags(stateItem->flags() & ~Qt::ItemIsEditable);
    stateItem->setData(QVariant(dev.getState()), Qt::EditRole);
    stateItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    coordinateXItem = new QStandardItem();
    coordinateXItem->setFlags(coordinateXItem->flags() & ~Qt::ItemIsEditable);
    coordinateXItem->setData(QVariant(dev.getCoordinateX()), Qt::EditRole);
    coordinateXItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);

    coordinateYItem = new QStandardItem();
    coordinateYItem->setFlags(coordinateYItem->flags() & ~Qt::ItemIsEditable);
    coordinateYItem->setData(QVariant(dev.getCoordinateY()), Qt::EditRole);
    coordinateYItem->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
}
Device DeviceItem::getDevice(){
    Device dev = Device(static_cast<quint8>(this->data(Qt::EditRole).toInt()),
                        static_cast<quint8>(typeItem->data(Qt::EditRole).toInt()),
                        nameItem->data(Qt::EditRole).toString(),
                        valueItem->data(Qt::EditRole).toInt(),
                        static_cast<quint8>(stateItem->data(Qt::EditRole).toInt()),
                        coordinateXItem->data(Qt::EditRole).toInt(),
                        coordinateYItem->data(Qt::EditRole).toInt());
    return dev;
}
