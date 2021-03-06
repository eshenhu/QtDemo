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
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include "cfg/unireslocation.h"

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
        SDis5 = 5,
        SDis10 = 10,
        SDis15 = 15,
        SDis20 = 20,
        SDis25 = 25,
        SDis30 = 30
    };

    enum ThrottleStepStageEnum {
        SThr1 = 1,
        SThr2 = 2,
        SThr5 = 5
    };

    enum DurationTime {
        D05Sec = 5,
        D10Sec = 10,
        D15Sec = 15,
        D20Sec = 20,
        D30Sec = 30,
        D60Sec = 60,
        D180Sec = 180,
        D300Sec = 300,
        D600Sec = 600,
        D1200Sec = 1200,
        D1800Sec = 1800,
        D3600Sec = 3600
    };

    static constexpr double DEFAULT_VOL = 10.0;
    static constexpr double MIN_VOL     = 0.0;
    static constexpr double MAX_VOL     = 20.0;
    static constexpr double STEP_VOL    = 1.0;

    static const int DEFAULT_THR = 50;
    static const int DEFAULT_THR_LOW  = 20;
    static const int DEFAULT_THR_HIGH = 80;
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
    m_volLimCheckBox = new QCheckBox();
    m_volLimitLineEdit = new QSpinBox();
    m_volLimitLineEdit->setValue(UniResLocation::getCfgResHdl()->max_vol());
    m_volLimitLineEdit->setRange(0, UniResLocation::getCfgResHdl()->max_vol());
    m_volLimitLineEdit->setSingleStep(1);
    QHBoxLayout* layout_vol = new QHBoxLayout();
    layout_vol->addWidget(m_volLimCheckBox);
    layout_vol->addWidget(m_volLimitLineEdit);
    connect(m_volLimCheckBox, &QCheckBox::stateChanged, [this](bool isChecked){
        m_volLimitLineEdit->setEnabled(!isChecked);
    });

    m_curLimCheckBox = new QCheckBox();
    m_curLimitLineEdit = new QSpinBox();
    m_curLimitLineEdit->setValue(UniResLocation::getCfgResHdl()->max_cur());
    m_curLimitLineEdit->setRange(0, UniResLocation::getCfgResHdl()->max_cur());
    m_curLimitLineEdit->setSingleStep(1);
    //m_currentLimCheckBox->setCheckState(Qt::Checked);
    QHBoxLayout* layout_cur = new QHBoxLayout();
    layout_cur->addWidget(m_curLimCheckBox);
    layout_cur->addWidget(m_curLimitLineEdit);
    connect(m_curLimCheckBox, &QCheckBox::stateChanged, [this](bool isChecked){
        m_curLimitLineEdit->setEnabled(!isChecked);
    });

    m_tempLimCheckBox = new QCheckBox();
    m_tempLimitLineEdit = new QSpinBox();
    m_tempLimitLineEdit->setValue(150);
    m_tempLimitLineEdit->setRange(0, 150);
    m_tempLimitLineEdit->setSingleStep(1);
    QHBoxLayout* layout_temp = new QHBoxLayout();
    layout_temp->addWidget(m_tempLimCheckBox);
    layout_temp->addWidget(m_tempLimitLineEdit);
    connect(m_tempLimCheckBox, &QCheckBox::stateChanged, [this](bool isChecked){
        m_tempLimitLineEdit->setEnabled(!isChecked);
    });

    m_sensitiveComboBox = new QComboBox(this);
    m_sensitiveComboBox->addItem(tr("Low"));
    m_sensitiveComboBox->addItem(tr("Med"));
    m_sensitiveComboBox->addItem(tr("High"));

    QFormLayout *chartSettingsLayout = new QFormLayout();
    chartSettingsLayout->setVerticalSpacing(1);
    //chartSettingsLayout->addRow("Test Plan", m_testSeletionComboBox);

    chartSettingsLayout->addRow(tr("Vol Limit"), layout_vol);
    chartSettingsLayout->addRow(tr("Cur Limit"), layout_cur);
    chartSettingsLayout->addRow(tr("Temp Limit"), layout_temp);
    chartSettingsLayout->addRow(tr("Sensitive"), m_sensitiveComboBox);

    //chartSettingsLayout->addRow(tr("Enable Protect"), m_enableProtecCheckBox);
    m_chartSettings = new QGroupBox(this);
    m_chartSettings->setLayout(chartSettingsLayout);

////    QLabel* m_enableProtecLabel = new QLabel(tr("Enable Protection"), this);
//    m_enableProtecCheckBox = new QCheckBox();
//    connect(m_enableProtecCheckBox, &QCheckBox::stateChanged, [this](bool isChecked){

