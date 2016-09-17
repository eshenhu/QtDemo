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
#include <QtWidgets/QFontDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>


QT_CHARTS_USE_NAMESPACE

ActionWidget::ActionWidget(QWidget *parent)
    : QWidget(parent),
      m_slice(0)
{
    //![1]
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
    QSplineSeries *series = new QSplineSeries;
    *series << QPointF(1, 5) << QPointF(3.5, 18) << QPointF(4.8, 7.5) << QPointF(10, 2.5);
    chart->addSeries(series);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLinePenColor(series->pen().color());

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    //![3]

    //![4]
    series = new QSplineSeries;
    *series << QPointF(1, 0.5) << QPointF(1.5, 4.5) << QPointF(2.4, 2.5) << QPointF(4.3, 12.5)
            << QPointF(5.2, 3.5) << QPointF(7.4, 16.5) << QPointF(8.3, 7.5) << QPointF(10, 17);
    chart->addSeries(series);

    QCategoryAxis *axisY3 = new QCategoryAxis;
    axisY3->append("Low", 5);
    axisY3->append("Medium", 12);
    axisY3->append("High", 17);
    axisY3->setLinePenColor(series->pen().color());
    axisY3->setGridLinePen((series->pen()));

    chart->addAxis(axisY3, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY3);
    //![4]

    //![5]
    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    // chart settings
    m_themeComboBox = new QComboBox();
    m_themeComboBox->addItem("Distance");
    m_themeComboBox->addItem("Voltage");
    m_themeComboBox->addItem("Throttle");
    m_themeComboBox->addItem("Aging");
    m_themeComboBox->addItem("Calibrate");
    m_themeComboBox->addItem("Manual");

    m_aaCheckBox = new QCheckBox();
    m_animationsCheckBox = new QCheckBox();
    m_animationsCheckBox->setCheckState(Qt::Checked);

    m_legendCheckBox = new QCheckBox();

    QFormLayout *chartSettingsLayout = new QFormLayout();
    chartSettingsLayout->addRow("Test Plan", m_themeComboBox);
    chartSettingsLayout->addRow("Vol Limit", m_aaCheckBox);
    chartSettingsLayout->addRow("Animations", m_animationsCheckBox);
    chartSettingsLayout->addRow("Legend", m_legendCheckBox);
    QGroupBox *chartSettings = new QGroupBox("Chart");
    chartSettings->setLayout(chartSettingsLayout);

//    connect(m_themeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateChartSettings()));
//    connect(m_aaCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateChartSettings()));
//    connect(m_animationsCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateChartSettings()));
//    connect(m_legendCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateChartSettings()));

    // series settings
    m_hPosition = new QSpinBox();
    m_hPosition->setMinimum(0);
    m_hPosition->setMaximum(20);
    m_hPosition->setSingleStep(1);
//    m_hPosition->setValue(m_series->horizontalPosition());

    m_vPosition = new QSpinBox();
    m_vPosition->setMinimum(0);
    m_vPosition->setMaximum(10);
    m_vPosition->setSingleStep(1);
//    m_vPosition->setValue(m_series->verticalPosition());

    m_sizeFactor = new QSpinBox();
    m_sizeFactor->setMinimum(0);
    m_sizeFactor->setMaximum(10);
    m_sizeFactor->setSingleStep(1);
//    m_sizeFactor->setValue(m_series->pieSize());

    m_startAngle = new QSpinBox();
    m_startAngle->setMinimum(0);
    m_startAngle->setMaximum(20);
    m_startAngle->setSingleStep(1);
//    m_startAngle->setValue(m_series->pieStartAngle());

//    QPushButton *appendSlice = new QPushButton("Append slice");
//    QPushButton *insertSlice = new QPushButton("Insert slice");
//    QPushButton *removeSlice = new QPushButton("Remove selected slice");

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow("Throttle", m_hPosition);
    seriesSettingsLayout->addRow("Vol Begin", m_vPosition);
    seriesSettingsLayout->addRow("Vol End", m_sizeFactor);
    seriesSettingsLayout->addRow("Vol Step", m_startAngle);
//    seriesSettingsLayout->addRow("End angle", m_endAngle);
//    seriesSettingsLayout->addRow("Hole size", m_holeSize);
//    seriesSettingsLayout->addRow(appendSlice);
//    seriesSettingsLayout->addRow(insertSlice);
//    seriesSettingsLayout->addRow(removeSlice);
    QGroupBox *seriesSettings = new QGroupBox("Selection");
    seriesSettings->setLayout(seriesSettingsLayout);

    // slice settings
    m_sliceName = new QLineEdit("<click a slice>");
    m_sliceName->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_sliceValue = new QDoubleSpinBox();

    QFormLayout *sliceSettingsLayout = new QFormLayout();
    sliceSettingsLayout->addRow("Label", m_sliceName);
    sliceSettingsLayout->addRow("Value", m_sliceValue);
//    sliceSettingsLayout->addRow("Pen", m_pen);
//    sliceSettingsLayout->addRow("Brush", m_brush);
//    sliceSettingsLayout->addRow("Label visible", m_sliceLabelVisible);
//    sliceSettingsLayout->addRow("Label font", m_font);
//    sliceSettingsLayout->addRow("Label brush", m_labelBrush);
//    sliceSettingsLayout->addRow("Label position", m_labelPosition);
//    sliceSettingsLayout->addRow("Label arm length", m_sliceLabelArmFactor);
//    sliceSettingsLayout->addRow("Exploded", m_sliceExploded);
//    sliceSettingsLayout->addRow("Explode distance", m_sliceExplodedFactor);
    QGroupBox *sliceSettings = new QGroupBox("Selected slice");
    sliceSettings->setLayout(sliceSettingsLayout);

//    connect(m_sliceName, SIGNAL(textChanged(QString)), this, SLOT(updateSliceSettings()));
//    connect(m_sliceValue, SIGNAL(valueChanged(double)), this, SLOT(updateSliceSettings()));
//    connect(m_pen, SIGNAL(clicked()), m_penTool, SLOT(show()));
//    connect(m_penTool, SIGNAL(changed()), this, SLOT(updateSliceSettings()));
//    connect(m_brush, SIGNAL(clicked()), m_brushTool, SLOT(show()));
//    connect(m_brushTool, SIGNAL(changed()), this, SLOT(updateSliceSettings()));
//    connect(m_font, SIGNAL(clicked()), this, SLOT(showFontDialog()));
//    connect(m_labelBrush, SIGNAL(clicked()), m_labelBrushTool, SLOT(show()));
//    connect(m_labelBrushTool, SIGNAL(changed()), this, SLOT(updateSliceSettings()));
//    connect(m_sliceLabelVisible, SIGNAL(toggled(bool)), this, SLOT(updateSliceSettings()));
//    connect(m_sliceLabelVisible, SIGNAL(toggled(bool)), this, SLOT(updateSliceSettings()));
//    connect(m_sliceLabelArmFactor, SIGNAL(valueChanged(double)), this, SLOT(updateSliceSettings()));
//    connect(m_sliceExploded, SIGNAL(toggled(bool)), this, SLOT(updateSliceSettings()));
//    connect(m_sliceExplodedFactor, SIGNAL(valueChanged(double)), this, SLOT(updateSliceSettings()));
//    connect(m_labelPosition, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSliceSettings()));

    // create chart view
    //m_chartView = new QChartView(chart);



    //! [1] //! [2]
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    //! [1] //! [3]
                                         | QDialogButtonBox::Cancel);


    // create main layout
    QHBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->addWidget(chartSettings);
    settingsLayout->addWidget(seriesSettings);
    settingsLayout->addWidget(sliceSettings);
    //settingsLayout->addWidget(m_buttonBox);
    settingsLayout->addStretch();


    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(m_chartView, 0, 0);
    baseLayout->addLayout(settingsLayout, 1, 0);
    //baseLayout->addLayout(actionLayout, 0, 2);

    setLayout(baseLayout);

