/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QLoggingCategory>


void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString & msg)
{
    static bool isOpen = false;
    static FILE* pfile;
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1-%2-%3-%4-%5").arg(context.category).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1-%2-%3-%4-%5").arg(context.category).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        break;
    case QtInfoMsg:
        txt = QString("Info: %1-%2-%3-%4-%5").arg(context.category).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1-%2-%3-%4-%5").arg(context.category).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1-%2-%3-%4-%5").arg(context.category).arg(context.file).arg(context.line).arg(context.function).arg(msg);
        break;
    }


    if (!isOpen)
    {
        QDateTime time = QDateTime::currentDateTime();
        QString strBuffer = time.toString("yyyy-MM-dd-hh-mm-ss");
        strBuffer += QStringLiteral(".log");

        pfile = fopen(qPrintable(strBuffer),"w");
        //static QFile outFile(strBuffer);
        //outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        //static QTextStream ts(&outFile);
        isOpen = true;
    }
    //QTextStream ts(&outFile);
    //ts << txt << endl;
    //fprintf(pfile, qPrintable(txt));
    fputs(qPrintable(txt), pfile);
    fputc('\n', pfile);
}

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.debug=true\n"
                                                    "driver.usb.debug=true\n"
                                                    "driver.usb.low=true\n"
                                                    "drone.engine.debug=true"));
    qSetMessagePattern(QStringLiteral("[%{type}] %{time} %{appname} (%{file}:%{line}) - %{message}"));
    //qInstallMessageHandler(myMessageHandler);
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
    return a.exec();
}
