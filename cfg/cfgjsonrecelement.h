#ifndef CFGJSONRECELEMENT_H
#define CFGJSONRECELEMENT_H

#include "cfg/cfgreshandlerinf.h"
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

public:
    CfgJsonRecElement build(){
        return CfgJsonRecElement(*this);
    }

public:
    CfgResHandlerInf::ProductVersion m_pv;
    QString m_manufacture = QStringLiteral("tongyi");
    quint8  m_vanes = 2;
};
#endif // CFGJSONRECELEMENT_H
