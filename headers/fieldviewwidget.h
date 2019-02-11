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
#ifndef FIELDVIEWWIDGET_H
#define FIELDVIEWWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "device.h"
#include "deviceitemmodel.h"
#include "connectionhandler.h"
#include "fieldrenderviewwidget.h"

class FieldViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FieldViewWidget(std::shared_ptr<DeviceItemModel> model, std::shared_ptr<ConnectionHandler> connectionHandler, QWidget *parent = nullptr);

private:
    void deviceMoved(const quint8 address);
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
    FieldRenderViewWidget * fieldRenderViewWidget;
};

#endif // FIELDVIEWWIDGET_H