//        bool isEnabled = !isChecked;
//        m_volLimCheckBox->setEnabled(isEnabled);
//        m_volLimitLineEdit->setEnabled(isEnabled);
//        m_curLimCheckBox->setEnabled(isEnabled);
//        m_curLimitLineEdit->setEnabled(isEnabled);
//        m_tempLimCheckBox->setEnabled(isEnabled);
//        m_tempLimitLineEdit->setEnabled(isEnabled);
//    });

    m_start_btn = new QPushButton(QIcon(":/ui/ui/play.png"), tr("Start"));
    m_start_btn->setStyleSheet("QPushButton { text-align:left; padding:12px}");
    m_start_btn->setCheckable(true);
    m_showgraph_btn = new QPushButton(QIcon(":/ui/ui/fileopen.png"), tr("Show Graph"));
    m_showgraph_btn->setStyleSheet("QPushButton { text-align:left; padding:12px}");

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(m_start_btn);
    seriesSettingsLayout->addRow(m_showgraph_btn);

    QGroupBox *seriesSettings = new QGroupBox(tr("Action"));
    seriesSettings->setLayout(seriesSettingsLayout);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    m_tabWidget = new QTabWidget;
    m_tabWidget->setTabPosition(QTabWidget::West);

    if (UniResLocation::getCfgResHdl()->num_of_motor() >= 2)
    {
        m_disTstTab = new DistanceTstTab();
        tabList[TestPlanEnum::Distance] = m_disTstTab;
    }

    m_volTstTab = new VoltageTstTab();
    tabList[TestPlanEnum::Voltage] = m_volTstTab;

    m_throTstTab = new ThrottleTstTab(TestPlanEnum::Throttle);
    tabList[TestPlanEnum::Throttle] = m_throTstTab;

    //Multipule test can be regarded as Throttle Test.
    //tabList[TestPlanEnum::Multiplue] = new MultipleTstTab();
    m_multiTstTab = new MultiTstTab(TestPlanEnum::Multiplue);
    tabList[TestPlanEnum::Multiplue] = m_multiTstTab;

    m_agingTstTab = new AgingTstTab();
    tabList[TestPlanEnum::Aging] = m_agingTstTab;

    //tabList[TestPlanEnum::Calibrate] = new CalibrateTstTab();
    m_manualTstTab = new ManualTstTab();
    tabList[TestPlanEnum::Manual] = m_manualTstTab;

    m_tabWidget->addTab(tabList[TestPlanEnum::Voltage], tr("Voltage"));
//    connect(m_volTstTab, &VoltageTstTab::updateUserSelection, [this](VoltageTstData data){
//        emit updateUserSelection(data);
//    } );
    connect(m_volTstTab, SIGNAL(updateUserSelection(UiCompMeasData)),
                                this, SIGNAL(updateUserSelection(UiCompMeasData)));

    m_tabWidget->addTab(tabList[TestPlanEnum::Throttle], tr("Throttle"));
    connect(m_throTstTab, SIGNAL(updateUserSelection(UiCompMeasData)),
            this, SIGNAL(updateUserSelection(UiCompMeasData)));

    if (m_disTstTab)
    {
        m_tabWidget->addTab(tabList[TestPlanEnum::Distance], tr("Distance"));
        connect(m_disTstTab, SIGNAL(updateUserSelection(UiCompMeasData)),
                this, SIGNAL(updateUserSelection(UiCompMeasData)));
    }

    m_tabWidget->addTab(tabList[TestPlanEnum::Multiplue], tr("Multiplue"));
    connect(m_multiTstTab, SIGNAL(updateUserSelection(UiCompMeasData)),
            this, SIGNAL(updateUserSelection(UiCompMeasData)));

    m_tabWidget->addTab(tabList[TestPlanEnum::Aging], tr("Aging"));
    connect(m_agingTstTab, SIGNAL(updateUserSelection(UiCompMeasData)),
                                this, SIGNAL(updateUserSelection(UiCompMeasData)));

    //m_tabWidget->addTab(tabList[TestPlanEnum::Calibrate], tr("Calibrate"));
    m_tabWidget->addTab(tabList[TestPlanEnum::Manual], tr("Manual"));
    connect(m_manualTstTab, SIGNAL(updateUserSelection(UiCompMeasData)),
                                this, SIGNAL(updateUserSelection(UiCompMeasData)));

    connect(m_manualTstTab, SIGNAL(syncDataDuringManual(double,quint32)),
            this, SIGNAL(syncDataDuringManual(double,quint32)));
    //syncDataDuringManual

