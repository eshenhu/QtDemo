#ifndef OPTIONSFRAME_H
#define OPTIONSFRAME_H

#include <QFrame>

class CfgMotorBootCfgModel;

namespace Ui {
class OptionsFrame;
}

class OptionsFrame : public QFrame
{
    Q_OBJECT

public:
    enum DURATION {
        D2S = 2,
        D5S = 5,
        D10S = 10,
        D30S = 30,
        D60S = 60 };

    static const quint32 DEFAULT_BOOT_VOL = 10;

    struct Settings {
        int duration = DURATION::D30S;
        int startDelay = DURATION::D5S;
        int softDelay = DURATION::D10S;
    };

public:
    explicit OptionsFrame(CfgMotorBootCfgModel* cfg, QWidget *parent = 0);
    ~OptionsFrame();

private:
    Ui::OptionsFrame *ui;
    CfgMotorBootCfgModel* m_cfg;
};

#endif // OPTIONSFRAME_H
