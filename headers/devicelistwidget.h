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
#ifndef DEVICELISTWIDGET_H
#define DEVICELISTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QInputDialog>

#include "device.h"
#include "deviceitemmodel.h"
#include "deviceeditingdialog.h"
#include "connectionhandler.h"
#include "customdelegates.h"

class DeviceListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DeviceListWidget(std::shared_ptr<DeviceItemModel> model, std::shared_ptr<ConnectionHandler> connectionHandler, QWidget *parent = nullptr);

private:
    void headerViewClicked(const int logicalIndex);
    void tableViewClicked(const QItemSelection & /* item */, const QItemSelection & /* item */);
    void addModifyDeviceClicked();
    void readDeviceClicked();
    void deleteDeviceClicked();
    void actionDeviceClicked();
    void clearAllClicked();

private:
    std::shared_ptr<DeviceItemModel> deviceItemModel;
    std::shared_ptr<ConnectionHandler> connectionHandler;
    QPushButton *addModifyDeviceButton;
    QPushButton *readDeviceButton;
    QPushButton *deleteDeviceButton;
    QPushButton *actionDeviceButton;
    QPushButton *clearAllButton;
    QTableView  *view;
};

#endif // DEVICELISTWIDGET_H
