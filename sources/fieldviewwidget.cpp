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
#include "fieldviewwidget.h"

FieldViewWidget::FieldViewWidget(std::shared_ptr<DeviceItemModel> model, std::shared_ptr<ConnectionHandler> connectionHandler, QWidget *parent) : QWidget(parent)
{
    this->deviceItemModel = model;
    this->connectionHandler = connectionHandler;
    fieldRenderViewWidget = new FieldRenderViewWidget(model, this);


    connect(fieldRenderViewWidget, &FieldRenderViewWidget::deviceMoved, this, &FieldViewWidget::deviceMoved);

    //connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DeviceListWidget::tableViewClicked);
    //connect(view->horizontalHeader(), &QHeaderView::sectionClicked, this, &DeviceListWidget::headerViewClicked);

    addModifyDeviceButton = new QPushButton(tr("Add"), this);
    readDeviceButton = new QPushButton(tr("Read"), this);
    deleteDeviceButton = new QPushButton(tr("Delete"), this);
    deleteDeviceButton->setEnabled(false);
    actionDeviceButton = new QPushButton(tr("action"), this);
    actionDeviceButton->setEnabled(false);
    clearAllButton = new QPushButton(tr("Clear all"), this);
    connect(addModifyDeviceButton, &QPushButton::clicked, this, &FieldViewWidget::addModifyDeviceClicked);
    connect(readDeviceButton, &QPushButton::clicked, this, &FieldViewWidget::readDeviceClicked);
    connect(deleteDeviceButton, &QPushButton::clicked, this, &FieldViewWidget::deleteDeviceClicked);
    connect(actionDeviceButton, &QPushButton::clicked, this, &FieldViewWidget::actionDeviceClicked);
    connect(clearAllButton, &QPushButton::clicked, this, &FieldViewWidget::clearAllClicked);

    QHBoxLayout *layoutButton = new QHBoxLayout();
    layoutButton->addWidget(addModifyDeviceButton);
    layoutButton->addWidget(readDeviceButton);
    layoutButton->addWidget(deleteDeviceButton);
    layoutButton->addWidget(actionDeviceButton);
    layoutButton->addWidget(clearAllButton);
    layoutButton->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(fieldRenderViewWidget);
    layout->addLayout(layoutButton);
    this->setLayout(layout);
}
void FieldViewWidget::deviceMoved(const quint8 address){
    if(deviceItemModel->hasAddress(address)){
    DeviceItem *item = deviceItemModel->getDeviceItemFromAddress(address);
    Device dev = item->getDevice();
    connectionHandler->sendData(Device::OrderCode::AddModify,
                                address,
                                Device::ActionCode::UnknownAction,
                                dev.getType(), dev.getName(),
                                dev.getCoordinateX(),
                                dev.getCoordinateY());
    }
}
void FieldViewWidget::addModifyDeviceClicked(){
    //TODO
}
void FieldViewWidget::readDeviceClicked(){
    connectionHandler->sendData(Device::OrderCode::Read, Device::ADDRESSBROADCAST);
}
void FieldViewWidget::deleteDeviceClicked(){
    //TODO
}
void FieldViewWidget::actionDeviceClicked(){
    //TODO
}
void FieldViewWidget::clearAllClicked(){
    int result = QMessageBox::question(this, tr("Confirmation"), tr("Are you sure you want to delete everything ?"),
                          QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes){
        deviceItemModel->clear();
    }
}

