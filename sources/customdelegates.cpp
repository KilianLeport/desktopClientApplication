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
#include "customdelegates.h"

//***************StateDelegate********************************
StateDelegate::StateDelegate(QObject *parent)
    : QStyledItemDelegate(parent){
}
QString StateDelegate::displayText(const QVariant &value, const QLocale &/*locale*/) const{
    return Device::stateList.value(value.toInt(), QString::number(value.toInt()));
}
//***************TypeDelegate********************************
TypeDelegate::TypeDelegate(QObject *parent)
    : QStyledItemDelegate(parent){
}
QString TypeDelegate::displayText(const QVariant &value, const QLocale &/*locale*/) const{
    return Device::typeList.value(value.toInt(), QString::number(value.toInt()));
}
