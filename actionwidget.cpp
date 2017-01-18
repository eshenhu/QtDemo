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
#include <QMessageBox>
#include <QProgressDialog>
#include <QSerialPortInfo>
#include <QString>
#include <memory>

#include "ui/compqchartwidget.h"
#include "ui/configtab.h"
#include "ui/testtab.h"
#include "cfg/cfgreshandler.h"

#include "driver/automationmodeldriverclz.h"
#include "unireslocation.h"
//#include "cfg/datajsonrecelement.h"

//#include "util/utildatarecordingclz.h"
//#include "cfg/cfgjsonrecelement.h"
#include "chartsview/chartsviewheaders.h"
#include "chartsview/chartviewerwin.h"

QT_CHARTS_USE_NAMESPACE

ActionWidget::ActionWidget(QWidget *parent)
    : QWidget(parent),
      m_msgBox(nullptr)
{
    m_measData.type = TestPlanEnum::Invaild;
    //m_cfgHandler = new CfgResHandler();
    //m_reader = new CfgJsonReader();
    //m_reader->load("PV11");

    //---------------------------------------------
//    UtilDataRecordingClz::getInstance().newRec();

//    qWarning() << "UtilDataRecordingClz.getInstance().getCfgName" << UtilDataRecordingClz::getInstance().getCfgFileName();
//    qWarning() << "UtilDataRecordingClz.getInstance().getRecName" << UtilDataRecordingClz::getInstance().getRecFileName();

//    CfgJsonRecElement ele = CfgJsonRecElement::CfgJsonRecElementBuilder().build();
//    ele.saveCfg(UtilDataRecordingClz::getInstance().getCfgFileName());

//    DataJsonRecElementE2::DataJsonRecElementE2FileHelper helper;
//    helper.newFile(UtilDataRecordingClz::getInstance().getRecFileName());
//    DataJsonRecElementE2& e2 = DataJsonRecElementE2::DataJsonRecElementE2GetHelper().getElem(true);

//    helper.writeData(e2);
//    e2.incCursor();
//    helper.writeData(e2);
//    helper.closeFile();

    //------------------------------------------------


    m_driver = new AutomationModelDriverClz(this);

    createTabWidget();
    //createChartView();
    m_chartWidget = new CompQChartWidget(UniResLocation::getCfgJsonHdl(), this);

    QHBoxLayout *baseLayout = new QHBoxLayout();
    baseLayout->addWidget(m_tabWidget, 0);
    baseLayout->addWidget(m_chartWidget, 1);
    //baseLayout->addWidget(m_chartView, 1);
    setLayout(baseLayout);


//    for (int idx = 0; idx <= m_subTestTabWidget->tabWidget()->count(); ++idx)
//    {
//        m_subTestTabWidget->tabWidget()->setTabEnabled(idx, false);
//    }
//    m_subTestTabWidget->chartSettings()->setCheckable(false);
//    m_tabWidget->setTabEnabled(1, false);

    connect(m_driver, &AutomationModelDriverClz::updateData, m_chartWidget, &CompQChartWidget::updateData);
    connect(m_driver, &AutomationModelDriverClz::stateChanged, [this](
            const AutomationModelDriverClz::QModBusState state, QString str){
        qDebug() << "ui.actionwidget received statechange signal" << str;
        if(state == AutomationModelDriverClz::QModBusState::FatalErrorException)
        {
            m_msgBox = std::unique_ptr<QMessageBox>(new QMessageBox(QMessageBox::Warning, tr("Going into RESET state"), str));
            m_msgBox->exec();
        }
        else if (state == AutomationModelDriverClz::QModBusState::Disconnected)
        {
            m_msgBox = nullptr;
        }
    });
    connect(m_subTestTabWidget->start_btn(), &QPushButton::clicked, [this](bool checked){

        // why it lead to crash? --eshenhu
        //QPushButton* button = qobject_cast<QPushButton*>(sender());

        QSerialPortSetting::Settings setting;
        if (checked)
        {
            bool isStatusOK = false;
            do{
                if (m_measData.type == TestPlanEnum::Invaild){
                    QMessageBox::warning(this, tr("Warning"), tr("No available selection\n"
                                                                 "Please make your selection"),
                                         QMessageBox::Ok);
                    break;
                }

                setting = this->doAutoSelectSerialPlugInPort();
                if (setting.name.isEmpty()){
                    QMessageBox::warning(this, tr("Warning"),
                                         tr("Scanning serial port failed \n"
                                            "Please check your cable connection status"),
                                         QMessageBox::Ok);
                    break;
                }

                isStatusOK = true;
                break;
            }while(1);

            if (isStatusOK){
                enableWidgetInFront(false);
                m_subTestTabWidget->start_btn()->setText(QStringLiteral("Stop"));
                m_subTestTabWidget->start_btn()->setIcon(QIcon(":/ui/ui/pause.png"));

                m_driver->startMeasTest(m_measData, UniResLocation::getCfgResHdl(), setting);
                m_driver->setUserSetSensitive(m_subTestTabWidget->getUserSetSensentive());
                //reset.
                m_measData.type = TestPlanEnum::Invaild;
            }
            else{
                enableWidgetInFront(true);
                m_subTestTabWidget->start_btn()->setChecked(false);
            }
        }
        else
        {
            enableWidgetInFront(true);
            m_subTestTabWidget->start_btn()->setText(QStringLiteral("Start"));
            m_subTestTabWidget->start_btn()->setIcon(QIcon(":/ui/ui/play.png"));

            m_driver->enterFSMResetState(tr("Info: User stop the ongoing test"));
        }
    });

    connect(m_subTestTabWidget->showgraph_btn(), &QPushButton::clicked, [=](bool checked){
        Q_UNUSED(checked)
        ChartViewerWin* chartViewer = new ChartViewerWin(parent);
        chartViewer->setWindowModality(Qt::ApplicationModal);
        chartViewer->show();
    });

//    updateSerieSettings();
//    updateChartSettings();
}