//    QGridLayout *settingsLayout = new QGridLayout();
//    settingsLayout->addWidget(chartSettings, 0, 0);
//    settingsLayout->setColumnStretch(0, 0);
//    settingsLayout->addWidget(m_tabWidget, 0, 1);
//    settingsLayout->setColumnStretch(1, 1);
//    settingsLayout->addWidget(seriesSettings, 0, 2);
//    settingsLayout->setColumnStretch(2, 0);
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(m_chartSettings, 0);
    settingsLayout->addWidget(m_tabWidget, 1);
    settingsLayout->addWidget(seriesSettings, 0);

    setLayout(settingsLayout);
}

QTabWidget *TestTab::tabWidget() const
{
    return m_tabWidget;
}

QGroupBox *TestTab::chartSettings() const
{
    return m_chartSettings;
}

void TestTab::enableLimitCheckBox(bool isEnabled)
{
    m_volLimCheckBox->setChecked(isEnabled);
    m_curLimCheckBox->setChecked(isEnabled);
    m_tempLimCheckBox->setChecked(isEnabled);
}

void TestTab::enableWidgetInFront(bool doshine)
{
    m_chartSettings->setEnabled(doshine);

    int activeIdx = m_tabWidget->currentIndex();
    if (activeIdx == -1 )
    {
        qWarning() << "UI getcurrentIndex = -1";
        return;
    }

    for (int idx = 0; idx < m_tabWidget->count(); ++idx)
    {
        m_tabWidget->setTabEnabled(idx, doshine);
    }

    /*
     * Manual Test Tab need special treatment.
    */
    if (activeIdx == (m_tabWidget->count() - 1))
    {
        m_tabWidget->setTabEnabled(m_tabWidget->count() - 1, true);
        m_manualTstTab->enableWidgetInFront(doshine);
    }

    m_tabWidget->setCurrentIndex(activeIdx);
}

UserSetSensitiveClz TestTab::getUserSetSensentive()
{
    UserSetSensitiveClz result;

    switch (m_sensitiveComboBox->currentIndex())
    {
    case 0:
    {
        result.rank = UserSetSensitiveClz::UserSetRankEnum::LOW;
    }
        break;
    case 1:
    {
        result.rank = UserSetSensitiveClz::UserSetRankEnum::MED;
    }
        break;
    case 2:
    {
        result.rank = UserSetSensitiveClz::UserSetRankEnum::HIGH;
    }
        break;
    default:
        qWarning() << "Error: not right selection on the sensitive";
        break;
    }

    if (m_volLimCheckBox->isChecked())
        result.volLimit = static_cast<quint32>(m_volLimitLineEdit->value());
    else
        result.volLimit = UINT32_MAX;

    if (m_curLimCheckBox->isChecked())
        result.curLimit = static_cast<quint32>(m_curLimitLineEdit->value());
    else
        result.curLimit = UINT32_MAX;

    if (m_tempLimCheckBox->isChecked())
        result.tempLimit = static_cast<quint32>(m_tempLimitLineEdit->value());
    else
        result.tempLimit = UINT32_MAX;

    return result;
}

quint32 TestTab::getGUIVolProtectionData() const
{
    quint32 result = 0;
    if (m_volLimCheckBox->isChecked())
        result = static_cast<quint32>(m_volLimitLineEdit->value());
    else
        result = UINT32_MAX;

    return result;
}

QPushButton *TestTab::start_btn() const
{
    return m_start_btn;
}

