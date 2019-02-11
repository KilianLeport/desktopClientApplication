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
#include <QApplication>

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(MainWindow::logViewerWidgetTab != nullptr){
        QMetaObject::invokeMethod(MainWindow::logViewerWidgetTab, "logMessage", Qt::QueuedConnection,
                                  Q_ARG(QString, msg), Q_ARG(int, type));

    }
    // Call the default handler.
    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(customMessageOutput);
    //Q_INIT_RESOURCE(resource);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("K");
    QCoreApplication::setApplicationName("ApplicationClient");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    MainWindow w;
    w.show();

    return app.exec();
}
