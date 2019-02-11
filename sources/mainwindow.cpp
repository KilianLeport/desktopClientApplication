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
#include "mainwindow.h"

LogViewerWidget * MainWindow::logViewerWidgetTab = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent){

    connectionHandler = std::make_shared<ConnectionHandler>();
    //TODO improve: what hapen when connectionHandler goes out of scope and the pointer is destroyed
    connect(connectionHandler.get(), &ConnectionHandler::connectionStateChanged, this, &MainWindow::connectionStateChanged);
    connect(connectionHandler.get(), &ConnectionHandler::frameTCPAnswerAck, this, &MainWindow::acknowledgeReceived);

    deviceItemModel = std::make_shared<DeviceItemModel>();

    creationActions();
    creationMenus();
    creationCentralZone();
    creationToolBar();

    //properties of the main window
    setWindowIcon(QIcon(":/icons/lightning.svg"));
    setWindowTitle(tr("Client Surveillance"));

    readSettings();
    connectionHandler->connectionToServer();
}
void MainWindow::creationActions(){
    //QActions
    actionExit = new QAction(tr("&Quit"), this);
    actionExit->setShortcut(QKeySequence("Ctrl+Q"));
    actionExit->setIcon(QIcon(":/icons/quit.svg"));
    connect(actionExit, &QAction::triggered, this, &MainWindow::close);
    actionShowLogTab = new QAction(tr("Show &log"), this);
    actionShowLogTab->setCheckable(true);
    actionShowLogTab->setChecked(true);
    actionShowLogTab->setShortcut(QKeySequence("Ctrl+L"));
    connect(actionShowLogTab, &QAction::triggered, this, &MainWindow::showHideTab);
    actionShowListDeviceTab = new QAction(tr("Show &VirtualField"), this);
    actionShowListDeviceTab->setCheckable(true);
    actionShowListDeviceTab->setChecked(true);
    actionShowListDeviceTab->setShortcut(QKeySequence("Ctrl+V"));
    connect(actionShowListDeviceTab, &QAction::triggered, this, &MainWindow::showHideTab);
    actionShowFieldViewTab = new QAction(tr("Show &Field View"), this);
    actionShowFieldViewTab->setCheckable(true);
    actionShowFieldViewTab->setChecked(true);
    actionShowFieldViewTab->setShortcut(QKeySequence("Ctrl+F"));
    connect(actionShowFieldViewTab, &QAction::triggered, this, &MainWindow::showHideTab);
}
void MainWindow::creationMenus(){
    //QMenus
    menuFile = menuBar()->addMenu(tr("&File"));
    menuFile->addAction(actionExit);
    menuView = menuBar()->addMenu(tr("&View"));
    menuView->addAction(actionShowListDeviceTab);
    menuView->addAction(actionShowLogTab);
    menuView->addAction(actionShowFieldViewTab);
}
void MainWindow::creationCentralZone(){
    centralZoneWidget = new QTabWidget(this);

    deviceListWidgetTab = new DeviceListWidget(deviceItemModel, connectionHandler, this);
    fieldViewWidgetTab = new FieldViewWidget(deviceItemModel, connectionHandler, this);
    logViewerWidgetTab = new LogViewerWidget(this);

    centralZoneWidget->setObjectName("centralZoneWidget");
    logViewerWidgetTab->setObjectName("logViewerWidgetTab");
    deviceListWidgetTab->setObjectName("deviceListWidgetTab");
    fieldViewWidgetTab->setObjectName("fieldViewWidgetTab");

    addTab(deviceListWidgetTab, actionShowListDeviceTab, tr("Device List"));
    addTab(fieldViewWidgetTab, actionShowFieldViewTab, tr("Field view"));
    addTab(logViewerWidgetTab, actionShowLogTab, tr("log"));

    connect(centralZoneWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    centralZoneWidget->setMovable(true);
    centralZoneWidget->setTabsClosable(true);

    setCentralWidget(centralZoneWidget);
}
void MainWindow::creationToolBar(){
    toolBar = addToolBar(tr("File"));

    connectionLabel = new QLabel("", this);

    connectionButton = new QPushButton("Connection", this);
    connect(connectionButton, &QPushButton::clicked, connectionHandler.get(), &ConnectionHandler::connectionToServer);

    toolBar->addAction(actionExit);
    toolBar->addSeparator();
    toolBar->addWidget(connectionButton);
    toolBar->addWidget(connectionLabel);
}
void MainWindow::addTab(QWidget *widget, QAction *action, const QString &label){
    tabQWidgetQActionHash.insert(widget, action);
    tabQActionQWidgetHash.insert(action, widget);
    tabQWidgetQStringHash.insert(widget, label);
    centralZoneWidget->addTab(widget, label);
}
void MainWindow::closeTab(const int tab){
    tabQWidgetQActionHash.value(centralZoneWidget->widget(tab))->setChecked(false);
    centralZoneWidget->removeTab(tab);
}
void MainWindow::showHideTab(const bool checked){
    QAction *action = qobject_cast<QAction *>(sender());
    QWidget *widget = tabQActionQWidgetHash.value(action);
    if(checked){
        centralZoneWidget->addTab(widget, tabQWidgetQStringHash.value(widget));
    }else{
        centralZoneWidget->removeTab(centralZoneWidget->indexOf(widget));
    }
}
void MainWindow::connectionStateChanged(const int connectionTCPState){
    switch( connectionTCPState ) {
    case ConnectionHandler::Connected :
        connectionLabel->setText(tr(" Connected"));
        connectionButton->setEnabled(false);
        connectionHandler->sendData(Device::OrderCode::Read, static_cast<quint8>(Device::ADDRESSBROADCAST));
        break;
    case ConnectionHandler::Pending :
        connectionLabel->setText(tr(" Pending..."));
        connectionButton->setEnabled(false);
        break;
    case ConnectionHandler::Disconnected :
        connectionLabel->setText(tr(" Disconnected"));
        connectionButton->setEnabled(true);
        break;
    default :
        connectionLabel->setText(tr(" Error"));
        connectionButton->setEnabled(false);
        break;
    }
}
void MainWindow::acknowledgeReceived(const Device::AckCode ackCode, const QVector<Device> deviceVector, const quint8 address, const Device::OrderCode orderCode){
    qInfo() << "acknowledgeReceived() ackCode: " << QString::number(ackCode) << " address: " << QString::number(address);
    if(ackCode == Device::AckCode::OKread){
        for (int i = 0; i < deviceVector.size(); ++i) {
            deviceItemModel->addModifyDevice(deviceVector.at(i));
        }
    }else{
        Device dev;
        DeviceItem *item;
        switch( orderCode ) {
        case Device::OrderCode::Action:
            switch( ackCode ) {
            case Device::AckCode::OK :
                connectionHandler->sendData(Device::OrderCode::Read, static_cast<quint8>(address));
                break;
            case Device::AckCode::UnreachedDevice :
            case Device::AckCode::InvalidAddress :
            case Device::AckCode::UnknownAddress :
                if(deviceItemModel->hasAddress(address)){
                    item = deviceItemModel->getDeviceItemFromAddress(address);
                    dev = item->getDevice();
                    dev.setState(Device::StateCode::Unreachable);
                    deviceItemModel->addModifyDevice(dev);
                }
                break;
            case Device::AckCode::InvalidName :
            case Device::AckCode::InvalidFrameFromClient :
            case Device::AckCode::InvalidFrameFromServer :
                break;
            default :
                qCritical() << "unknown ack !!!!! ";
                break;
            }
            break;
        case Device::OrderCode::AddModify:
            switch( ackCode ) {
            case Device::AckCode::OK :
                connectionHandler->sendData(Device::OrderCode::Read, static_cast<quint8>(address));
                break;
            case Device::AckCode::InvalidAddress :
            case Device::AckCode::UnknownAddress :
            case Device::AckCode::InvalidName :
            case Device::AckCode::UnreachedDevice :
            case Device::AckCode::InvalidFrameFromClient :
            case Device::AckCode::InvalidFrameFromServer :
                break;
            default :
                qCritical() << "unknown ack !!!!! ";
                break;
            }
            break;
        case Device::OrderCode::Delete:
            switch( ackCode ) {
            case Device::AckCode::OK :
            case Device::AckCode::InvalidAddress :
            case Device::AckCode::UnknownAddress :
                if(deviceItemModel->hasAddress(address)){
                    item = deviceItemModel->getDeviceItemFromAddress(address);
                    deviceItemModel->removeRow(item->row());
                }
                break;
            case Device::AckCode::InvalidName :
            case Device::AckCode::UnreachedDevice :
            case Device::AckCode::InvalidFrameFromClient :
            case Device::AckCode::InvalidFrameFromServer :
                break;
            default :
                qCritical() << "unknown ack !!!!! ";
                break;
            }
            break;
        case Device::OrderCode::UnknownCode:
            break;
        default:
            break;
        }
    }
}
void MainWindow::readSettings(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    }
    if(settings.childGroups().contains(centralZoneWidget->objectName())){
        settings.beginGroup(centralZoneWidget->objectName());
        if(settings.value(deviceListWidgetTab->objectName()).toInt() >= 0)
            centralZoneWidget->tabBar()->moveTab(centralZoneWidget->indexOf(deviceListWidgetTab), settings.value(deviceListWidgetTab->objectName()).toInt());
        else{
            centralZoneWidget->removeTab(centralZoneWidget->indexOf(deviceListWidgetTab));
            tabQWidgetQActionHash.value(deviceListWidgetTab)->setChecked(false);
        }
        if(settings.value(logViewerWidgetTab->objectName()).toInt() >= 0)
            centralZoneWidget->tabBar()->moveTab(centralZoneWidget->indexOf(logViewerWidgetTab), settings.value(logViewerWidgetTab->objectName()).toInt());
        else{
            centralZoneWidget->removeTab(centralZoneWidget->indexOf(logViewerWidgetTab));
            tabQWidgetQActionHash.value(logViewerWidgetTab)->setChecked(false);
        }
        centralZoneWidget->setCurrentIndex(settings.value("currentindex").toInt());
        settings.endGroup();
    }
}
void MainWindow::writeSettings(){
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());

    settings.beginGroup(centralZoneWidget->objectName());
    settings.setValue(deviceListWidgetTab->objectName(), centralZoneWidget->indexOf(deviceListWidgetTab));
    settings.setValue(logViewerWidgetTab->objectName(), centralZoneWidget->indexOf(logViewerWidgetTab));
    settings.setValue("currentindex", centralZoneWidget->currentIndex());
    settings.endGroup();
}
void MainWindow::closeEvent(QCloseEvent *event){
    writeSettings();
    QMainWindow::closeEvent(event);
}
