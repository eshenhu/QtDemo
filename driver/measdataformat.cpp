#include "measdataformat.h"
#include <climits>
#include <QtDebug>
//#include <actionwidget.h>
#include "cfg/datajsonrecelement.h"

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

/*
 *   Protocol:
 *       Voltage was preseted with 16bit. 0xFFFF represet the maximum
 *       supported voltage value.
 *       f.g. in PV11 version, the max vol is 55V, then 0xFFFF represet
 *            55V.
 */
void MeasDataFormat::setVol(const quint32 &value)
{
//    CfgResHandlerInf* cfgHandler = ActionWidget::getCfgResHdl();
//    quint32 max_value = cfgHandler->max_vol();
//    vol = (65535 / max_value) * value;
    vol = 100 * value;
}

void MeasDataFormat::setVol(const double &value)
{
    vol = static_cast<quint32>(100 * value);
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

void MeasDataFormat::reset()
{
    vol = thro_1 = thro_2 = 0;
    dis = 0xFFFFFFFF;
}

AbstractPeriodicalMeasDataUpdate::AbstractPeriodicalMeasDataUpdate(const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const quint32 boot_voltage,
                                                                   const quint32 boot_thro, const quint32 durationInSec, const quint32 intervalInMSec):
    m_tick(0),
    m_delay_start(delay_start),
    m_PRP_delay(PRP_delay),
    m_soft_delay(soft_delay),
    m_boot_voltage(boot_voltage),
    m_boot_thro(boot_thro),
    m_durationInSec(durationInSec),
    m_intervalInMSec(intervalInMSec),
    m_phase(Phase::Phase_SoftStart)
{
}
bool AbstractPeriodicalMeasDataUpdate::update()
{
    bool rtn = this->updateData();

    DataJsonRecElementE2& e2 = DataJsonRecElementE2::DataJsonRecElementE2GetHelper().getElem();
    e2.setMetaData(m_data->getVol(), m_data->getThro_1(), m_data->getThro_2(), m_data->getDis());

    return rtn;
}

bool AbstractPeriodicalMeasDataUpdate::updateData()
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
            //m_data->setVol(0);
            m_data->setThro_1(0);
            m_data->setThro_2(0);

            static quint32 step = 1000 * m_delay_start / m_intervalInMSec;
            quint32 vol = m_boot_voltage * m_tick / step;
            m_data->setVol(vol);
        }
        else if (m_tick <= 1000 * (m_delay_start + m_PRP_delay)/m_intervalInMSec)
        {
            // do nothing here, keep as previous.
        }
        else if (m_tick <= 1000 * (m_delay_start + m_PRP_delay +m_soft_delay)/m_intervalInMSec)
        {
            static quint32 step = 1000 * m_soft_delay / m_intervalInMSec;
            quint32 thro = m_boot_thro * (m_tick - 1000 * (m_delay_start + m_PRP_delay)/m_intervalInMSec) / step;
            //m_data->setVol(vol);
            m_data->setThro_1(thro);
            m_data->setThro_2(thro);
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
    qInfo() << "util.measdataformat tick=" <<  m_tick << "m_phase=" << (quint8)m_phase
            << "vol=" << m_data->getVol() << "thro_1" << m_data->getThro_1()
            << "m_delay_start" << m_delay_start << "m_PRP_delay" << m_PRP_delay
            << "m_soft_delay" << m_soft_delay;


    return rtn;
}

PeriodicalVolMeasDataUpdate::PeriodicalVolMeasDataUpdate(const double start, const double end, const double step, const quint32 thro,
                                                         const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const quint32 boot_voltage,
                                                         const quint32 durationInSec, const quint32 intervalInMSec):
    AbstractPeriodicalMeasDataUpdate(delay_start, PRP_delay, soft_delay, boot_voltage, thro, durationInSec, intervalInMSec),
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
        if(m_calc_value > (m_end_vol + m_step))
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
                                                           const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const quint32 boot_voltage,
                                                           const quint32 durationInSec, const quint32 intervalInMSec)
    :AbstractPeriodicalMeasDataUpdate(delay_start, PRP_delay,soft_delay, boot_voltage, start, durationInSec, intervalInMSec),
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
        m_data->setThro_1(m_calc_value);
        m_data->setThro_2(m_calc_value);
        m_data->setDis(UINT_MAX);

        m_calc_value += m_step;
        if(m_calc_value > (m_end_thro + m_step))
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

PeriodicalDisMeasDataUpdate::PeriodicalDisMeasDataUpdate(const quint32 start, const quint32 end, const quint32 step, const quint32 vol, const quint32 thro,
                                                         const quint32 delay_start, const quint32 PRP_delay, const quint32 soft_delay, const quint32 boot_voltage,
                                                         const quint32 durationInSec, const quint32 intervalInMSec)
    :AbstractPeriodicalMeasDataUpdate(delay_start, PRP_delay,soft_delay, boot_voltage, thro, durationInSec, intervalInMSec),
      m_step(step),
      m_start_dis(start),
      m_end_dis(end),
      m_vol(vol),
      m_thro(thro),
      m_calc_value(start)
{
}

bool PeriodicalDisMeasDataUpdate::updateValue()
{
    bool rtn = false;
    if (m_start_dis <= m_end_dis)
    {
        m_data->setVol(m_vol);
        m_data->setThro_1(m_thro);
        m_data->setThro_2(m_thro);
        m_data->setDis(m_calc_value);

        m_calc_value += m_step;
        if(m_calc_value > (m_end_dis + m_step))
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
