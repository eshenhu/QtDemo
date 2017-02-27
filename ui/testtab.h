#ifndef TESTTAB_H
#define TESTTAB_H

#include <QWidget>
#include "ui/uiheader.h"

class QSpinBox;
class QDoubleSpinBox;
class QDialogButtonBox;
class QCheckBox;
class QComboBox;
class QPushButton;
class QTabWidget;
class CfgResHandlerInf;
class QGroupBox;
class QLineEdit;
class QFormLayout;
class QSlider;
class DistanceTstTab : public QWidget
{
    Q_OBJECT
public:
    QDoubleSpinBox *m_voltage;
    QSpinBox *m_throttle;
    QSpinBox *m_disStart;
    QSpinBox *m_disEnd;
    QComboBox *m_disStep;
    QComboBox *m_duration;
    QPushButton *m_apply_btn;

public:
    explicit DistanceTstTab(QWidget *parent = 0);

Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);

private slots:
    void validateUserInput(bool checked = false);
};

class VoltageTstTab : public QWidget
{
    Q_OBJECT
private:
    QSpinBox *m_throttle;
    QDoubleSpinBox *m_voltage_start;
    QDoubleSpinBox *m_voltage_end;
    QDoubleSpinBox *m_voltage_step;
    QComboBox *m_duration;
    QPushButton *m_apply_btn;

public:
    explicit VoltageTstTab(QWidget *parent = 0);

Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);

public slots:
    void validateUserInput(bool checked = false);
};


class ThrottleTstTab : public QWidget
{
    Q_OBJECT
    QDoubleSpinBox* m_voltage;
    QSpinBox* m_thro_start;
    QSpinBox* m_thro_end;
    QComboBox* m_thro_step;
    QComboBox *m_duration;
    QPushButton *m_apply_btn;

public:
    explicit ThrottleTstTab(TestPlanEnum type, QWidget *parent = 0);

Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);

public slots:
    void validateUserInput(bool checked = false);

private:
    TestPlanEnum m_type;
};

class MultiTstTab : public QWidget
{
    Q_OBJECT
public:
    QDoubleSpinBox* m_voltage;
    QSpinBox* m_thro_end;
    QComboBox *m_duration;
    QPushButton *m_apply_btn;
    QFormLayout *seriesSettingsLayout;
public:
    explicit MultiTstTab(TestPlanEnum type, QWidget *parent = 0);

Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);

public slots:
    void validateUserInput(bool checked = false);

private:
    TestPlanEnum m_type;
};

class AgingTstTab : public QWidget
{
    Q_OBJECT
private:
    QSpinBox *m_throttle;
    QDoubleSpinBox *m_voltage;
    QComboBox *m_duration;
    QPushButton *m_apply_btn;

public:
    explicit AgingTstTab(QWidget *parent = 0);

Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);

public slots:
    void validateUserInput(bool checked = false);
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
private:
    QSpinBox *m_spinbox_throttle;
    QSpinBox *m_spinbox_voltage;
    QSlider *m_throttle;
    QSlider *m_voltage;
    QPushButton *m_set_btn;
    QPushButton *m_apply_btn;

public:
    explicit ManualTstTab(QWidget *parent = 0);
    void enableWidgetInFront(bool doshine);
private:
    void emitValueChanged();

Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);
    void syncDataDuringManual(const double vol, const quint32 thro);
};

/*
 * Attention: ManualTestTab must be laid on the last tab list in m_tabWidget!!!
 * with the reason of dim the GUI when test is ongoing.
 *
 * It will be preferred use the std::map to store this relationship.
 *
*/
class TestTab : public QWidget
{
    Q_OBJECT

public:
    explicit TestTab(QWidget *parent = 0);
    static TestTab* getTestTabInstance() {
        static TestTab* tab = new TestTab();
        return tab;
    }

public:
    QTabWidget *m_tabWidget;
    QWidget* tabList[TestPlanEnum::Manual + 1];

    QPushButton *start_btn() const;
    QPushButton *showgraph_btn() const;
    QTabWidget *tabWidget() const;
    QGroupBox *chartSettings() const;

    void enableLimitCheckBox(bool);
    void enableWidgetInFront(bool doshine);

    UserSetSensitiveClz getUserSetSensentive();
    quint32 getGUIVolProtectionData() const;

private:
    //QComboBox *m_testSeletionComboBox;
    QCheckBox *m_volLimCheckBox;
    QSpinBox* m_volLimitLineEdit;
    QCheckBox *m_curLimCheckBox;
    QSpinBox* m_curLimitLineEdit;
    QCheckBox *m_tempLimCheckBox;
    QSpinBox* m_tempLimitLineEdit;

    QComboBox* m_sensitiveComboBox;

    QGroupBox *m_chartSettings;

    QDialogButtonBox* m_buttonBox;

    QPushButton* m_start_btn;
    QPushButton* m_showgraph_btn;

    VoltageTstTab* m_volTstTab;
    ThrottleTstTab* m_throTstTab;
    DistanceTstTab* m_disTstTab = nullptr;
    MultiTstTab* m_multiTstTab;

    AgingTstTab* m_agingTstTab;
    ManualTstTab* m_manualTstTab;

//    QIcon m_playIcon = QIcon(":/play.png");
//    QIcon m_pauseIcon = QIcon(":/play.png");


Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);
    void syncDataDuringManual(const double vol, const quint32 thro);

private:
    void enableTestTab(TestPlanEnum);
};



#endif // TESTTAB_H
