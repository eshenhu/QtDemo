#ifndef TESTTAB_H
#define TESTTAB_H

#include <QWidget>

class QSpinBox;
class QDialogButtonBox;
class QCheckBox;
class QComboBox;
class QPushButton;
class QTabWidget;

enum TestPlanEnum {
    Distance,
    Voltage,
    Throttle,
    Multiplue,
    Aging,
    Calibrate,
    Manual
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

#endif // TESTTAB_H
