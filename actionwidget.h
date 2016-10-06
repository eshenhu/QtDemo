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
class SettingsDialog;

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ActionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActionWidget(QWidget *parent = 0);

    void setSettingDialog(SettingsDialog *settingDialog);
    SettingsDialog *settingDialog() const;

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
    SettingsDialog *m_settingDialog;
    void createChartView();
    void createTabWidget();
};
#endif // MAINWIDGET_H
