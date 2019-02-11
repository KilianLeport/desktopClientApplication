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
#include "deviceeditingdialog.h"

DeviceEditingDialog::DeviceEditingDialog(Device *device, QString &title, QWidget *parent ) : QDialog(parent){
    this->device = device;
    QString address = QString::number(device->getAddress());
    if(device->getAddress() == 255)
        address = "";

    lineEditAddress = new QLineEdit(address, this);
    comboBoxType = new QComboBox(this);
    lineEditName = new QLineEdit(device->getName(), this);
    lineEditAddress->setValidator(new QIntValidator(0, Device::MAXIMUMADDRESS, this));
    comboBoxType->addItems(Device::typeList);
    if(device->getType() < Device::typeList.size())
        comboBoxType->setCurrentIndex(device->getType());

    spinBoxCoordinateX = new QSpinBox(parent);
    spinBoxCoordinateX->setMinimum(0);
    spinBoxCoordinateX->setMaximum(Device::MAXIMUMCOORDINATEX);
    spinBoxCoordinateX->setValue(device->getCoordinateX());
    spinBoxCoordinateY = new QSpinBox(parent);
    spinBoxCoordinateY->setMinimum(0);
    spinBoxCoordinateY->setMaximum(Device::MAXIMUMCOORDINATEY);
    spinBoxCoordinateY->setValue(device->getCoordinateY());

    QString regExpString = QString::number(Device::MAXIMUMSIZENAMEDEVICE) + "}";
    QValidator *v = new QRegExpValidator(QRegExp("[A-Za-z0-9 ]{0," + regExpString), this);
    lineEditName->setValidator(v);
    QObject::connect(lineEditAddress, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged()));
    QObject::connect(lineEditName, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged()));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(buttonOKClicked()));
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    this->textChanged(); //Actualize OK button state

    QGroupBox *formGroupBox = new QGroupBox(tr("Device edition"), this);

    QFormLayout *formLayout = new QFormLayout(this);
    formLayout->addRow(tr("&Address:"), lineEditAddress);
    formLayout->addRow(tr("&Type:"), comboBoxType);
    formLayout->addRow(tr("&Name:"), lineEditName);
    formLayout->addRow(tr("&Coordinate X:"), spinBoxCoordinateX);
    formLayout->addRow(tr("&Coordinate Y:"), spinBoxCoordinateY);
    formLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
    formGroupBox->setLayout(formLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(formGroupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(title);
    setMinimumSize(300,180);
}
void DeviceEditingDialog::textChanged(){
    if(!lineEditAddress->text().isEmpty())
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    else
        buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}
void DeviceEditingDialog::buttonOKClicked(){
    device->setAddress(static_cast<quint8>(lineEditAddress->text().toInt()));
    device->setType(static_cast<quint8>(comboBoxType->currentIndex()));
    device->setName(lineEditName->text());
    device->setCoordinateX(spinBoxCoordinateX->value());
    device->setCoordinateY(spinBoxCoordinateY->value());
    QDialog::accept();
}
