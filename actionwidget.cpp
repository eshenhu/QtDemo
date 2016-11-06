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
#include "actionwidget.h"
#include <QtWidgets/QPushButton>

#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>

#include <QtGlobal>
#include <QtWidgets/QMessageBox>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>
#include <QProgressDialog>
#include <QSerialPortInfo>

#include "ui/compqchartwidget.h"
#include "ui/configtab.h"
#include "ui/testtab.h"
#include "cfg/cfgreshandler.h"
#include "cfg/cfgjsonprimaryelement.h"
#include "driver/automationmodeldriverclz.h"

QT_CHARTS_USE_NAMESPACE

ActionWidget::ActionWidget(QWidget *parent)
    : QWidget(parent)
{
    m_measData.type = JsonGUIPrimType::INVALID;
    m_cfgHandler = new CfgResHandler();
    m_reader = new CfgJsonReader();
    m_reader->load("PV1");

    createTabWidget();
    //createChartView();
    m_chartWidget = new CompQChartWidget(m_reader, this);

    QHBoxLayout *baseLayout = new QHBoxLayout();
    baseLayout->addWidget(m_tabWidget, 0);
    baseLayout->addWidget(m_chartWidget, 1);
    //baseLayout->addWidget(m_chartView, 1);
    setLayout(baseLayout);

    m_driver = new AutomationModelDriverClz(this);

    connect(m_driver, &AutomationModelDriverClz::updateData, m_chartWidget, &CompQChartWidget::updateData);
    connect(m_subTestTabWidget->start_btn(), &QPushButton::clicked, [this](bool checked){
        QSerialPortSetting::Settings setting = this->doAutoSelectSerialPlugInPort();
        if (setting.name.isEmpty()){
            QMessageBox::warning(this, tr("Warning"),
                                 tr("Scanning serial port failed \n"
                                    "Please check your cable connection status"),
                                 QMessageBox::Ok);
            return;
        }

        if (m_measData.type == JsonGUIPrimType::INVALID){
            QMessageBox::warning(this, tr("Warning"), tr("No available selection\n"
                                                         "Please make your selection"),
                                 QMessageBox::Ok);
            return;
        }
        m_driver->startMeasTest(m_measData, setting);
        //reset.
        m_measData.type = JsonGUIPrimType::INVALID;
    });


//    updateSerieSettings();
//    updateChartSettings();
}

ActionWidget::~ActionWidget()
{
    if(m_cfgHandler)
        delete m_cfgHandler;
    if (m_reader)
        delete m_reader;
    if (m_chartWidget)
        delete m_chartWidget;
}

void ActionWidget::createTabWidget()
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->setTabPosition(QTabWidget::North);
    m_tabWidget->setTabBarAutoHide(true);

    m_subTestTabWidget = new TestTab();
    connect(m_subTestTabWidget, SIGNAL(updateUserSelection(VoltageTstData)),
            this, SLOT(updateUserInput(VoltageTstData)));
    connect(m_subTestTabWidget, SIGNAL(updateUserSelection(ThrottleTstData)),
            this, SLOT(updateUserInput(ThrottleTstData)));

    m_tabWidget->addTab(m_subTestTabWidget, tr("Test"));
    m_subConfigTabWidget = new ConfigTab(m_cfgHandler);
    m_tabWidget->addTab(m_subConfigTabWidget, tr("Config"));
}

//void MainWidget::updateChartSettings()
//{
//    QChart::ChartTheme theme = (QChart::ChartTheme) m_themeComboBox->itemData(m_themeComboBox->currentIndex()).toInt();
//    m_chartView->chart()->setTheme(theme);
//    m_chartView->setRenderHint(QPainter::Antialiasing, m_aaCheckBox->isChecked());

//    if (m_animationsCheckBox->checkState() == Qt::Checked)
//        m_chartView->chart()->setAnimationOptions(QChart::AllAnimations);
//    else
//        m_chartView->chart()->setAnimationOptions(QChart::NoAnimation);

