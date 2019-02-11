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
#ifndef DEVICEITEMMODEL_H
#define DEVICEITEMMODEL_H

#include <QStandardItemModel>

#include "device.h"
#include "deviceitem.h"

class DeviceItemModel : public QStandardItemModel
{
    Q_OBJECT

public:
    DeviceItemModel(QObject *parent = nullptr);
    void clear();
    void addModifyDevice(const Device &dev);
    bool hasAddress(const int address);
    DeviceItem *getDeviceItemFromAddress(const int address);
    QDomDocument getDomDocument();

    static const int ADDRESSCOLUMN = 0;
    static const int NAMECOLUMN = 1;
    static const int TYPECOLUMN = 2;
    static const int STATECOLUMN = 3;
    static const int VALUECOLUMN = 4;
    static const int COORDINATEXCOLUMN = 4;
    static const int COORDINATEYCOLUMN = 4;

private:
    void initialize();

};

#endif // DEVICEITEMMODEL_H
