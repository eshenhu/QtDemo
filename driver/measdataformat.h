#ifndef MEASDATAFORMAT_H
#define MEASDATAFORMAT_H

#include <qglobal.h>
#include "ui/uiheader.h"
#include "driver/modelpoctype.h"

using namespace ModelPOC;

class MeasDataFormat
{
public:
    MeasDataFormat();
    ~MeasDataFormat(){}

    quint32 getVol() const;
    //void setVol(const quint32 &value);
    void setVol(const double &value);

    quint32 getThro_1() const;
    void setThro_1(const quint32 &value);

    quint32 getThro_2() const;
    void setThro_2(const quint32 &value);

    quint32 getDis() const;
    void setDis(const quint32 &value);

    void reset();

private:
    quint32 vol;
    quint32 thro_1;
    quint32 thro_2;
    quint32 dis;
};


class MeasDataUpdateInf
{
public:
    virtual ~MeasDataUpdateInf() {}

    virtual bool update() = 0;
    virtual Phase phase() const = 0;
    virtual void setSeed(MeasDataFormat* data) = 0;
};

//Q_DECLARE_INTERFACE(MeasDataUpdateInf, "com.meas.driver.MeasDataUpdateInf/1.0")

class AbstractMeasDataUpdate : public MeasDataUpdateInf
{
public:
    AbstractMeasDataUpdate(){}
    virtual ~AbstractMeasDataUpdate() {}

    // MeasDataUpdateInf interface
public:
    void setSeed(MeasDataFormat* data) override {
        m_data = data;
    }

protected:
    MeasDataFormat* m_data = nullptr;
};

class AbstractPeriodicalMeasDataUpdate : public AbstractMeasDataUpdate
{
//    enum struct Phase
//    {
//        Phase_SoftStart,
//        Phase_PRPDelay,
//        Phase_HardDelay,
//        Phase_NomalRunning
//    };

public:
    AbstractPeriodicalMeasDataUpdate(const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const double boot_voltage,
                                     const quint32 boot_thro, const quint32 durationInSec, const quint32 intervalInMSec = 500);
    virtual ~AbstractPeriodicalMeasDataUpdate() {m_phase = Phase::Phase_SoftStart;}

private:
    quint32 m_tick;
    quint32 m_delay_start;
    quint32 m_PRP_delay;
    quint32 m_soft_delay;
    double m_boot_voltage;
    quint32 m_boot_thro;
    quint32 m_durationInSec;
    quint32 m_intervalInMSec;
    Phase m_phase;

public:
    virtual bool updateValue() = 0;   //This virtual function will be called after one duration time (5Sec) eclapsed.

    // MeasDataUpdateInf interface
public:
    /*
     * return:
     *     true: indicate finished
     *     false: ongoing test
     */
    bool update() override;

    Phase phase() const override;

private:
    bool updateData();
};

class PeriodicalVolMeasDataUpdate : public AbstractPeriodicalMeasDataUpdate
{
public:
    PeriodicalVolMeasDataUpdate(const double start, const double end, const double step, const quint32 thro,
                                const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const double boot_voltage,
                                const quint32 durationInSec, const quint32 intervalInMSec = INTERVALS_MS);
    ~PeriodicalVolMeasDataUpdate(){}

private:
    double m_step;
    const double m_start_vol;
    const double m_end_vol;
    const quint32 m_thro;
    double m_calc_value;

    // MeasDataUpdateInf interface
public:
    bool updateValue() override;
};

class PeriodicalThroMeasDataUpdate : public AbstractPeriodicalMeasDataUpdate
{
public:
    PeriodicalThroMeasDataUpdate(const quint32 start, const quint32 end, const quint32 step, const double vol,
                                 const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const double boot_voltage,
                                 const quint32 durationInSec, const quint32 intervalInMSec = INTERVALS_MS);
    ~PeriodicalThroMeasDataUpdate(){}

private:
    const quint32 m_start_thro;
    const quint32 m_end_thro;
    quint32 m_step;
    const double m_vol;
    quint32 m_calc_value;

    // MeasDataUpdateInf interface
public:
    bool updateValue() override;
};

class PeriodicalDisMeasDataUpdate : public AbstractPeriodicalMeasDataUpdate
{
public:
    PeriodicalDisMeasDataUpdate(const quint32 start, const quint32 end, const quint32 step, const double vol, const quint32 thro,
                                const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const double boot_voltage,
                                const quint32 durationInSec, const quint32 intervalInMSec = INTERVALS_MS);
    ~PeriodicalDisMeasDataUpdate(){}

private:
    const quint32 m_step;
    const quint32 m_start_dis;
    const quint32 m_end_dis;
    const double m_vol;
    const quint32 m_thro;
    quint32 m_calc_value;

    // MeasDataUpdateInf interface
public:
    bool updateValue() override;
};

/*
 * it was perferred make a new base class to do such job. --eshenhu
*/
class OneShotManualMeasDataUpdate : public AbstractPeriodicalMeasDataUpdate
{
public:
    OneShotManualMeasDataUpdate(const double vol, const quint32 thro,
                                const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const double boot_voltage,
                                const quint32 durationInSec = 5, const quint32 intervalInMSec = INTERVALS_MS);
    ~OneShotManualMeasDataUpdate(){}

private:
    double  m_vol;
    quint32 m_thro;

public:
    void updateValueManually(const double vol, const quint32 thro)
    {
        m_vol = vol;
        m_thro = thro;
    }

    // MeasDataUpdateInf interface
public:
    bool updateValue() override;
};
#endif // MEASDATAFORMAT_H
