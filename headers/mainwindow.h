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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QTabWidget>
#include <QToolBar>
#include <QMenuBar>
#include <QLabel>
#include <QApplication>
#include <QStatusBar>

#include "connectionhandler.h"
#include "device.h"
#include "logviewerwidget.h"
#include "devicelistwidget.h"
#include "fieldviewwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    static LogViewerWidget *logViewerWidgetTab;

private:
    void creationActions();
    void creationMenus();
    void creationCentralZone();
    void creationToolBar();
    void addTab(QWidget *widget, QAction *action, const QString &label);
    void closeTab(const int tab);
    void showHideTab(const bool checked);
    void connectionStateChanged(const int connectionTCPState);
    void acknowledgeReceived(Device::AckCode ackCode, const QVector<Device> deviceVector, const quint8 address, const Device::OrderCode orderCode);
    void readSettings();
    void writeSettings();
    void closeEvent(QCloseEvent *event) override;

private:
    //QActions
    QAction *actionExit;
    QAction *actionShowLogTab;
    QAction *actionShowListDeviceTab;
    QAction *actionShowFieldViewTab;
    //Menu Bar
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuView;
    //Central Zone
    QTabWidget *centralZoneWidget;
    DeviceListWidget *deviceListWidgetTab;
    FieldViewWidget *fieldViewWidgetTab;
    QHash<int, QWidget*> tabQwidgetHash;
    QHash<int, QString> tabQStringHash;
    QHash<QWidget*, QAction*> tabQWidgetQActionHash;
    QHash<QAction*, QWidget*> tabQActionQWidgetHash;
    QHash<QWidget*, QString> tabQWidgetQStringHash;
    //Tool Bar
    QToolBar *toolBar;
    QPushButton *connectionButton;
    QLabel *connectionLabel;

    //Model
    std::shared_ptr<DeviceItemModel> deviceItemModel;

    //network
    std::shared_ptr<ConnectionHandler> connectionHandler;
};

#endif // MAINWINDOW_H
