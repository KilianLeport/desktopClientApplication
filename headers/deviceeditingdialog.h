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
#ifndef DEVICEEDITINGDIALOG_H
#define DEVICEEDITINGDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QIntValidator>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>

#include "device.h"

class DeviceEditingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceEditingDialog(Device *device, QString &title, QWidget *parent = nullptr);

private slots:
    void buttonOKClicked();
    void textChanged();

private:
    Device *device;
    QLineEdit *lineEditAddress;
    QLineEdit *lineEditName;
    QComboBox *comboBoxType;
    QSpinBox *spinBoxCoordinateX;
    QSpinBox *spinBoxCoordinateY;
    QDialogButtonBox *buttonBox;

};

#endif // DEVICEEDITINGDIALOG_H
