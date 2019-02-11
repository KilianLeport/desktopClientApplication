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
#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include <QtNetwork>
#include <QtXml>

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

#include "device.h"

class ConnectionHandler : public QObject
{
    Q_OBJECT

public:
    enum ConnectionState : int
    {
        Disconnected,
        Pending,
        Connected
    };

public:
    explicit ConnectionHandler(QObject *parent = nullptr);
    void sendData(const Device::OrderCode orderCode, const quint8 address = 255, const quint8 action = 0, const quint8 type = 0, const QString name = QString(), const int coordinateX = 0, const int coordinateY = 0);

signals:
    void connectionStateChanged(const int connectionTCPState);
    void frameTCPAnswerAck(const Device::AckCode ackCode, const QVector<Device> deviceVector, const quint8 address = 255, const Device::OrderCode orderCode = Device::OrderCode::UnknownCode);

public slots:
    void connectionToServer();

private slots:
    void udpReadyRead();
    void timerOut();
    void tcpReadyRead();
    void connected();
    void disconnected();
    void socketError(const QAbstractSocket::SocketError error);

private:
    void initialisationSockets();
    void tcpConnection(const QHostAddress &address, const quint16 &port);
    void changeConnectionState(const ConnectionState newConnectionTCPState);
    void handleFrame(const QByteArray &frame);

private:
    //network
    QTcpSocket *tcpSocket;
    QUdpSocket *udpSocket;
    QHostAddress ipAddressComputer;
    static const int PORT_SERVER = 10101;
    QTimer *udpTimerAnswer;
    ConnectionState connectionTCPState;

    QByteArray bufferSocket;
    int sizeBufferSocket;
    const char* CODE_CLIENT = "CODECLIENT";
    const char* CODE_SERVER = "CODESERVER";
};

#endif // CONNECTIONHANDLER_H