ActionWidget::~ActionWidget()
{
//    if(m_cfgHandler)
//        delete m_cfgHandler;
//    if (m_reader)
//        delete m_reader;
    if (m_chartWidget)
        delete m_chartWidget;
}

void ActionWidget::createTabWidget()
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->setTabPosition(QTabWidget::North);
    m_tabWidget->setTabBarAutoHide(true);

    m_subTestTabWidget = new TestTab();

    connect(m_subTestTabWidget, &TestTab::updateUserSelection, [this](UiCompMeasData data){
        m_measData = data;
    });

    connect(m_subTestTabWidget, SIGNAL(syncDataDuringManual(double,quint32)),
            m_driver, SLOT(syncDataDuringManualTest(double,quint32)));

    m_tabWidget->addTab(m_subTestTabWidget, tr("Test"));
    m_subConfigTabWidget = new ConfigTab(UniResLocation::getCfgResHdl());
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

//void ActionWidget::updateUserInput(UiCompMeasData data)
//{
//    m_measData = data;
//}

//void ActionWidget::updateUserInput(ThrottleTstData data)
//{
//    m_measData.type = TestCasePrimType::TCTHROTTLE;
//    m_measData.data.v = data;
//}

//void ActionWidget::updateUserInput(DistanceTstData data)
//{
//    m_measData.type = TestCasePrimType::TCDISTANCE;
//    m_measData.data.w = data;
//}

//void ActionWidget::updateUserInput(MultipuleTstData data)
//{
//    m_measData.type = TestCasePrimType::TCMULTIPULE;
//    m_measData.data.x = data;
//}


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

/*
 * dim / shine the Widget in front.
 *      false: dim
 *      true : shine
*/
void ActionWidget::enableWidgetInFront(bool doshine)
{
    m_subConfigTabWidget->setEnabled(doshine);
    m_subTestTabWidget->enableWidgetInFront(doshine);
}

//#include "moc_actionwidget.cpp"
