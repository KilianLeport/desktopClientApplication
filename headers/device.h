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
#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QDomDocument>
#include <QTcpSocket>

class Device
{

public:
    Device();
    Device(const quint8 address, const quint8 type, const QString name = QString(), const int value = 0 , const quint8 state = 0, const int coordinateX = 0, const int coordinateY = 0);
    Device(const Device* device);
    void setName(const QString name);
    void setAddress(const quint8 address);
    void setType(const quint8 type);
    void setValue(const int value);
    void setState(const quint8 state);
    void setCoordinateX(const int coordinateX);
    void setCoordinateY(const int coordinateY);
    QString getName() const;
    quint8 getAddress() const;
    quint8 getType() const;
    QString getTypeString() const;
    int getValue() const;
    QString toString() const;
    quint8 getState() const;
    QString getStateString() const;
    int getCoordinateX() const;
    int getCoordinateY() const;
    QDomDocument getDomDocument();
    static Device parseDomNode(const QDomNode domNode);


    //maximum constant
    static const int MAXIMUMADDRESS = 254;
    static const int MAXIMUMTYPE = 255;
    static const int MAXIMUMSTATE = 255;
    static const int MAXIMUMVALUE = 65535;
    static const int MAXIMUMSIZENAMEDEVICE = 32;
    static const int MAXIMUMCOORDINATEX = 100;
    static const int MAXIMUMCOORDINATEY = 100;

    static const quint8 ADDRESSBROADCAST = 255;

    static const QStringList actionList;
    static const QStringList typeList;
    static const QStringList stateList;
    static const QStringList HeaderLabelList;

    enum AckCode : quint8
    {
        OKread = 1,
        OK = 2,
        InvalidAddress = 3,
        UnknownAddress = 4,
        InvalidName = 5,
        UnreachedDevice = 6,
        InvalidFrameFromClient = 7,
        InvalidFrameFromServer = 8,
        ServerOverloaded = 9
    };
    enum OrderCode : quint8
    {
        UnknownCode = 0,
        AddModify = 1,
        Delete = 2,
        Read = 3,
        Action = 4
    };
    enum ActionCode : quint8
    {
        UnknownAction = 0,
        TurnOn = 1,
        TurnOff = 2
    };
    enum StateCode : quint8
    {
        UnknownState = 0,
        On = 1,
        Off = 2,
        Unreachable = 3
    };
    enum TypeCode : quint8
    {
        UnknownType = 0,
        Interrupter = 1,
        Measure = 2
    };

    struct OrderStructure {
        Device::OrderCode orderCode;
        QTcpSocket *socket;
        quint8 address;
        quint8 action;
        quint8 type;
        QString name;
        int coordinateX;
        int coordinateY;
        OrderStructure()
        {
            orderCode = Device::OrderCode::UnknownCode;
            socket = nullptr;
            address = 255;
            action = Device::ActionCode::UnknownAction;
            type = Device::TypeCode::UnknownType;
            name = QString();
            coordinateX = 0;
            coordinateY = 0;
        }
    };

private:
    QString name;
    quint8 address;
    int value;
    quint8 type;
    quint8 state;
    int coordinateX;
    int coordinateY;
};

#endif // DEVICE_H
