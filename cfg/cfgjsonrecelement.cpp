#include "cfgjsonrecelement.h"
#include <QFile>
#include <QJsonDocument>

bool CfgJsonRecElement::loadCfg(const QString& str)
{
    QFile loadFile(str);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());

    return true;
}

bool CfgJsonRecElement::saveCfg(const QString& str)
{
    QFile saveFile(str);

    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject cfgObject;
    write(cfgObject);
    QJsonDocument saveDoc(cfgObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

void CfgJsonRecElement::read(const QJsonObject &json)
{
    m_pv = static_cast<CfgResHandlerInf::ProductVersion>(json["pv"].toInt());
    m_manufacture = json["manufacture"].toString();
    m_vanes = json["vanes"].toInt();
}

void CfgJsonRecElement::write(QJsonObject &json) const
{
    json["pv"] = static_cast<quint8>(m_pv);
    json["manufacture"] = m_manufacture;
    json["vanes"] = m_vanes;
}

CfgJsonRecElement::CfgJsonRecElement(const CfgJsonRecElement::CfgJsonRecElementBuilder &builder)
{
    m_pv = builder.m_pv;
    m_manufacture = builder.m_manufacture;
    m_vanes = builder.m_vanes;
}

