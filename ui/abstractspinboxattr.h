#ifndef ABSTRACTSPINBOXATTR_H
#define ABSTRACTSPINBOXATTR_H

#include "basedspinboxattrinf.h"

class AbstractSpinBoxAttr : public BasedSpinBoxAttrInf
{
public:
    //Q_INTERFACES(BasedSpinBoxAttrInf)

    AbstractSpinBoxAttr() = default;
    AbstractSpinBoxAttr(const JsonPVConfig& config, const JsonGUIElement& ele);
    ~AbstractSpinBoxAttr(){}

    // BasedSpinBoxAttrInf interface
public:
    JsonGUIPrimType type() const override
    {
        return m_elem.type();
    }
    QString str() const override
    {
        return m_elem.str();
    }
    indexOnMotor idx() const override
    {
        return m_elem.idx();
    }
    qint16 lowLimit() const override
    {
        return m_elem.lowLimit();
    }
    qint16 upLimit() const override
    {
        return m_elem.upLimit();
    }
    QString unit() const override
    {
        return m_elem.unit();
    }
    bool isSelected() const override
    {
        return m_elem.isSelected();
    }

    virtual void update(const QModbus2DataUnit *data, Phase phase) override;

    double pushData() const;

private:
    JsonPVConfig   m_config;
    JsonGUIElement m_elem;
    qint32 m_data;
    double m_pushData;
};

#endif // ABSTRACTSPINBOXATTR_H