QPushButton *TestTab::showgraph_btn() const
{
    return m_showgraph_btn;
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
    CfgResHandlerInf* pCfgResHdl = UniResLocation::getCfgResHdl();

    // series settings
    m_voltage = new QDoubleSpinBox();
    m_voltage->setDecimals(1);
    m_voltage->setMinimumWidth(70);
    m_voltage->setMaximumWidth(70);
    m_voltage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage->setRange(0, pCfgResHdl->max_vol());
    //m_voltage->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_voltage->setSingleStep(ConstValue::STEP_VOL);
    m_voltage->setValue(ConstValue::DEFAULT_VOL);

    m_throttle = new QSpinBox();
    m_throttle->setMinimumWidth(70);
    m_throttle->setMaximumWidth(70);
    m_throttle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_throttle->setRange(0, pCfgResHdl->max_throttle());
    //m_throttle->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_throttle->setSingleStep(ConstValue::STEP_THR);
    m_throttle->setValue(ConstValue::DEFAULT_THR);

    m_disStart = new QSpinBox();
    m_disStart->setMinimumWidth(70);
    m_disStart->setMaximumWidth(70);
    m_disStart->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_disStart->setRange(ConstValue::DIS_START, pCfgResHdl->max_distance());
    m_disStart->setSingleStep(ConstValue::DIS_STEP);
    m_disStart->setValue(ConstValue::DIS_START);

    m_disEnd = new QSpinBox();
    m_disEnd->setMinimumWidth(70);
    m_disEnd->setMaximumWidth(70);
    m_disEnd->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_disEnd->setRange(ConstValue::DIS_START, pCfgResHdl->max_distance());
    m_disEnd->setSingleStep(ConstValue::DIS_STEP);
    m_disEnd->setValue(pCfgResHdl->max_distance());

    m_disStep = new QComboBox();
    m_disStep->setMinimumWidth(70);
    m_disStep->setMaximumWidth(70);
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis1));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis2));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis5));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis10));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis15));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis20));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis25));
    m_disStep->addItem(QString::number(ConstValue::DistanceStepStageEnum::SDis30));

    m_disStep->setCurrentIndex(3);

    m_duration = new QComboBox();
    m_duration->setMinimumWidth(70);
    m_duration->setMaximumWidth(70);
    m_duration->addItem(QString::number(ConstValue::DurationTime::D05Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D10Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D15Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D20Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D30Sec));
    m_duration->setCurrentIndex(1);

    m_apply_btn = new QPushButton(tr("Apply"));

    connect(m_apply_btn, SIGNAL(clicked(bool)), this, SLOT(validateUserInput(bool)));

    //connect(nextButton, &QAbstractButton::clicked, mapper, &QDataWidgetMapper::toNext);

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Vol(V)"), m_voltage);
    seriesSettingsLayout->addRow(tr("Thro (%)"), m_throttle);
    seriesSettingsLayout->addRow(tr("Dis Bg(mm)"), m_disStart);
    seriesSettingsLayout->addRow(tr("Dis Ed(mm)"), m_disEnd);
    seriesSettingsLayout->addRow(tr("Dis Sp(mm)"), m_disStep);
    seriesSettingsLayout->addRow(tr("Dur (sec)"), m_duration);
    seriesSettingsLayout->addRow(m_apply_btn);

    QFormLayout *outputListLayout = new QFormLayout();

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);
    horizonLayout->addLayout(outputListLayout, 1);

    setLayout(horizonLayout);
}

void DistanceTstTab::validateUserInput(bool checked)
{
    Q_UNUSED(checked)

    bool isLegal = true;
    QString illegalReason;

    quint32 volProtectionValue = TestTab::getTestTabInstance()->getGUIVolProtectionData();
    if(m_disStart->value() >= m_disEnd->value())
    {
        isLegal = false;
        illegalReason = tr("The START value must be larger than END value");
    }

    if (m_voltage->value() > volProtectionValue)
    {
        isLegal = false;
        illegalReason = tr("Input value must less than the vol Protection Value");
    }

    if (!isLegal)
    {
        QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                             illegalReason, QMessageBox::Close);
        warningBox.exec();
        return;
    }

    //DistanceTstData
    UiCompMeasData val;
    val.type = TestPlanEnum::Distance;

    DistanceTstData& data = val.data.w;
    data.vol = m_voltage->value();
    data.thro = m_throttle->value();
    data.dis_beg = m_disStart->value();
    data.dis_end = m_disEnd->value();
    data.dis_step = (quint16)m_disStep->currentText().toInt();
    data.duration = (quint16)m_duration->currentText().toInt();
    emit updateUserSelection(val);
}


