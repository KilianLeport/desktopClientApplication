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
#ifndef FIELDRENDERVIEWWIDGET_H
#define FIELDRENDERVIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>

#include "deviceitemmodel.h"
#include "connectionhandler.h"

class FieldRenderViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FieldRenderViewWidget(std::shared_ptr<DeviceItemModel> model, QWidget *parent = nullptr);

signals:
    void deviceMoved(const quint8 address);
private:
    int getDiameterOfCircle();
    QPoint getQPointOfDevice(const Device dev);
    quint8 getDeviceAddressAtThisPoint(const QPoint point);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    std::shared_ptr<DeviceItemModel> deviceItemModel;
    quint8 addressDeviceSelected;
    bool isDeviceSelected;
    int oldCoordinateX;
    int oldCoordinateY;
};

#endif // FIELDRENDERVIEWWIDGET_H
