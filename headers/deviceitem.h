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
#ifndef DEVICEITEM_H
#define DEVICEITEM_H

#include <QStandardItem>

#include "device.h"

class DeviceItem : public QStandardItem
{
public:
    DeviceItem(const Device &dev);
    Device getDevice();
    QStandardItem *getNameItem() const { return nameItem; }
    QStandardItem *getTypeItem() const { return typeItem; }
    QStandardItem *getStateItem() const { return stateItem; }
    QStandardItem *getValueItem() const { return valueItem; }
    QStandardItem *getCoordinateXItem() const { return coordinateXItem; }
    QStandardItem *getCoordinateYItem() const { return coordinateYItem; }

private:
    QStandardItem *nameItem;
    QStandardItem *typeItem;
    QStandardItem *stateItem;
    QStandardItem *valueItem;
    QStandardItem *coordinateXItem;
    QStandardItem *coordinateYItem;
};

#endif // DEVICEITEM_H
