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
#include "logviewerwidget.h"

LogViewerWidget::LogViewerWidget(QWidget *parent) : QWidget(parent){
    plainTextEdit = new QPlainTextEdit(this);
    clearTextButton = new QPushButton(tr("Clear"), this);
    QObject::connect(clearTextButton, SIGNAL(clicked()), plainTextEdit, SLOT(clear()));

    checkBoxDebug = new QCheckBox(tr("Debug"), this);
    checkBoxInfo = new QCheckBox(tr("Info"), this);
    checkBoxWarning = new QCheckBox(tr("Warning"), this);
    checkBoxCritical = new QCheckBox(tr("Critical"), this);
    checkBoxDebug->setChecked(true);
    checkBoxInfo->setChecked(true);
    checkBoxWarning->setChecked(true);
    checkBoxCritical->setChecked(true);

    QHBoxLayout *layoutCheckbox = new QHBoxLayout();
    layoutCheckbox->addWidget(checkBoxDebug);
    layoutCheckbox->addWidget(checkBoxInfo);
    layoutCheckbox->addWidget(checkBoxWarning);
    layoutCheckbox->addWidget(checkBoxCritical);
    layoutCheckbox->addStretch();

    QHBoxLayout *layoutButtonDebug = new QHBoxLayout();
    layoutButtonDebug->addWidget(clearTextButton);
    layoutButtonDebug->addStretch();

    QVBoxLayout *layoutDebug = new QVBoxLayout();
    layoutDebug->addWidget(plainTextEdit);
    layoutDebug->addLayout(layoutCheckbox);
    layoutDebug->addLayout(layoutButtonDebug);

    this->setLayout(layoutDebug);
}
void LogViewerWidget::logMessage(const QString &msg, const int type){
    QString formatedMessage = "";
    switch (type) {
    case QtDebugMsg:
        if(checkBoxDebug->isChecked()){
            formatedMessage.append("<font color=\"blue\"><strong>Debug:</strong> " + msg + "</font>");
            plainTextEdit->appendHtml(formatedMessage);
        }
        break;
    case QtInfoMsg:
        if(checkBoxInfo->isChecked()){
            formatedMessage.append("<font color=\"black\"><strong>Info:</strong> " + msg + "</font>");
            plainTextEdit->appendHtml(formatedMessage);
        }
        break;
    case QtWarningMsg:
        if(checkBoxWarning->isChecked()){
            formatedMessage.append("<font color=\"green\"><strong>Warning:</strong> " + msg + "</font>");
            plainTextEdit->appendHtml(formatedMessage);
        }
        break;
    case QtCriticalMsg:
        if(checkBoxCritical->isChecked()){
            formatedMessage.append("<font color=\"red\"><strong>Critical:</strong> " + msg + "</font>");
            plainTextEdit->appendHtml(formatedMessage);
        }
        break;
    case QtFatalMsg:
        abort();
    }
}