//    if (m_legendCheckBox->checkState() == Qt::Checked)
//        m_chartView->chart()->legend()->show();
//    else
//        m_chartView->chart()->legend()->hide();
//}


const CfgJsonReader *ActionWidget::reader() const
{
    return m_reader;
}

void ActionWidget::updateUserInput(VoltageTstData data)
{
    m_measData.type = JsonGUIPrimType::VOLTAGE;
    m_measData.data.u = data;
}

void ActionWidget::updateUserInput(ThrottleTstData data)
{
    m_measData.type = JsonGUIPrimType::THROTTLE;
    m_measData.data.v = data;
}

QSerialPortSetting::Settings ActionWidget::doAutoSelectSerialPlugInPort()
{
    QSerialPortSetting::Settings p;

    QSerialPort* serial = new QSerialPort();
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);

    const auto infos = QSerialPortInfo::availablePorts();

    QProgressDialog progress(tr("Scanning serial port ..."), tr("Abort scanning"), 0, infos.size(), this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(4000);

    int i = 0;
    bool found = false;
    for (const QSerialPortInfo &info : infos)
    {
        progress.setValue(i++);

        if (progress.wasCanceled())
            break;

        p.name = info.portName();
        serial->setPortName(p.name);

        if (serial->open(QIODevice::ReadWrite)) {

            //F0 CC 09 00 F6 FF 07 00 00 00 00 FF FF FF FF XX XX
            do{
                //F0 CC 01 00 FE FF 05 7F 43
                // send Reset for ACK.
                QByteArray writeData;
                writeData.resize(9);
                writeData[0] = 0xF0;
                writeData[1] = 0xCC;
                writeData[2] = 0x01;
                writeData[3] = 0x00;
                writeData[4] = 0xFE;
                writeData[5] = 0xFF;
                writeData[6] = 0x05;
                writeData[7] = 0x7F;
                writeData[8] = 0x43;

                qint64 bytesWritten = serial->write(writeData);

                if (bytesWritten == -1) {
                    qDebug() << "Failed to write the data to port" << p.name << "error: %2" << serial->errorString();
                    break;
                } else if (bytesWritten != writeData.size()) {
                    qDebug() << "Failed to write all the data to port " << p.name <<  ", error: " << serial->errorString();
                    break;
                } else if (!serial->waitForBytesWritten(5000)) {
                    qDebug() << "Operation timed out or an error occurred for port " << p.name << ", error: " << serial->errorString();
                    break;
                }

                qDebug() << "Data successfully sent to port" << p.name;

                QByteArray readData = serial->readAll();
                while (serial->waitForReadyRead(2000))
                    readData.append(serial->readAll());

                qDebug() << "Data successfully received from port" << p.name;
                qDebug() << readData.toHex();

                if (serial->error() == QSerialPort::ReadError) {
                    qDebug() << "Failed to read from port " << p.name << ", error: " << serial->errorString();
                    break;
                } else if (serial->error() == QSerialPort::TimeoutError && readData.isEmpty()) {
                    qDebug() << "No data was currently available for reading from port " << p.name;
                    break;
                } else if (readData.at(6) != 0x06 && readData.at(6) != 0x08 && readData.at(6) != 0x0b){
                    qDebug() << "No matching function code received" << readData.at(6) << "end";
                    break;
                }

                found = true;
                break;
            }while (1);

            // we need reclaim the resource.
            serial->close();

            if (found)
                break;
        }
        else
        {
            qDebug() << "util.qSerialPortHelper Open Port" << info.portName()
                     << "failed!";
        }
    }
    progress.setValue(infos.size());

    if (!found){
        p.name = QString("");
    }
    return p;
}

QSerialPortSetting *ActionWidget::settingDialog() const
{
    return m_settingDialog;
}

void ActionWidget::setSettingDialog(QSerialPortSetting *settingDialog)
{
    m_settingDialog = settingDialog;
}

//#include "moc_actionwidget.cpp"
