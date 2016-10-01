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
#ifndef ACTIONWIDGET_H
#define ACTIONWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <qdialogbuttonbox.h>
#include <qspinbox.h>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QTabWidget;
QT_END_NAMESPACE

class PenTool;
class BrushTool;
class CustomSlice;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

enum TestPlanEnum {
    Distance,
    Voltage,
    Throttle,
    Multiplue,
    Aging,
    Calibrate,
    Manual
};

class ActionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionWidget(QWidget *parent = 0);

public Q_SLOTS:
//    void updateChartSettings();
//    void updateSerieSettings();
//    void updateSliceSettings();

    void showFontDialog();

private:
//    QComboBox *m_themeComboBox;
//    QCheckBox *m_aaCheckBox;
//    QCheckBox *m_animationsCheckBox;
//    QCheckBox *m_legendCheckBox;

    QChartView *m_chartView;

    CustomSlice *m_slice;

    QTabWidget *m_tabWidget;

    QDialogButtonBox *m_buttonBox;
    void createChartView();
    void createTabWidget();
};


class TestTab : public QWidget
{
    Q_OBJECT

public:
    explicit TestTab(QWidget *parent = 0);

protected slots:
    void updateOptionsSelection(int index);

public:
    QTabWidget *m_tabWidget;
    QWidget* tabList[TestPlanEnum::Manual + 1];
    QWidget* lastActiveWidget;

private:
    QComboBox *m_testSeletionComboBox;
    QCheckBox *m_aaCheckBox;
    QCheckBox *m_animationsCheckBox;
    QCheckBox *m_legendCheckBox;

    QDialogButtonBox* m_buttonBox;

    QPushButton* m_start_btn;
    QPushButton* m_showgraph_btn;

private:
    void enableTestTab(TestPlanEnum);
};

class DistanceTstTab : public QWidget
{
    Q_OBJECT
public:
    QSpinBox *m_voltage;
    QSpinBox *m_throttle;
    QSpinBox *m_disStart;
    QSpinBox *m_disEnd;
    QComboBox *m_disStep;
    QPushButton *m_apply_btn;

public:
    explicit DistanceTstTab(QWidget *parent = 0);


private slots:
    void validateUserInput(bool checked = false);
};

class VoltageTstTab : public QWidget
{
    Q_OBJECT
private:
    QSpinBox *m_throttle;
    QSpinBox *m_voltage_start;
    QSpinBox *m_voltage_end;
    QComboBox *m_voltage_step;
    QPushButton *m_apply_btn;
public:

    explicit VoltageTstTab(QWidget *parent = 0);
};
class ThrottleTstTab : public QWidget
{
    Q_OBJECT

public:
    explicit ThrottleTstTab(QWidget *parent = 0);
};
class MultipleTstTab : public QWidget
{
    Q_OBJECT

public:
    explicit MultipleTstTab(QWidget *parent = 0);
};
class AgingTstTab : public QWidget
{
    Q_OBJECT

public:
    explicit AgingTstTab(QWidget *parent = 0);
};
class CalibrateTstTab : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrateTstTab(QWidget *parent = 0);
};
class ManualTstTab : public QWidget
{
    Q_OBJECT

public:
    explicit ManualTstTab(QWidget *parent = 0);
};

#endif // MAINWIDGET_H
