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
#include "customslice.h"
#include "pentool.h"
#include "brushtool.h"
#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtCharts/QChartView>

#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <QtGlobal>
#include <QtWidgets/QMessageBox>
#include "ui/compqchartwidget.h"
#include "ui/configtab.h"
#include "ui/testtab.h"
#include "cfg/cfgreshandler.h"
#include "cfg/cfgjsonprimaryelement.h"

QT_CHARTS_USE_NAMESPACE

ActionWidget::ActionWidget(QWidget *parent)
    : QWidget(parent)
{
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

//    updateSerieSettings();
//    updateChartSettings();
}

ActionWidget::~ActionWidget()
{
    if(m_cfgHandler)
        delete m_cfgHandler;
    if (m_reader)
        delete m_reader;
}

void ActionWidget::createChartView()
{
    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("Measument Test");
    //![1]
    //!
    //![2]
    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(10);
    chart->addAxis(axisX, Qt::AlignBottom);
    //![2]

    //![3]
    QLineSeries *series = new QLineSeries;
    *series << QPointF(1, 5) << QPointF(3.5, 18) << QPointF(4.8, 7.5) << QPointF(10, 2.5);
    chart->addSeries(series);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLinePenColor(series->pen().color());

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    //![3]

    //![5]
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
}

void ActionWidget::createTabWidget()
{
    m_tabWidget = new QTabWidget;
    m_tabWidget->setTabPosition(QTabWidget::West);
    m_tabWidget->setTabBarAutoHide(true);
    m_tabWidget->addTab(new TestTab(), tr("Test"));
    m_tabWidget->addTab(new ConfigTab(m_cfgHandler), tr("Config"));
    //m_tabWidget->hide();
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

//void MainWidget::updateSerieSettings()
//{
//    m_series->setHorizontalPosition(m_hPosition->value());
//    m_series->setVerticalPosition(m_vPosition->value());
//    m_series->setPieSize(m_sizeFactor->value());
//    m_holeSize->setMaximum(m_sizeFactor->value());
//    m_series->setPieStartAngle(m_startAngle->value());
//    m_series->setPieEndAngle(m_endAngle->value());
//    m_series->setHoleSize(m_holeSize->value());
//}

const CfgJsonReader *ActionWidget::reader() const
{
    return m_reader;
}

SettingsDialog *ActionWidget::settingDialog() const
{
    return m_settingDialog;
}

void ActionWidget::setSettingDialog(SettingsDialog *settingDialog)
{
    m_settingDialog = settingDialog;
}

//#include "moc_actionwidget.cpp"