//    updateSerieSettings();
//    updateChartSettings();
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

//void MainWidget::updateSliceSettings()
//{
//    if (!m_slice)
//        return;

//    m_slice->setLabel(m_sliceName->text());

//    m_slice->setValue(m_sliceValue->value());

//    m_slice->setPen(m_penTool->pen());
//    m_slice->setBrush(m_brushTool->brush());

//    m_slice->setLabelBrush(m_labelBrushTool->brush());
//    m_slice->setLabelVisible(m_sliceLabelVisible->isChecked());
//    m_slice->setLabelArmLengthFactor(m_sliceLabelArmFactor->value());
//    m_slice->setLabelPosition((QPieSlice::LabelPosition)m_labelPosition->currentIndex()); // assumes that index is in sync with the enum

//    m_slice->setExploded(m_sliceExploded->isChecked());
//    m_slice->setExplodeDistanceFactor(m_sliceExplodedFactor->value());
//}

/*
void MainWidget::handleSliceClicked(QPieSlice *slice)
{
    m_slice = static_cast<CustomSlice *>(slice);

    // name
    m_sliceName->blockSignals(true);
    m_sliceName->setText(slice->label());
    m_sliceName->blockSignals(false);

    // value
    m_sliceValue->blockSignals(true);
    m_sliceValue->setValue(slice->value());
    m_sliceValue->blockSignals(false);

    // pen
    m_pen->setText(PenTool::name(m_slice->pen()));
    m_penTool->setPen(m_slice->pen());

    // brush
    m_brush->setText(m_slice->originalBrush().color().name());
    m_brushTool->setBrush(m_slice->originalBrush());

    // label
    m_labelBrush->setText(BrushTool::name(m_slice->labelBrush()));
    m_labelBrushTool->setBrush(m_slice->labelBrush());
    m_font->setText(slice->labelFont().toString());
    m_sliceLabelVisible->blockSignals(true);
    m_sliceLabelVisible->setChecked(slice->isLabelVisible());
    m_sliceLabelVisible->blockSignals(false);
    m_sliceLabelArmFactor->blockSignals(true);
    m_sliceLabelArmFactor->setValue(slice->labelArmLengthFactor());
    m_sliceLabelArmFactor->blockSignals(false);
    m_labelPosition->blockSignals(true);
    m_labelPosition->setCurrentIndex(slice->labelPosition()); // assumes that index is in sync with the enum
    m_labelPosition->blockSignals(false);

    // exploded
    m_sliceExploded->blockSignals(true);
    m_sliceExploded->setChecked(slice->isExploded());
    m_sliceExploded->blockSignals(false);
    m_sliceExplodedFactor->blockSignals(true);
    m_sliceExplodedFactor->setValue(slice->explodeDistanceFactor());
    m_sliceExplodedFactor->blockSignals(false);
}
*/
void ActionWidget::showFontDialog()
{
    if (!m_slice)
        return;

    QFontDialog dialog(m_slice->labelFont());
    dialog.show();
    dialog.exec();

    m_slice->setLabelFont(dialog.currentFont());
    m_font->setText(dialog.currentFont().toString());
}

#include "moc_actionwidget.cpp"

#include <qdialogbuttonbox.h>