VoltageTstTab::VoltageTstTab(QWidget *parent)
    : QWidget(parent)
{
    CfgResHandlerInf* pCfgResHdl = UniResLocation::getCfgResHdl();
    // series settings
    m_throttle = new QSpinBox();
    m_throttle->setMinimumWidth(70);
    m_throttle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_throttle->setRange(0, pCfgResHdl->max_throttle());
    //m_throttle->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_throttle->setSingleStep(ConstValue::STEP_THR);
    m_throttle->setValue(ConstValue::DEFAULT_THR);

    m_voltage_start = new QDoubleSpinBox();
    m_voltage_start->setDecimals(1);
    m_voltage_start->setMinimumWidth(70);
    m_voltage_start->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage_start->setRange(0, pCfgResHdl->max_vol());
    //m_voltage_start->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_voltage_start->setSingleStep(ConstValue::STEP_VOL);
    m_voltage_start->setValue(ConstValue::DEFAULT_VOL);

    m_voltage_end = new QDoubleSpinBox();
    m_voltage_end->setDecimals(1);
    m_voltage_end->setMinimumWidth(70);
    m_voltage_end->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage_end->setRange(0, pCfgResHdl->max_vol());
    //m_voltage_end->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_voltage_end->setSingleStep(ConstValue::STEP_VOL);
    m_voltage_end->setValue(ConstValue::MAX_VOL);

    m_voltage_step = new QDoubleSpinBox();
    m_voltage_step->setDecimals(1);
    m_voltage_step->setMinimumWidth(70);

    m_voltage_step->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage_step->setRange(0, pCfgResHdl->max_vol());
    m_voltage_step->setSingleStep(ConstValue::STEP_VOL);
    m_voltage_step->setValue(ConstValue::STEP_VOL);

//    m_voltage_step->addItem(QString::number(ConstValue::VolStepStageEnum::S1));
//    m_voltage_step->addItem(QString::number(ConstValue::VolStepStageEnum::S2));
//    m_voltage_step->addItem(QString::number(ConstValue::VolStepStageEnum::S5));
//    m_voltage_step->setCurrentIndex(0);

    m_duration = new QComboBox();
    m_duration->setMinimumWidth(70);
    m_duration->setMaximumWidth(70);
    m_duration->addItem(QString::number(ConstValue::DurationTime::D05Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D10Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D15Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D20Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D30Sec));
    m_duration->setCurrentIndex(1);

    m_apply_btn = new QPushButton(tr("Apply"));

    connect(m_apply_btn, SIGNAL(clicked(bool)), this, SLOT(validateUserInput(bool)));

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Thro (%)"), m_throttle);
    seriesSettingsLayout->addRow(tr("Vol Beg(V)"), m_voltage_start);
    seriesSettingsLayout->addRow(tr("Vol End(V)"), m_voltage_end);
    seriesSettingsLayout->addRow(tr("Vol Stp(V)"), m_voltage_step);
    seriesSettingsLayout->addRow(tr("Durs (Sec)"), m_duration);
    seriesSettingsLayout->addRow(m_apply_btn);

    QFormLayout *outputListLayout = new QFormLayout();

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);
    horizonLayout->addLayout(outputListLayout, 1);

    setLayout(horizonLayout);

    QObject::connect(m_apply_btn, &QPushButton::clicked, [this](bool checked){
        Q_UNUSED(checked)
    });
}

void VoltageTstTab::validateUserInput(bool checked)
{
    Q_UNUSED(checked)

    bool isLegal = true;
    QString illegalReason;

    quint32 volProtectionValue = TestTab::getTestTabInstance()->getGUIVolProtectionData();
    if (m_voltage_start->value() >= m_voltage_end->value())
    {
        isLegal = false;
        illegalReason = tr("The START value must be larger than END value");
    }

    if (m_voltage_end->value() > volProtectionValue)
    {
        isLegal = false;
        illegalReason = tr("Input value must less than the vol Protection Value");
    }

    if (!isLegal)
    {
        QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                             illegalReason, QMessageBox::Close);
        warningBox.exec();
        return;
    }


    UiCompMeasData val;
    val.type = TestPlanEnum::Voltage;
    VoltageTstData& data = val.data.u;

    data.thro = m_throttle->value();
    data.vol_beg = m_voltage_start->value();
    data.vol_end = m_voltage_end->value();
    data.vol_step = m_voltage_step->value();
    data.duration = (quint16)m_duration->currentText().toInt();
    emit updateUserSelection(val);
}

