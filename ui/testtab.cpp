#include "testtab.h"


//#include "moc_actionwidget.cpp"

#include <QFormLayout>
#include <QGroupBox>
#include <qdialogbuttonbox.h>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTabWidget>
#include <QMessageBox>

class ConstValue
{
public:
    enum VolStepStageEnum {
        S1 = 1,
        S2 = 2,
        S5 = 5
    };

    enum DistanceStepStageEnum {
        SDis1 = 1,
        SDis2 = 2,
        SDis5 = 5
    };

    enum ThrottleStepStageEnum {
        SThr1 = 1,
        SThr2 = 2,
        SThr5 = 5
    };

    static const int DEFAULT_VOL = 10;
    static const int MIN_VOL     = 0;
    static const int MAX_VOL     = 20;
    static const int STEP_VOL    = 1;

    static const int DEFAULT_THR = 50;
    static const int MIN_THR     = 0;
    static const int MAX_THR     = 99;
    static const int STEP_THR    = 1;

    static const int DIS_FWD     = 1;
    static const int DIS_START   = 0;
    static const int DIS_END     = 99;
    static const int DIS_STEP    = 1;

public:
    explicit ConstValue(){}
};

TestTab::TestTab(QWidget *parent)
    : QWidget(parent)
{
    // test settings
    m_testSeletionComboBox = new QComboBox();
    m_testSeletionComboBox->addItem(tr("Distance"));
    m_testSeletionComboBox->addItem(tr("Voltage"));
    m_testSeletionComboBox->addItem(tr("Throttle"));
    m_testSeletionComboBox->addItem(tr("Multiple"));
    m_testSeletionComboBox->addItem(tr("Aging"));
    m_testSeletionComboBox->addItem(tr("Calibrate"));
    m_testSeletionComboBox->addItem(tr("Manual"));
    m_testSeletionComboBox->setCurrentIndex(TestPlanEnum::Distance);

    m_aaCheckBox = new QCheckBox();
    m_animationsCheckBox = new QCheckBox();
    m_animationsCheckBox->setCheckState(Qt::Checked);

    m_legendCheckBox = new QCheckBox();

    QFormLayout *chartSettingsLayout = new QFormLayout();
    chartSettingsLayout->addRow("Test Plan", m_testSeletionComboBox);
    chartSettingsLayout->addRow("Vol Limit", m_aaCheckBox);
    chartSettingsLayout->addRow("Animations", m_animationsCheckBox);
    chartSettingsLayout->addRow("Legend", m_legendCheckBox);
    QGroupBox *chartSettings = new QGroupBox("Chart");
    chartSettings->setLayout(chartSettingsLayout);

    connect(m_testSeletionComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOptionsSelection(int)));


    m_start_btn = new QPushButton(tr("Start"));
    m_showgraph_btn = new QPushButton(tr("Show Graph"));

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(m_start_btn);
    seriesSettingsLayout->addRow(m_showgraph_btn);

    QGroupBox *seriesSettings = new QGroupBox(tr("Action"));
    seriesSettings->setLayout(seriesSettingsLayout);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    m_tabWidget = new QTabWidget;
    tabList[TestPlanEnum::Distance] = new DistanceTstTab();
    tabList[TestPlanEnum::Voltage] = new VoltageTstTab();
    tabList[TestPlanEnum::Throttle] = new ThrottleTstTab();
    tabList[TestPlanEnum::Multiplue] = new MultipleTstTab();
    tabList[TestPlanEnum::Aging] = new AgingTstTab();
    tabList[TestPlanEnum::Calibrate] = new CalibrateTstTab();
    tabList[TestPlanEnum::Manual] = new ManualTstTab();

    m_tabWidget->insertTab(0, tabList[TestPlanEnum::Distance], tr("Distance"));

    QGridLayout *settingsLayout = new QGridLayout();
    settingsLayout->addWidget(chartSettings, 0, 0);
    settingsLayout->setColumnStretch(0, 0);
    settingsLayout->addWidget(m_tabWidget, 0, 1);
    settingsLayout->setColumnStretch(1, 1);
    settingsLayout->addWidget(seriesSettings, 0, 2);
    settingsLayout->setColumnStretch(2, 0);

    setLayout(settingsLayout);
}

void TestTab::updateOptionsSelection(int index)
{
    m_tabWidget->removeTab(0);
    switch (index) {
    case Distance:
        m_tabWidget->insertTab(0, tabList[TestPlanEnum::Distance], tr("Distance"));
        break;
    case Voltage:
        m_tabWidget->insertTab(0, tabList[TestPlanEnum::Voltage], tr("Voltage"));
        break;
    case Throttle:
        m_tabWidget->insertTab(0, tabList[TestPlanEnum::Throttle], tr("Throttle"));
        break;
    case Multiplue:
        m_tabWidget->insertTab(0, tabList[TestPlanEnum::Multiplue], tr("Multiplue"));
        break;
    case Aging:
        m_tabWidget->insertTab(0, tabList[TestPlanEnum::Aging], tr("Aging"));
        break;
    case Calibrate:
        m_tabWidget->insertTab(0, tabList[TestPlanEnum::Calibrate], tr("Calibrate"));
        break;
    case Manual:
        m_tabWidget->insertTab(0, tabList[TestPlanEnum::Manual], tr("Manual"));
        break;
    default:
        qWarning("Unsupport tab");
        break;
    }
}

