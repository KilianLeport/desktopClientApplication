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
#include "device.h"

const QStringList Device::actionList = QStringList() << QObject::tr("Unknown")
                                                     << QObject::tr("Turn On")
                                                     << QObject::tr("Turn Off");
const QStringList Device::typeList = QStringList() << QObject::tr("Unknown")
                                                   << QObject::tr("Interrupter")
                                                   << QObject::tr("Measure");
const QStringList Device::stateList = QStringList() << QObject::tr("Unknown")
                                                    << QObject::tr("On")
                                                    << QObject::tr("Off")
                                                    << QObject::tr("Unreachable");
const QStringList Device::HeaderLabelList = QStringList()<< QObject::tr("Address")
                                                         << QObject::tr("Name")
                                                         << QObject::tr("Type")
                                                         << QObject::tr("State")
                                                         << QObject::tr("Value")
                                                         << QObject::tr("Coordinate X")
                                                         << QObject::tr("Coordinate Y");

Device::Device(){
    address = 255;
    type = 0;
    name = QString();
    value = 0;
    state = 0;
    coordinateX = 0;
    coordinateY = 0;
}
Device::Device(const quint8 address, const quint8 type, const QString name, const int value, const quint8 state, const int coordinateX, const int coordinateY){
    this->address = address;
    this->type = type;
    this->name = name;
    this->value = value;
    this->state = state;
    this->coordinateX = coordinateX;
    this->coordinateY = coordinateY;
}
Device::Device(const Device *device){
    this->address = device->getAddress();
    this->type = device->getType();
    this->name = device->getName();
    this->value = device->getValue();
    this->state = device->getState();
    this->coordinateX = device->getCoordinateX();
    this->coordinateY = device->getCoordinateY();
}
void Device::setName(const QString name){
    this->name = name;
}
void Device::setAddress(const quint8 address){
    this->address = address;
}
void Device::setType(const quint8 type){
    this->type = type;
}
void Device::setValue(const int value){
    this->value = value;
}
void Device::setState(const quint8 state){
    this->state = state;
}
void Device::setCoordinateX(const int coordinateX){
    this->coordinateX = coordinateX;
}
void Device::setCoordinateY(const int coordinateY){
    this->coordinateY = coordinateY;
}
QString Device::getName() const{
    return name;
}
quint8 Device::getAddress() const{
    return address;
}
quint8 Device::getType() const{
    return type;
}
QString Device::getTypeString() const{
    return typeList.value(type, QString::number(type));
}
int Device::getValue() const{
    return value;
}
quint8 Device::getState() const{
    return state;
}
QString Device::getStateString() const{
    return stateList.value(state, QString::number(state));
}
int Device::getCoordinateX() const{
    return coordinateX;
}
int Device::getCoordinateY() const{
    return coordinateY;
}
QString Device::toString() const{
    QString str = "";
    str + "address  " + QString::number(address)
            + "type  " + QString::number(type)
            + "name  " + name
            + "value  " + QString::number(value)
            + "state  " + QString::number(state)
            + "coordinateX  " + QString::number(coordinateX)
            + "coordinateY  " + QString::number(coordinateY);
    return str;
}
QDomDocument Device::getDomDocument(){
    QDomDocument doc;
    QDomElement deviceElement = doc.createElement("device");
    QDomElement addressElement = doc.createElement("address");
    QDomElement valueElement = doc.createElement("value");
    QDomElement typeElement = doc.createElement("type");
    QDomElement nameElement = doc.createElement("name");
    QDomElement stateElement = doc.createElement("state");
    QDomElement coordinateXElement = doc.createElement("coordinateX");
    QDomElement coordinateYElement = doc.createElement("coordinateY");

    addressElement.appendChild(doc.createTextNode(QString::number(this->getAddress())));
    valueElement.appendChild(doc.createTextNode(QString::number(this->getValue())));
    typeElement.appendChild(doc.createTextNode(QString::number(this->getType())));
    nameElement.appendChild(doc.createTextNode(this->getName()));
    stateElement.appendChild(doc.createTextNode(QString::number(this->getState())));
    coordinateXElement.appendChild(doc.createTextNode(QString::number(this->getCoordinateX())));
    coordinateYElement.appendChild(doc.createTextNode(QString::number(this->getCoordinateY())));

    deviceElement.appendChild(addressElement);
    deviceElement.appendChild(valueElement);
    deviceElement.appendChild(typeElement);
    deviceElement.appendChild(nameElement);
    deviceElement.appendChild(stateElement);
    deviceElement.appendChild(coordinateXElement);
    deviceElement.appendChild(coordinateYElement);
    doc.appendChild(deviceElement);

    return doc;
}
Device Device::parseDomNode(const QDomNode domNode){
    QDomElement address = domNode.firstChildElement("address");
    QDomElement value = domNode.firstChildElement("value");
    QDomElement type = domNode.firstChildElement("type");
    QDomElement name = domNode.firstChildElement("name");
    QDomElement state = domNode.firstChildElement("state");
    QDomElement coordinateX = domNode.firstChildElement("coordinateX");
    QDomElement coordinateY = domNode.firstChildElement("coordinateY");
    QString str = coordinateX.text();
    if (!(address.isNull() || value.isNull() || type.isNull()|| name.isNull() || state.isNull() || coordinateX.isNull() || coordinateY.isNull())){
        return Device(static_cast<quint8>(address.text().toInt()),
                      static_cast<quint8>(type.text().toInt()),
                      name.text(),
                      value.text().toInt(),
                      static_cast<quint8>(state.text().toInt()),
                      coordinateX.text().toInt(),
                      coordinateY.text().toInt());
    }
    return Device();
}