ThrottleTstTab::ThrottleTstTab(TestPlanEnum type, QWidget *parent)
    : QWidget(parent),
     m_type(type)
{
    CfgResHandlerInf* pCfgResHdl = UniResLocation::getCfgResHdl();
    // series settings
    m_voltage = new QDoubleSpinBox();
    m_voltage->setDecimals(1);
    m_voltage->setMinimumWidth(70);
    m_voltage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage->setRange(0, pCfgResHdl->max_vol());
    //m_voltage->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_voltage->setSingleStep(ConstValue::STEP_VOL);
    m_voltage->setValue(ConstValue::DEFAULT_VOL);

    m_thro_start = new QSpinBox();
    m_thro_start->setMinimumWidth(70);
    m_thro_start->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_thro_start->setRange(0, pCfgResHdl->max_throttle());
    //m_thro_start->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_thro_start->setSingleStep(ConstValue::STEP_THR);
    m_thro_start->setValue(ConstValue::DEFAULT_THR_LOW);

    m_thro_end = new QSpinBox();
    m_thro_end->setMinimumWidth(70);
    m_thro_end->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_thro_end->setRange(0, pCfgResHdl->max_throttle());
    //m_thro_end->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_thro_end->setSingleStep(ConstValue::STEP_THR);
    m_thro_end->setValue(ConstValue::DEFAULT_THR_HIGH);

    m_thro_step = new QComboBox();
    m_thro_step->setMinimumWidth(70);
    m_thro_step->addItem(QString::number(ConstValue::ThrottleStepStageEnum::SThr1));
    m_thro_step->addItem(QString::number(ConstValue::ThrottleStepStageEnum::SThr2));
    m_thro_step->addItem(QString::number(ConstValue::ThrottleStepStageEnum::SThr5));
    m_thro_step->setCurrentIndex(0);

    m_duration = new QComboBox();
    m_duration->setMinimumWidth(70);
    m_duration->setMaximumWidth(70);
    m_duration->addItem(QString::number(ConstValue::DurationTime::D05Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D10Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D15Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D20Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D30Sec));
    m_duration->setCurrentIndex(1);

    m_apply_btn = new QPushButton(tr("Apply"));

    connect(m_apply_btn, SIGNAL(clicked(bool)), this, SLOT(validateUserInput(bool)));


    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Vol (V)"), m_voltage);
    seriesSettingsLayout->addRow(tr("Thr Beg(%)"), m_thro_start);
    seriesSettingsLayout->addRow(tr("Thr End(%)"), m_thro_end);
    seriesSettingsLayout->addRow(tr("Thr Stp(%)"), m_thro_step);
    seriesSettingsLayout->addRow(tr("Durs (Sec)"), m_duration);
    seriesSettingsLayout->addRow(m_apply_btn);

    QFormLayout *outputListLayout = new QFormLayout();

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);
    horizonLayout->addLayout(outputListLayout, 1);

    setLayout(horizonLayout);
}

void ThrottleTstTab::validateUserInput(bool checked)
{
    Q_UNUSED(checked)

    bool isLegal = true;
    QString illegalReason;

    quint32 volProtectionValue = TestTab::getTestTabInstance()->getGUIVolProtectionData();
    if(m_thro_start->value() >= m_thro_end->value())
    {
        isLegal = false;
        illegalReason = tr("The START value must be larger than END value");
    }

    if (m_voltage->value() > volProtectionValue)
    {
        isLegal = false;
        illegalReason = tr("Input value must less than the vol Protection Value");
    }

    if (!isLegal)
    {
        QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                             illegalReason, QMessageBox::Close);
        warningBox.exec();
        return;
    }

    UiCompMeasData val;
    val.type = m_type;

    ThrottleTstData& data = val.data.v;
    data.vol = m_voltage->value();
    data.thro_beg = m_thro_start->value();
    data.thro_end = m_thro_end->value();
    data.thro_step = (quint16)m_thro_step->currentText().toInt();
    data.duration = (quint16)m_duration->currentText().toInt();
    emit updateUserSelection(val);
}


AgingTstTab::AgingTstTab(QWidget *parent)
    : QWidget(parent)
{
    CfgResHandlerInf* pCfgResHdl = UniResLocation::getCfgResHdl();
    // series settings
    m_throttle = new QSpinBox();
    m_throttle->setMinimumWidth(70);
    m_throttle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_throttle->setRange(0, pCfgResHdl->max_throttle());
    //m_throttle->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_throttle->setSingleStep(ConstValue::STEP_THR);
    m_throttle->setValue(ConstValue::DEFAULT_THR);

    m_voltage = new QDoubleSpinBox();
    m_voltage->setDecimals(1);
    m_voltage->setMinimumWidth(70);
    m_voltage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage->setRange(0, pCfgResHdl->max_vol());
    //m_voltage_start->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_voltage->setSingleStep(ConstValue::STEP_VOL);
    m_voltage->setValue(ConstValue::DEFAULT_VOL);

    m_duration = new QComboBox();
    m_duration->setMinimumWidth(70);
    m_duration->setMaximumWidth(70);
    m_duration->addItem(QString::number(ConstValue::DurationTime::D30Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D60Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D180Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D300Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D600Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D1200Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D1800Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D3600Sec));
    m_duration->setCurrentIndex(2);

    m_apply_btn = new QPushButton(tr("Apply"));

    connect(m_apply_btn, SIGNAL(clicked(bool)), this, SLOT(validateUserInput(bool)));

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Thro (%)"), m_throttle);
    seriesSettingsLayout->addRow(tr("Vol (V)"), m_voltage);
    seriesSettingsLayout->addRow(tr("Durs (Sec)"), m_duration);
    seriesSettingsLayout->addRow(m_apply_btn);

    QFormLayout *outputListLayout = new QFormLayout();

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);
    horizonLayout->addLayout(outputListLayout, 1);

    setLayout(horizonLayout);
}