//void TestTab::enableTestTab(TestPlanEnum e)
//{
//    int tabIdx = m_tabWidget->currentIndex();
//    if (tabIdx != -1)
//        m_tabWidget->removeTab(static_cast<TestPlanEnum>(tabIdx));
//    m_tabWidget->addTab(e);
//}


DistanceTstTab::DistanceTstTab(QWidget *parent)
    : QWidget(parent)
{
    // series settings
    m_voltage = new QSpinBox();
    m_voltage->setMinimumWidth(70);
    m_voltage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_voltage->setSingleStep(ConstValue::STEP_VOL);
    m_voltage->setValue(ConstValue::DEFAULT_VOL);

    m_throttle = new QSpinBox();
    m_throttle->setMinimumWidth(70);
    m_throttle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_throttle->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_throttle->setSingleStep(ConstValue::STEP_THR);
    m_throttle->setValue(ConstValue::DEFAULT_THR);

    m_disStart = new QSpinBox();
    m_disStart->setMinimumWidth(70);
    m_disStart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_disStart->setRange(ConstValue::DIS_START, ConstValue::DIS_END);
    m_disStart->setSingleStep(ConstValue::DIS_STEP);
    m_disStart->setValue(ConstValue::DIS_START);

    m_disEnd = new QSpinBox();
    m_disEnd->setMinimumWidth(70);
    m_disEnd->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_disEnd->setRange(ConstValue::DIS_START, ConstValue::DIS_END);
    m_disEnd->setSingleStep(ConstValue::DIS_STEP);
    m_disEnd->setValue(ConstValue::DIS_END);

    m_disStep = new QComboBox();
    m_disStep->setMinimumWidth(70);
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis1));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis2));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis5));
    m_disStep->setCurrentIndex(0);

    m_apply_btn = new QPushButton(tr("Apply"));

    connect(m_apply_btn, SIGNAL(clicked(bool)), this, SLOT(validateUserInput(bool)));

    //connect(nextButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toNext);

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Voltage(V)"), m_voltage);
    seriesSettingsLayout->addRow(tr("Throttle(%)"), m_throttle);
    seriesSettingsLayout->addRow(tr("Dis Start(mm)"), m_disStart);
    seriesSettingsLayout->addRow(tr("Dis End(mm)"), m_disEnd);
    seriesSettingsLayout->addRow(tr("Dis Step(mm)"), m_disStep);
    seriesSettingsLayout->addRow(m_apply_btn);

    QFormLayout *outputListLayout = new QFormLayout();

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);
    horizonLayout->addLayout(outputListLayout, 1);

    setLayout(horizonLayout);
}

void DistanceTstTab::validateUserInput(bool checked)
{
    if(m_disStart->value() >= m_disEnd->value())
    {
        QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                             tr("The END value is larger than START value"),
                             QMessageBox::Close);
        warningBox.exec();
    }
}


VoltageTstTab::VoltageTstTab(QWidget *parent)
    : QWidget(parent)
{
    // series settings
    m_throttle = new QSpinBox();
    m_throttle->setMinimumWidth(70);
    m_throttle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_throttle->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_throttle->setSingleStep(ConstValue::STEP_THR);
    m_throttle->setValue(ConstValue::DEFAULT_THR);

    m_voltage_start = new QSpinBox();
    m_voltage_start->setMinimumWidth(70);
    m_voltage_start->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage_start->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_voltage_start->setSingleStep(ConstValue::STEP_VOL);
    m_voltage_start->setValue(ConstValue::DEFAULT_VOL);

    m_voltage_end = new QSpinBox();
    m_voltage_end->setMinimumWidth(70);
    m_voltage_end->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage_end->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_voltage_end->setSingleStep(ConstValue::STEP_VOL);
    m_voltage_end->setValue(ConstValue::MAX_VOL);

    m_voltage_step = new QComboBox();
    m_voltage_step->setMinimumWidth(70);
    m_voltage_step->addItem(QString::number(ConstValue::VolStepStageEnum::S1));
    m_voltage_step->addItem(QString::number(ConstValue::VolStepStageEnum::S2));
    m_voltage_step->addItem(QString::number(ConstValue::VolStepStageEnum::S5));
    m_voltage_step->setCurrentIndex(0);

    m_apply_btn = new QPushButton(tr("Apply"));

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Throttle(%)"), m_throttle);
    seriesSettingsLayout->addRow(tr("Vol Start(V)"), m_voltage_start);
    seriesSettingsLayout->addRow(tr("Vol End(V)"), m_voltage_end);
    seriesSettingsLayout->addRow(tr("Vol Step(V)"), m_voltage_step);

    QFormLayout *outputListLayout = new QFormLayout();

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);
    horizonLayout->addLayout(outputListLayout, 1);

    setLayout(horizonLayout);
}

ThrottleTstTab::ThrottleTstTab(QWidget *parent)
    : QWidget(parent)
{

}

MultipleTstTab::MultipleTstTab(QWidget *parent)
    : QWidget(parent)
{

}

AgingTstTab::AgingTstTab(QWidget *parent)
    : QWidget(parent)
{

}

CalibrateTstTab::CalibrateTstTab(QWidget *parent)
    : QWidget(parent)
{

}

ManualTstTab::ManualTstTab(QWidget *parent)
    : QWidget(parent)
{

}
