#ifndef TESTTAB_H
#define TESTTAB_H

#include <QWidget>
#include "ui/uiheader.h"

class QSpinBox;
class QDialogButtonBox;
class QCheckBox;
class QComboBox;
class QPushButton;
class QTabWidget;
class CfgResHandlerInf;

class DistanceTstTab : public QWidget
{
    Q_OBJECT
public:
    QSpinBox *m_voltage;
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
    QSpinBox *m_voltage_start;
    QSpinBox *m_voltage_end;
    QComboBox *m_voltage_step;
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
    QSpinBox* m_voltage;
    QSpinBox* m_thro_start;
    QSpinBox* m_thro_end;
    QComboBox* m_thro_step;
    QComboBox *m_duration;
    QPushButton *m_apply_btn;

public:
    explicit ThrottleTstTab(TestCasePrimType type, QWidget *parent = 0);

Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);

public slots:
    void validateUserInput(bool checked = false);

private:
    TestCasePrimType m_type;
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

class TestTab : public QWidget
{
    Q_OBJECT

public:
    explicit TestTab(QWidget *parent = 0);

public:
    QTabWidget *m_tabWidget;
    QWidget* tabList[TestPlanEnum::Manual + 1];
    QWidget* lastActiveWidget;

    const QPushButton *start_btn() const;

    const QPushButton *showgraph_btn() const;

private:
    //QComboBox *m_testSeletionComboBox;
    QCheckBox *m_volCheckBox;
    QCheckBox *m_animationsCheckBox;
    QCheckBox *m_legendCheckBox;

    QDialogButtonBox* m_buttonBox;

    QPushButton* m_start_btn;
    QPushButton* m_showgraph_btn;

    VoltageTstTab* m_volTstTab;
    ThrottleTstTab* m_throTstTab;
    DistanceTstTab* m_disTstTab;
    ThrottleTstTab* m_multiTstTab;


Q_SIGNALS:
    void updateUserSelection(UiCompMeasData data);

private:
    void enableTestTab(TestPlanEnum);
};



#endif // TESTTAB_H