void AgingTstTab::validateUserInput(bool checked)
{
    Q_UNUSED(checked)

    bool isLegal = true;
    QString illegalReason;

    quint32 volProtectionValue = TestTab::getTestTabInstance()->getGUIVolProtectionData();

    if (m_voltage->value() > volProtectionValue)
    {
        isLegal = false;
        illegalReason = tr("Input value must less than the vol Protection Value");
    }

    if (!isLegal)
    {
        QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                             illegalReason, QMessageBox::Close);
        warningBox.exec();
        return;
    }

    UiCompMeasData val;
    val.type = TestPlanEnum::Voltage;
    VoltageTstData& data = val.data.u;

    data.thro = m_throttle->value();
    data.vol_beg = m_voltage->value();
    data.vol_end = m_voltage->value() + 1.0;
    data.vol_step = 5.0;
    data.duration = (quint16)m_duration->currentText().toInt();
    emit updateUserSelection(val);
}

CalibrateTstTab::CalibrateTstTab(QWidget *parent)
    : QWidget(parent)
{

}

ManualTstTab::ManualTstTab(QWidget *parent)
    : QWidget(parent)
{
    CfgResHandlerInf* pCfgResHdl = UniResLocation::getCfgResHdl();
    // series settings
    m_spinbox_throttle = new QSpinBox();
    m_spinbox_throttle->setMinimumWidth(70);
    m_spinbox_throttle->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_spinbox_throttle->setRange(0, pCfgResHdl->max_throttle());
    //m_spinbox_throttle->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_spinbox_throttle->setSingleStep(ConstValue::STEP_THR);
    m_spinbox_throttle->setValue(ConstValue::DEFAULT_THR);
    connect(m_spinbox_throttle, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int value){
       m_throttle->setValue(value);
       emitValueChanged();
    });

    m_spinbox_voltage = new QSpinBox();
    //m_spinbox_voltage->setDecimals(1);
    m_spinbox_voltage->setMinimumWidth(70);
    m_spinbox_voltage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_spinbox_voltage->setRange(5, pCfgResHdl->max_vol());
    //m_spinbox_voltage->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_spinbox_voltage->setSingleStep(ConstValue::STEP_VOL);
    m_spinbox_voltage->setValue(ConstValue::DEFAULT_VOL);
    connect(m_spinbox_voltage, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [this](int value){
       m_voltage->setValue(value);
       emitValueChanged();
    });

    m_throttle = new QSlider(Qt::Orientation::Horizontal);
    m_throttle->setMinimumWidth(70);
    m_throttle->setFocusPolicy(Qt::StrongFocus);
    m_throttle->setTickPosition(QSlider::TicksBothSides);
    m_throttle->setTickInterval(10);
    m_throttle->setSingleStep(ConstValue::STEP_THR);
    m_throttle->setMinimum(0);
    m_throttle->setMaximum(pCfgResHdl->max_throttle());
    m_throttle->setValue(ConstValue::DEFAULT_THR);
    connect(m_throttle, &QSlider::valueChanged, [this](int value){
       m_spinbox_throttle->setValue(value);
       emitValueChanged();
    });

    m_voltage = new QSlider(Qt::Orientation::Horizontal);
    m_voltage->setMinimumWidth(70);
    m_voltage->setFocusPolicy(Qt::StrongFocus);
    m_voltage->setTickPosition(QSlider::TicksBothSides);
    m_voltage->setTickInterval(10);
    m_voltage->setSingleStep(ConstValue::STEP_VOL);
    m_voltage->setMinimum(5);
    m_voltage->setMaximum(pCfgResHdl->max_vol());
    m_voltage->setValue(ConstValue::DEFAULT_VOL);
    connect(m_voltage, &QSlider::valueChanged, [this](int value){
       m_spinbox_voltage->setValue(value);
       emitValueChanged();
    });

    m_apply_btn = new QPushButton(tr("Apply"));
    //m_set_btn = new QPushButton(tr("Set"));

    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Thro (%)"), m_spinbox_throttle);
    seriesSettingsLayout->addRow(m_throttle);
    seriesSettingsLayout->addRow(tr("Vol (V)"), m_spinbox_voltage);
    seriesSettingsLayout->addRow(m_voltage);
    seriesSettingsLayout->addRow(m_apply_btn);
    //seriesSettingsLayout->addRow(m_set_btn);

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);

    setLayout(horizonLayout);

    QObject::connect(m_apply_btn, &QPushButton::clicked, [this](bool checked){
        Q_UNUSED(checked)

        bool isLegal = true;
        QString illegalReason;

        quint32 volProtectionValue = TestTab::getTestTabInstance()->getGUIVolProtectionData();
        if (m_voltage->value() > volProtectionValue)
        {
            isLegal = false;
            illegalReason = tr("Input value must less than the vol Protection Value");
        }

        if (!isLegal)
        {
            QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                                 illegalReason, QMessageBox::Close);
            warningBox.exec();
            return;
        }

        UiCompMeasData val;
        val.type = TestPlanEnum::Manual;
        ManualTstData& data = val.data.y;

        data.thro = m_throttle->value();
        data.vol = m_voltage->value();
        emit updateUserSelection(val);
    });

