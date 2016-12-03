#ifndef CFGJSONRECELEMENT_H
#define CFGJSONRECELEMENT_H

#include "cfg/cfgreshandlerinf.h"
#include "ui/uiheader.h"
#include "comm/qmodbusdataunit.h"
#include <QString>
class QFile;

class CfgJsonRecElement
{
private:
    CfgJsonRecElement() = default;
    //CfgJsonRecElement(const QFile&);
public:
    bool loadCfg(const QString&);
    bool saveCfg(const QString&);

private:
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

public:
    class CfgJsonRecElementBuilder;

private:
    CfgJsonRecElement(const CfgJsonRecElement::CfgJsonRecElementBuilder& builder);

private:
    CfgResHandlerInf::ProductVersion m_pv;
    QString m_manufacture = QStringLiteral("tongyi");
    quint8  m_vanes = 2;
    TestPlanEnum m_plan = TestPlanEnum::Invaild;
    QModbus2DataUnit::MotorTypeEnum m_motorType;
};

class CfgJsonRecElement::CfgJsonRecElementBuilder
{
public:
    CfgJsonRecElementBuilder(){
        m_pv = CfgResHandlerInf::ProductVersion::PV11;
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

    CfgJsonRecElementBuilder& motorType(const QModbus2DataUnit::MotorTypeEnum v)
    {
        m_motorType = v;
        return *this;
    }

public:
    CfgJsonRecElement build(){
        return CfgJsonRecElement(*this);
    }

public:
    TestPlanEnum m_plan = TestPlanEnum::Invaild;
    CfgResHandlerInf::ProductVersion m_pv;
    QString m_manufacture = QStringLiteral("tongyi");
    quint8  m_vanes = 2;
    QModbus2DataUnit::MotorTypeEnum m_motorType = QModbus2DataUnit::MotorTypeEnum::ELECE;
};
#endif // CFGJSONRECELEMENT_H
