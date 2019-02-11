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
#include "connectionhandler.h"

ConnectionHandler::ConnectionHandler(QObject *parent) : QObject(parent){
    initialisationSockets();
}
void ConnectionHandler::initialisationSockets(){
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)){
            ipAddressComputer = address;
            qDebug() << "address:   " << address.toString();
        }
    }
    changeConnectionState(Disconnected);
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(tcpReadyRead()));
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(ipAddressComputer, 10102);
    udpTimerAnswer = new QTimer(this);
    udpTimerAnswer->setSingleShot(true);
    connect(udpTimerAnswer, SIGNAL(timeout()), this, SLOT(timerOut()));
}
void ConnectionHandler::tcpConnection(const QHostAddress &address, const quint16 &port){
    tcpSocket->abort();
    tcpSocket->connectToHost(address, port);
}
void ConnectionHandler::connectionToServer(){
    changeConnectionState(Pending);
    connect(udpSocket, &QAbstractSocket::readyRead, this, &ConnectionHandler::udpReadyRead);
    udpTimerAnswer->start(2000);
    qDebug() << "Connection pending... ";

    QByteArray code(CODE_CLIENT);
    udpSocket->writeDatagram(code, QHostAddress::Broadcast, PORT_SERVER);
}
void ConnectionHandler::changeConnectionState(const ConnectionState newConnectionTCPState){
    connectionTCPState = newConnectionTCPState;
    emit connectionStateChanged(connectionTCPState);
}
void ConnectionHandler::sendData(const Device::OrderCode orderCode,
                                 const quint8 address,
                                 const quint8 action,
                                 const quint8 type,
                                 const QString name,
                                 const int coordinateX,
                                 const int coordinateY){
    QDomDocument doc;
    QDomElement orderElement = doc.createElement("order");

    QDomElement orderCodeElement = doc.createElement("orderCode");
    QDomElement addressElement = doc.createElement("address");
    QDomElement nameElement = doc.createElement("name");
    QDomElement typeElement = doc.createElement("type");
    QDomElement actionElement = doc.createElement("action");
    QDomElement coordinateXElement = doc.createElement("coordinateX");
    QDomElement coordinateYElement = doc.createElement("coordinateY");

    orderCodeElement.appendChild(doc.createTextNode(QString::number(orderCode)));
    addressElement.appendChild(doc.createTextNode(QString::number(address)));

    doc.appendChild(orderElement);
    orderElement.appendChild(orderCodeElement);
    orderElement.appendChild(addressElement);

    switch(orderCode){
    case Device::OrderCode::AddModify:
        nameElement.appendChild(doc.createTextNode(name));
        typeElement.appendChild(doc.createTextNode(QString::number(type)));
        coordinateXElement.appendChild(doc.createTextNode(QString::number(coordinateX)));
        coordinateYElement.appendChild(doc.createTextNode(QString::number(coordinateY)));

        orderElement.appendChild(nameElement);
        orderElement.appendChild(typeElement);
        orderElement.appendChild(coordinateXElement);
        orderElement.appendChild(coordinateYElement);
        break;
    case Device::OrderCode::Delete:
        break;
    case Device::OrderCode::Read:
        break;
    case Device::OrderCode::Action:
        actionElement.appendChild(doc.createTextNode(QString::number(action)));
        orderElement.appendChild(actionElement);
        break;
    default:
        break;
    }
    const QByteArray data = doc.toByteArray();
    const int size = data.size();
    QByteArray sizeArray;
    QDataStream streamSize(&sizeArray, QIODevice::WriteOnly);
    streamSize.setVersion(QDataStream::Qt_5_10);
    streamSize << size;

    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_5_10);
    stream.writeRawData(sizeArray, sizeof(int));
    stream.writeRawData(data, size);

    if(connectionTCPState == Connected){
        qInfo() << "sendData() Send to server orderCode :" << QString::number(orderCode) << " order: " << QString::number(address);
        tcpSocket->write(block);
    }
}
void ConnectionHandler::handleFrame(const QByteArray &frame){
    qDebug() << "handleFrame()";
    QVector<Device> deviceVector;
    QDomDocument doc;
    doc.setContent(frame);

    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    if(docElem.nodeName().compare("ack") == 0 && n.nodeName().compare("ackCode") == 0){
        QDomElement e = n.toElement();
        QDomNodeList device = doc.elementsByTagName("device");
        if(!e.isNull() && !device.isEmpty()) {
            quint8 ackCode = static_cast<quint8>(e.text().toInt());
            QDomElement addressElement = device.item(0).firstChildElement("address");
            QDomElement orderCodeElement = device.item(0).firstChildElement("orderCode");
            if(!addressElement.isNull()){
                quint8 address = static_cast<quint8>(addressElement.text().toInt());
                if(ackCode == Device::AckCode::OKread){
                    bool frameValid = true;

                    for (int i = 0; i < device.size(); i++) {
                        Device dev = Device::parseDomNode(device.item(i));
                        if(dev.getAddress() == 255)
                            frameValid = false;
                        else
                            deviceVector.push_back(dev);
                    }
                    if(frameValid){
                        emit frameTCPAnswerAck((Device::AckCode) ackCode, deviceVector, address);
                    }else{
                        qCritical() << tr("Ack OK read: error");
                        deviceVector.clear();
                        emit frameTCPAnswerAck(Device::AckCode::InvalidFrameFromServer, deviceVector);
                    }
                }else{
                    if(!orderCodeElement.isNull()){
                        quint8 orderCode = static_cast<quint8>(orderCodeElement.text().toInt());
                        emit frameTCPAnswerAck((Device::AckCode) ackCode, deviceVector, address, (Device::OrderCode) orderCode);
                    }else{
                        qCritical() << tr("Ack OK read: error");
                        deviceVector.clear();
                        emit frameTCPAnswerAck(Device::AckCode::InvalidFrameFromServer, deviceVector);
                    }
                }
            }else
                emit frameTCPAnswerAck(Device::AckCode::InvalidFrameFromServer, deviceVector);
        }else{
            qCritical() << "Frame invalid from server";
            emit frameTCPAnswerAck(Device::AckCode::InvalidFrameFromServer, deviceVector);
        }
    }
}
//******************************private slots***********************************
void ConnectionHandler::udpReadyRead(){
    if(udpTimerAnswer->isActive()){
        qDebug() << "Answer received.... ";
        QByteArray datagram;
        datagram.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        udpSocket->readDatagram(datagram.data(), datagram.size(),&sender, &senderPort);
        qDebug() << "Message size: " << datagram.size();
        qDebug() << "Message from: " << sender.toString();
        qDebug() << "Message port: " << senderPort;
        qDebug() << "Message: " << datagram;
        QByteArray codeServer(CODE_SERVER);
        QByteArray code;
        QDataStream stream(&datagram, QIODevice::ReadOnly);
        stream >> code;
        if(code == codeServer){
            quint32 adresse(0);
            quint16 port(0);
            stream >> adresse >> port;
            QHostAddress adresseTCP(adresse);
            qDebug() << "address" << adresseTCP.toString() << "   port   " << port;
            tcpConnection(adresseTCP, port);
        }
    }
}
void ConnectionHandler::tcpReadyRead(){
    while (tcpSocket->bytesAvailable() > 0)
    {
        bufferSocket.append(tcpSocket->readAll());
        while(bufferSocket.size() >= 4){
            if (sizeBufferSocket == 0 && bufferSocket.size() >= 4){ //if size of data has received completely, store it on our global variable
                QByteArray source = bufferSocket.mid(0, 4);
                QDataStream stream(&source, QIODevice::ReadOnly);
                stream >> sizeBufferSocket;
                bufferSocket.remove(0, 4);
            }
            if (sizeBufferSocket > 0 && bufferSocket.size() >= sizeBufferSocket){ // If data has received completely
                QByteArray data = bufferSocket.mid(0, sizeBufferSocket);
                bufferSocket.remove(0, sizeBufferSocket);
                sizeBufferSocket = 0;
                handleFrame(data);
            }
        }
    }
}
void ConnectionHandler::connected(){
    changeConnectionState(Connected);
    udpTimerAnswer->stop();
    bufferSocket.clear();
    sizeBufferSocket = 0;
    qInfo() << "connected()  : connected ";
}
void ConnectionHandler::disconnected(){
    changeConnectionState(Disconnected);
    bufferSocket.clear();
    sizeBufferSocket = 0;
    qInfo() << "disconnected()  : disconnected ";
}
void  ConnectionHandler::timerOut(){
    qDebug() << "timer out!" ;
    if(connectionTCPState == Pending)
        changeConnectionState(Disconnected);
    disconnect(udpSocket, &QAbstractSocket::readyRead, this, &ConnectionHandler::udpReadyRead);
}
void ConnectionHandler::socketError(const QAbstractSocket::SocketError error){
    switch(error)
    {
    case QAbstractSocket::HostNotFoundError:
        qCritical() << tr("Server not found. Check the address and the port.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qCritical() << tr("Server refused the connection. Check the address and the port.");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qCritical() << tr("Server cut the connection.");
        break;
    default:
        qCritical() << tcpSocket->errorString();
        break;
    }
}
