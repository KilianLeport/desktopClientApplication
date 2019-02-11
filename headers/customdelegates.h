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
#ifndef CUSTOMDELEGATES_H
#define CUSTOMDELEGATES_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QCheckBox>
#include <QPainter>
#include <QApplication>
#include <QItemEditorFactory>
#include <QDebug>

#include "device.h"

class StateDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    StateDelegate(QObject *parent = nullptr);
    QString displayText(const QVariant &value, const QLocale &) const override;
};

class TypeDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TypeDelegate(QObject *parent = nullptr);
    QString displayText(const QVariant &value, const QLocale &) const override;
};
#endif // CUSTOMDELEGATES_H
