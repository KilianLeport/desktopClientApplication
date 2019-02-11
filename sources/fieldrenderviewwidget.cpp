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
#include "fieldrenderviewwidget.h"

FieldRenderViewWidget::FieldRenderViewWidget(std::shared_ptr<DeviceItemModel> model, QWidget *parent) : QWidget(parent)
{
    this->deviceItemModel = model;
    connect(deviceItemModel.get(), &QStandardItemModel::itemChanged, this, [this]{ update(); });
    connect(deviceItemModel.get(), &QStandardItemModel::rowsInserted, this, [this]{ update(); });
    connect(deviceItemModel.get(), &QStandardItemModel::rowsRemoved, this, [this]{ update(); });

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void FieldRenderViewWidget::paintEvent(QPaintEvent * /* event */)
{
    double rate = qSqrt(qPow(width(), 2)+qPow(height(), 2)) / 40;
    int diameter = static_cast<int>(rate);
    int margin = 5;
    int startAngle = 0;
    int arcLength = 360 * 16;

    QPainter painter(this);
    qDebug() << "paintevent radius: "<< QString::number(diameter) << "width: " << QString::number(width()) << "  height:  " << QString::number(height());

    DeviceItem *item;
    Device dev;
    for (int row = 0; row < deviceItemModel->rowCount(); ++row) {
        item = static_cast<DeviceItem*>(deviceItemModel->item(row, DeviceItemModel::ADDRESSCOLUMN));
        dev = item->getDevice();

        QPoint pDev = getQPointOfDevice(dev);
        int coordinateX = pDev.x();
        int coordinateY = pDev.y();

        QFont font = painter.font();
        font.setPixelSize(diameter/2);
        if(isDeviceSelected && addressDeviceSelected == dev.getAddress())
            font.setBold(true);
        else
            font.setBold(false);
        painter.setFont(font);

        QFontMetrics fm(painter.font());
        int textWidth = fm.width(dev.getName());
        int coordinateTextX = coordinateX + diameter + margin;
        int coordinateTextY = coordinateY;
        if((coordinateTextX + textWidth) >= width())
            coordinateTextX = coordinateX - margin - textWidth;


        painter.setPen(Qt::NoPen);
        if(dev.getState() == Device::StateCode::On)
            painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        else
            painter.setBrush(QBrush(Qt::red, Qt::SolidPattern));

        QRect rect(coordinateX, coordinateY, diameter, diameter);
        painter.drawPie(rect, startAngle, arcLength);

        painter.setPen(Qt::SolidLine);
        QRect rectText(coordinateTextX, coordinateTextY, textWidth, diameter);
        painter.drawText(rectText,
                         Qt::AlignCenter,
                         dev.getName());
    }

}
int FieldRenderViewWidget::getDiameterOfCircle(){
    double rate = qSqrt(qPow(width(), 2)+qPow(height(), 2)) / 40;
    return static_cast<int>(rate);
}
QPoint FieldRenderViewWidget::getQPointOfDevice(const Device dev){
    int coordinateX = dev.getCoordinateX();
    int coordinateY = dev.getCoordinateY();
    int diameter = getDiameterOfCircle();
    coordinateX = (coordinateX*width())/Device::MAXIMUMCOORDINATEX;
    coordinateY = (coordinateY*height())/Device::MAXIMUMCOORDINATEY;

    if((coordinateX + diameter) >= width())
        coordinateX = width() - diameter;
    if((coordinateY + diameter) >= height())
        coordinateY = height() - diameter;
    return QPoint(coordinateX, coordinateY);
}

quint8 FieldRenderViewWidget::getDeviceAddressAtThisPoint(const QPoint point){
    DeviceItem *item;
    Device dev;
    for (int row = 0; row < deviceItemModel->rowCount(); ++row) {
        item = static_cast<DeviceItem*>(deviceItemModel->item(row, DeviceItemModel::ADDRESSCOLUMN));
        dev = item->getDevice();

        QPoint pDev = getQPointOfDevice(dev);
        int d = static_cast<int>(qSqrt(qPow(qFabs(pDev.x() - point.x()), 2) + qPow(qFabs(pDev.y() - point.y()), 2)));
        if (d <= getDiameterOfCircle())
            return dev.getAddress();
    }
    return 255;
}
void FieldRenderViewWidget::mousePressEvent(QMouseEvent *event)
{
    addressDeviceSelected = getDeviceAddressAtThisPoint(QPoint(event->x(), event->y()));
    if(addressDeviceSelected != 255)
        isDeviceSelected = true;
    update();
    //deviceSelected = nullptr;
    qDebug() << "mousePressEvent x: "<< QString::number(event->x()) << "y: " << QString::number(event->y());
    qDebug() << "address: "<< QString::number(addressDeviceSelected);
}

void FieldRenderViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isDeviceSelected){
        DeviceItem *item = deviceItemModel->getDeviceItemFromAddress(addressDeviceSelected);
        Device dev = item->getDevice();
        int x = oldCoordinateX;
        int y = oldCoordinateY;
        bool deviceMoved = false;
        if(event->x() >= 0 && event->x() <= width())
            x = (Device::MAXIMUMCOORDINATEX*event->x())/width();
        if(x != oldCoordinateX && x <= Device::MAXIMUMCOORDINATEX){
            oldCoordinateX = x;
            dev.setCoordinateX(x);
            deviceMoved = true;
        }
        if(event->y() >= 0 && event->y() <= height())
            y = (Device::MAXIMUMCOORDINATEY*event->y())/height();
        if(y != oldCoordinateY && y <= Device::MAXIMUMCOORDINATEY){
            oldCoordinateY = y;
            dev.setCoordinateY(y);
            deviceMoved = true;
        }

        if(deviceMoved){
            deviceItemModel->addModifyDevice(dev);
        }
    }
    qDebug() << "mouseMoveEvent x: "<< QString::number(event->x()) << "y: " << QString::number(event->y());
}
void FieldRenderViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    emit deviceMoved(addressDeviceSelected);
    addressDeviceSelected = 255;
    isDeviceSelected = false;
    update();
    qDebug() << "mouseReleaseEvent x: "<< QString::number(event->x()) << "y: " << QString::number(event->y());
}

