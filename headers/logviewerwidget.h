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
#ifndef LOGVIEWERWIDGET_H
#define LOGVIEWERWIDGET_H

#include <QWidget>
#include <QMessageLogContext>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

class LogViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LogViewerWidget(QWidget *parent = nullptr);
    Q_INVOKABLE void logMessage(const QString &msg, const int type);
signals:

private:
    QPushButton *clearTextButton;
    QPlainTextEdit *plainTextEdit;
    QCheckBox *checkBoxDebug;
    QCheckBox *checkBoxWarning;
    QCheckBox *checkBoxInfo;
    QCheckBox *checkBoxCritical;
};

#endif // LOGVIEWERWIDGET_H
