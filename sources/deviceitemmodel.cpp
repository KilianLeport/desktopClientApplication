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
#include "deviceitemmodel.h"

DeviceItemModel::DeviceItemModel(QObject *parent): QStandardItemModel(parent){
    initialize();
}
void DeviceItemModel::initialize(){
    QStringList headerLabels = Device::HeaderLabelList;
    this->setHorizontalHeaderLabels(headerLabels);
}
void DeviceItemModel::clear(){
    QStandardItemModel::clear();
    initialize();
}
void DeviceItemModel::addModifyDevice(const Device &dev){
    if(!hasAddress(dev.getAddress())){
        DeviceItem *deviceItem = new DeviceItem(dev);
        this->appendRow(QList<QStandardItem*>() << deviceItem
                        << deviceItem->getNameItem()
                        << deviceItem->getTypeItem()
                        << deviceItem->getStateItem()
                        << deviceItem->getValueItem()
                        << deviceItem->getCoordinateXItem()
                        << deviceItem->getCoordinateYItem());
    }else{
        DeviceItem *item = static_cast<DeviceItem*>(findItems(QString::number(dev.getAddress())).constFirst());
        item->getNameItem()->setData(QVariant(dev.getName()), Qt::EditRole);
        item->getTypeItem()->setData(QVariant(dev.getType()), Qt::EditRole);
        item->getStateItem()->setData(QVariant(dev.getState()), Qt::EditRole);
        item->getValueItem()->setData(QVariant(dev.getValue()), Qt::EditRole);
        item->getCoordinateXItem()->setData(QVariant(dev.getCoordinateX()), Qt::EditRole);
        item->getCoordinateYItem()->setData(QVariant(dev.getCoordinateY()), Qt::EditRole);
    }
    sort(ADDRESSCOLUMN);
}
bool DeviceItemModel::hasAddress(const int address){
    QList<QStandardItem*> list = findItems(QString::number(address), Qt::MatchExactly, ADDRESSCOLUMN);
    return !list.isEmpty();
}
DeviceItem *DeviceItemModel::getDeviceItemFromAddress(const int address){
    if(!hasAddress(address))
        return nullptr;
    DeviceItem *item = static_cast<DeviceItem*>(findItems(QString::number(address), Qt::MatchExactly, DeviceItemModel::ADDRESSCOLUMN)
            .constFirst());
    return item;
}
QDomDocument DeviceItemModel::getDomDocument(){
    QDomDocument doc;
    QDomElement devicesElement = doc.createElement("devices");
    doc.appendChild(devicesElement);
    for (int row = 0; row < rowCount(); ++row) {
        DeviceItem *item = static_cast<DeviceItem*>(this->item(row, ADDRESSCOLUMN));
        devicesElement.appendChild(item->getDevice().getDomDocument());
    }
    return doc;
}
