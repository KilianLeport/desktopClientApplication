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
#include "devicelistwidget.h"

DeviceListWidget::DeviceListWidget(std::shared_ptr<DeviceItemModel> model, std::shared_ptr<ConnectionHandler> connectionHandler, QWidget *parent) : QWidget(parent){
    this->deviceItemModel = model;
    this->connectionHandler = connectionHandler;
    view = new QTableView(this) ;
    view->setModel(deviceItemModel.get());
    view->verticalHeader()->setVisible(false);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);

    view->setItemDelegateForColumn(DeviceItemModel::TYPECOLUMN, new TypeDelegate(this));
    view->setItemDelegateForColumn(DeviceItemModel::STATECOLUMN, new StateDelegate(this));

    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DeviceListWidget::tableViewClicked);
    connect(view->horizontalHeader(), &QHeaderView::sectionClicked, this, &DeviceListWidget::headerViewClicked);

    addModifyDeviceButton = new QPushButton(tr("Add"), this);
    readDeviceButton = new QPushButton(tr("Read"), this);
    deleteDeviceButton = new QPushButton(tr("Delete"), this);
    deleteDeviceButton->setEnabled(false);
    actionDeviceButton = new QPushButton(tr("action"), this);
    actionDeviceButton->setEnabled(false);
    clearAllButton = new QPushButton(tr("Clear all"), this);
    connect(addModifyDeviceButton, &QPushButton::clicked, this, &DeviceListWidget::addModifyDeviceClicked);
    connect(readDeviceButton, &QPushButton::clicked, this, &DeviceListWidget::readDeviceClicked);
    connect(deleteDeviceButton, &QPushButton::clicked, this, &DeviceListWidget::deleteDeviceClicked);
    connect(actionDeviceButton, &QPushButton::clicked, this, &DeviceListWidget::actionDeviceClicked);
    connect(clearAllButton, &QPushButton::clicked, this, &DeviceListWidget::clearAllClicked);

    QHBoxLayout *layoutButton = new QHBoxLayout();
    layoutButton->addWidget(addModifyDeviceButton);
    layoutButton->addWidget(readDeviceButton);
    layoutButton->addWidget(deleteDeviceButton);
    layoutButton->addWidget(actionDeviceButton);
    layoutButton->addWidget(clearAllButton);
    layoutButton->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    layout->addLayout(layoutButton);
    this->setLayout(layout);
}
void DeviceListWidget::tableViewClicked(const QItemSelection & /* unused */, const QItemSelection & /* unused */){
    if(view->selectionModel()->hasSelection()){
        deleteDeviceButton->setEnabled(true);
        actionDeviceButton->setEnabled(true);
        addModifyDeviceButton->setText(tr("Modify device"));
    }
    else{
        deleteDeviceButton->setEnabled(false);
        actionDeviceButton->setEnabled(false);
        addModifyDeviceButton->setText(tr("Add device"));
    }
}
void DeviceListWidget::headerViewClicked(const int logicalIndex){
    deviceItemModel->sort(logicalIndex);
}
void DeviceListWidget::addModifyDeviceClicked(){
    Device dev;
    QString title = tr("Add new device");
    QItemSelectionModel *selectedModel = view->selectionModel();
    if(selectedModel->hasSelection()){
        title = tr("Modify device");
        DeviceItem *item = static_cast<DeviceItem*>(deviceItemModel
                ->itemFromIndex(selectedModel->currentIndex().siblingAtColumn(DeviceItemModel::ADDRESSCOLUMN)));
        dev = Device(item->getDevice());
    }else{//add new device
        dev = Device();
    }
    DeviceEditingDialog deviceDialogEditing(&dev, title, this);
    int result = deviceDialogEditing.exec();
    if(result == QDialog::Accepted){
        connectionHandler->sendData(Device::OrderCode::AddModify, dev.getAddress(), 0, dev.getType(), dev.getName(), dev.getCoordinateX(), dev.getCoordinateY());
    }
}
void DeviceListWidget::readDeviceClicked(){
    int addressToRead = 255;
    QItemSelectionModel *selectedModel = view->selectionModel();
    if(selectedModel->hasSelection()){
        DeviceItem *item = static_cast<DeviceItem*>(deviceItemModel
                ->itemFromIndex(selectedModel->currentIndex().siblingAtColumn(DeviceItemModel::ADDRESSCOLUMN)));
        addressToRead = item->data(Qt::EditRole).toInt();
    }
    connectionHandler->sendData(Device::OrderCode::Read, static_cast<quint8>(addressToRead));
}
void DeviceListWidget::deleteDeviceClicked(){
    int address = 255;
    QItemSelectionModel *selectedModel = view->selectionModel();
    if(selectedModel->hasSelection()){
        DeviceItem *item = static_cast<DeviceItem*>(deviceItemModel
                ->itemFromIndex(selectedModel->currentIndex().siblingAtColumn(DeviceItemModel::ADDRESSCOLUMN)));
        address = item->data(Qt::EditRole).toInt();
        QString question = tr("Are you sure you want to delete this item?");
        int result = QMessageBox::question(this, tr("Confirmation"), question, QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes){
            connectionHandler->sendData(Device::OrderCode::Delete, static_cast<quint8>(address));
        }
    }
}
void DeviceListWidget::actionDeviceClicked(){
    int address = 255;
    QItemSelectionModel *selectedModel = view->selectionModel();
    if(selectedModel->hasSelection()){
        DeviceItem *item = static_cast<DeviceItem*>(deviceItemModel
                ->itemFromIndex(selectedModel->currentIndex().siblingAtColumn(DeviceItemModel::ADDRESSCOLUMN)));
        address = item->data(Qt::EditRole).toInt();
        bool accepted;
        int index = 0;
        QString actionItem = QInputDialog::getItem(this, "Action", "Action:", Device::actionList, 0, false, &accepted);
        if (accepted && !actionItem.isEmpty()){
            index = Device::actionList.indexOf(actionItem);
            connectionHandler->sendData(Device::OrderCode::Action, static_cast<quint8>(address),  static_cast<quint8>(index));
        }
    }
}
void DeviceListWidget::clearAllClicked(){
    int result = QMessageBox::question(this, tr("Confirmation"), tr("Are you sure you want to delete everything ?"),
                          QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes){
        deviceItemModel->clear();
    }
}