//    QObject::connect(m_set_btn, &QPushButton::clicked, [this](bool checked){
//        Q_UNUSED(checked)

//        emit syncDataDuringManual(m_voltage->value(), m_throttle->value());
//    });
}

void ManualTstTab::enableWidgetInFront(bool doshine)
{
    m_throttle->setEnabled(true);
    m_voltage->setEnabled(true);
    //m_set_btn->setEnabled(true);
    m_apply_btn->setEnabled(doshine);
}

void ManualTstTab::emitValueChanged()
{
    emit syncDataDuringManual(m_voltage->value(), m_throttle->value());
}


MultiTstTab::MultiTstTab(TestPlanEnum type, QWidget *parent)
    : QWidget(parent),
     m_type(type)
{
    CfgResHandlerInf* pCfgResHdl = UniResLocation::getCfgResHdl();
    // series settings
    m_voltage = new QDoubleSpinBox();
    m_voltage->setDecimals(1);
    m_voltage->setMinimumWidth(70);
    m_voltage->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_voltage->setRange(0, pCfgResHdl->max_vol());
    //m_voltage->setRange(ConstValue::MIN_THR, ConstValue::MAX_THR);
    m_voltage->setSingleStep(ConstValue::STEP_VOL);
    m_voltage->setValue(ConstValue::DEFAULT_VOL);

    m_thro_end = new QSpinBox();
    m_thro_end->setMinimumWidth(70);
    m_thro_end->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    m_thro_end->setRange(0, pCfgResHdl->max_throttle());
    //m_thro_end->setRange(ConstValue::MIN_VOL, ConstValue::MAX_VOL);
    m_thro_end->setSingleStep(ConstValue::STEP_THR);
    m_thro_end->setValue(ConstValue::DEFAULT_THR_HIGH);

    m_duration = new QComboBox();
    m_duration->setMinimumWidth(70);
    m_duration->setMaximumWidth(70);
    m_duration->addItem(QString::number(ConstValue::DurationTime::D05Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D10Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D15Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D20Sec));
    m_duration->addItem(QString::number(ConstValue::DurationTime::D30Sec));
    m_duration->setCurrentIndex(1);

    m_apply_btn = new QPushButton(tr("Apply"));

    connect(m_apply_btn, SIGNAL(clicked(bool)), this, SLOT(validateUserInput(bool)));


    QFormLayout *seriesSettingsLayout = new QFormLayout();
    seriesSettingsLayout->addRow(tr("Vol (V)"), m_voltage);
    seriesSettingsLayout->addRow(tr("Thr End(%)"), m_thro_end);
    seriesSettingsLayout->addRow(tr("Durs (Sec)"), m_duration);
    seriesSettingsLayout->addRow(m_apply_btn);

    QFormLayout *outputListLayout = new QFormLayout();

    QHBoxLayout *horizonLayout = new QHBoxLayout();
    horizonLayout->addLayout(seriesSettingsLayout, 0);
    horizonLayout->addLayout(outputListLayout, 1);

    setLayout(horizonLayout);
}

void MultiTstTab::validateUserInput(bool checked)
{
    Q_UNUSED(checked)

    bool isLegal = true;
    QString illegalReason;

    quint32 volProtectionValue = TestTab::getTestTabInstance()->getGUIVolProtectionData();

    if (m_voltage->value() > volProtectionValue)
    {
        isLegal = false;
        illegalReason = tr("Input value must less than the vol Protection Value");
    }

    if (!isLegal)
    {
        QMessageBox warningBox(QMessageBox::Warning, tr("Warning"),
                             illegalReason, QMessageBox::Close);
        warningBox.exec();
        return;
    }

    UiCompMeasData val;
    val.type = m_type;

    ThrottleTstData& data = val.data.v;
    data.vol = m_voltage->value();
    data.thro_beg = 1;
    data.thro_end = m_thro_end->value();
    data.thro_step = 1;
    data.duration = (quint16)m_duration->currentText().toInt();
    emit updateUserSelection(val);
}
