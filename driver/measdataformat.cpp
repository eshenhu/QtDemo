#include "measdataformat.h"
#include <climits>
#include <QtDebug>

MeasDataFormat::MeasDataFormat()
{
    vol = 0;
    thro_1 = 0;
    thro_2 = 0;
    dis = 0xFFFFFFFF;
}

quint32 MeasDataFormat::getVol() const
{
    return vol;
}

void MeasDataFormat::setVol(const quint32 &value)
{
    vol = value;
}

quint32 MeasDataFormat::getThro_1() const
{
    return thro_1;
}

void MeasDataFormat::setThro_1(const quint32 &value)
{
    thro_1 = value;
}

quint32 MeasDataFormat::getThro_2() const
{
    return thro_2;
}

void MeasDataFormat::setThro_2(const quint32 &value)
{
    thro_2 = value;
}

quint32 MeasDataFormat::getDis() const
{
    return dis;
}

void MeasDataFormat::setDis(const quint32 &value)
{
    dis = value;
}

AbstractPeriodicalMeasDataUpdate::AbstractPeriodicalMeasDataUpdate(const quint32 delay_start, const quint32 soft_delay, const quint32 boot_voltage,
                                                                   const quint32 durationInSec, const quint32 intervalInMSec):
    m_tick(0),
    m_delay_start(delay_start),
    m_soft_delay(soft_delay),
    m_boot_voltage(boot_voltage),
    m_durationInSec(durationInSec),
    m_intervalInMSec(intervalInMSec),
    m_phase(Phase::Phase_SoftStart)
{
}

bool AbstractPeriodicalMeasDataUpdate::update()
{
    bool rtn = false;

    if (m_tick < UINT_MAX)
    {
        ++m_tick;
    }

    /*            ------------
     *           /
     *          /
     * ---------
     *
     * delay   soft
     */

    if (m_phase == Phase::Phase_SoftStart)
    {
        if (m_tick <= 1000 * m_delay_start/m_intervalInMSec)
        {
            m_data->setVol(0);
        }
        else if (m_tick <= 1000 * (m_delay_start + m_soft_delay)/m_intervalInMSec)
        {
            static quint32 step = 1000 * m_soft_delay / m_intervalInMSec;
            quint32 vol = m_boot_voltage * (m_tick - 1000 * m_delay_start/m_intervalInMSec) / step;
            m_data->setVol(vol);
        }
        else
        {
            m_tick = 0;
            this->updateValue();
            m_phase = Phase::Phase_NomalRunning;
        }
    }
    else
    {
        if (0 == (m_tick % (1000 * m_durationInSec/m_intervalInMSec)))
        {
            if(this->updateValue())
                rtn = true;
        }
    }
//    qInfo() << "util.measdataformat tick=" << m_tick << "m_phase=" << (quint8)m_phase
//            << "vol=" << m_data->getVol() << "thro_1" << m_data->getThro_1();
    return rtn;
}

PeriodicalVolMeasDataUpdate::PeriodicalVolMeasDataUpdate(const quint32 start, const quint32 end, const quint32 step, const quint32 thro,
                                                         const quint32 delay_start, const quint32 soft_delay, const quint32 boot_voltage,
                                                         const quint32 durationInSec, const quint32 intervalInMSec):
    AbstractPeriodicalMeasDataUpdate(delay_start, soft_delay, boot_voltage, durationInSec, intervalInMSec),
    m_start_vol(start),
    m_end_vol(end),
    m_thro(thro),
    m_calc_value(start)
{
    if (m_start_vol > m_end_vol)
    {
        m_step = 0;
        qWarning() << "com.meas.driver - PeriodicalVolMeasDataUpdate m_start_vol" << m_start_vol << "< m_end_vold" << m_end_vol;
    }
    else
    {
        m_step = step;
    }
}

bool PeriodicalVolMeasDataUpdate::updateValue()
{
    bool rtn = false;
    if (m_start_vol <= m_end_vol)
    {
        m_data->setVol(m_calc_value);
        m_data->setThro_1(m_thro);
        m_data->setThro_2(m_thro);
        m_data->setDis(UINT_MAX);

        m_calc_value += m_step;
        if(m_calc_value > m_end_vol)
        {
            rtn = true;
        }
    }
    else
    {
        rtn = true;
    }
    return rtn;
}

PeriodicalThroMeasDataUpdate::PeriodicalThroMeasDataUpdate(const quint32 start, const quint32 end, const quint32 step, const quint32 vol,
                                                           const quint32 delay_start, const quint32 soft_delay, const quint32 boot_voltage,
                                                           const quint32 durationInSec, const quint32 intervalInMSec)
    :AbstractPeriodicalMeasDataUpdate(delay_start, soft_delay, boot_voltage, durationInSec, intervalInMSec),
      m_start_thro(start),
      m_end_thro(end),
      m_step(0),
      m_vol(vol),
      m_calc_value(start)
{
    if (m_start_thro > m_end_thro)
    {
        qWarning() << "com.meas.driver - PeriodicalThroMeasDataUpdate m_start_thro" << m_start_thro << "< m_end_thro" << m_end_thro;
    }
    else
    {
        m_step = step;
    }
}

bool PeriodicalThroMeasDataUpdate::updateValue()
{
    bool rtn = false;
    if (m_start_thro <= m_end_thro)
    {
        m_data->setVol(m_vol);
        m_data->setThro_1(m_start_thro);
        m_data->setThro_2(m_start_thro);
        m_data->setDis(UINT_MAX);

        m_calc_value += m_step;
        if(m_calc_value > m_end_thro)
        {
            rtn = true;
        }
    }
    else
    {
        rtn = true;
    }
    return rtn;
}
