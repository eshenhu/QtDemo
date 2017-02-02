#ifndef CFGJSONRECELEMENT_H
#define CFGJSONRECELEMENT_H

#include "cfg/cfgreshandlerinf.h"
#include "ui/uiheader.h"
#include "comm/qmodbusdataunit.h"
#include <QString>
class QFile;

class CfgJsonRecElement
{
public:
    CfgJsonRecElement() = default;
    //CfgJsonRecElement(const QFile&);
public:
    bool loadCfg(const QString&);
    bool saveCfg(const QString&);

private:
    bool read(const QJsonObject &json);
    void write(QJsonObject &json) const;

public:
    class CfgJsonRecElementBuilder;

    QModbus2DataUnit::MotorTypeEnum motorType() const;

    quint8 numOfMotor() const;

    TestPlanEnum plan() const;

    CfgResHandlerInf::ProductVersion pv() const;

private:
    CfgJsonRecElement(const CfgJsonRecElement::CfgJsonRecElementBuilder& builder);

private:
    CfgResHandlerInf::ProductVersion m_pv;
    QString m_manufacture = QStringLiteral("tongyi");
    quint8  m_vanes = 2;
    TestPlanEnum m_plan = TestPlanEnum::Invaild;
    quint8  m_numOfMotor = 0;
    QModbus2DataUnit::MotorTypeEnum m_motorType;
    QString m_timeStamp;
};

class CfgJsonRecElement::CfgJsonRecElementBuilder
{
public:
    CfgJsonRecElementBuilder(){
        m_pv = CfgResHandlerInf::ProductVersion::INVALID;
    }

    CfgJsonRecElementBuilder(CfgResHandlerInf::ProductVersion v)
    {
        m_pv = v;
    }

    //CfgJsonRecElementBuilder& loadFile(const QFile&);
    CfgJsonRecElementBuilder& PV(const CfgResHandlerInf::ProductVersion v)
    {
        m_pv = v;
        return *this;
    }

    CfgJsonRecElementBuilder& manufacture(const QString& v)
    {
        m_manufacture = v;
        return *this;
    }

    CfgJsonRecElementBuilder& vanes(const quint8 v)
    {
        m_vanes = v;
        return *this;
    }

    CfgJsonRecElementBuilder& plans(const TestPlanEnum v)
    {
        m_plan = v;
        return *this;
    }

    CfgJsonRecElementBuilder& numOfMotor(const quint8 v)
    {
        m_numOfMotor = v;
        return *this;
    }

    CfgJsonRecElementBuilder& motorType(const QModbus2DataUnit::MotorTypeEnum v)
    {
        m_motorType = v;
        return *this;
    }

    CfgJsonRecElementBuilder& timeStamp(const QString& v)
    {
        m_timeStamp = v;
        return *this;
    }

public:
    CfgJsonRecElement build(){
        return CfgJsonRecElement(*this);
    }

public:
    TestPlanEnum m_plan = TestPlanEnum::Invaild;
    CfgResHandlerInf::ProductVersion m_pv;
    QString m_manufacture = QStringLiteral("shilu");
    quint8  m_vanes = 2;
    quint8  m_numOfMotor = 0;
    QModbus2DataUnit::MotorTypeEnum m_motorType = QModbus2DataUnit::MotorTypeEnum::ELECE;
    QString m_timeStamp;
};
#endif // CFGJSONRECELEMENT_H
