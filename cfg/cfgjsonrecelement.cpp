#include "cfgjsonrecelement.h"
#include <QFile>
#include <QJsonDocument>
#include <QCryptographicHash>

bool CfgJsonRecElement::loadCfg(const QString& str)
{
    QFile loadFile(str);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    return read(loadDoc.object());
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

bool CfgJsonRecElement::read(const QJsonObject &json)
{
    bool result = false;

    m_plan = static_cast<TestPlanEnum>(json["plan"].toInt());
    m_pv = static_cast<CfgResHandlerInf::ProductVersion>(json["pv"].toInt());
    m_manufacture = json["manufacture"].toString();
    m_vanes = json["vanes"].toInt();
    m_motorType = static_cast<QModbus2DataUnit::MotorTypeEnum>(json["motor_type"].toInt());
    m_numOfMotor = json["numOfMotor"].toInt();
    m_timeStamp = json["timeStamp"].toString();

    QString md5CheckSum(json["checksum"].toString());

    // Write MD5 Code in the end
    QByteArray md5Data;
    md5Data.append(QByteArray::number((int)m_plan));
    md5Data.append(QByteArray::number((int)m_pv));
    md5Data.append(m_manufacture);
    md5Data.append((int)m_vanes);
    md5Data.append(QByteArray::number((int)m_motorType));
    md5Data.append(QByteArray::number((int)m_numOfMotor));
    md5Data.append(m_timeStamp);

    QCryptographicHash MD5HashMethod(QCryptographicHash::Md5);
    MD5HashMethod.addData(md5Data);
    QByteArray MD5Result = MD5HashMethod.result().toHex();
    QString toStr(MD5Result);

    if (md5CheckSum == toStr)
    {
        result = true;
    }
    else
    {
        result = false;
        qWarning("Warning: No matched checksum with the data in this file!");
    }

    return result;
}

void CfgJsonRecElement::write(QJsonObject &json) const
{
    json["plan"] = static_cast<quint8>(m_plan);
    json["pv"] = static_cast<quint8>(m_pv);
    json["manufacture"] = m_manufacture;
    json["vanes"] = m_vanes;
    json["motor_type"] = static_cast<quint8>(m_motorType);
    json["numOfMotor"] = m_numOfMotor;
    json["timeStamp"] = m_timeStamp;

    // Write MD5 Code in the end
    QByteArray md5Data;
    md5Data.append(QByteArray::number((int)m_plan));
    md5Data.append(QByteArray::number((int)m_pv));
    md5Data.append(m_manufacture);
    md5Data.append((int)m_vanes);
    md5Data.append(QByteArray::number((int)m_motorType));
    md5Data.append(QByteArray::number((int)m_numOfMotor));
    md5Data.append(m_timeStamp);

    QCryptographicHash MD5HashMethod(QCryptographicHash::Md5);
    MD5HashMethod.addData(md5Data);
    QByteArray MD5Result = MD5HashMethod.result().toHex();
    QString toStr(MD5Result);

    json["checksum"] = toStr;
}

CfgJsonRecElement::CfgJsonRecElement(const CfgJsonRecElement::CfgJsonRecElementBuilder &builder)
{
    m_plan = builder.m_plan;
    m_pv = builder.m_pv;
    m_manufacture = builder.m_manufacture;
    m_vanes = builder.m_vanes;
    m_numOfMotor = builder.m_numOfMotor;
    m_motorType = builder.m_motorType;
    m_timeStamp = builder.m_timeStamp;
}

CfgResHandlerInf::ProductVersion CfgJsonRecElement::pv() const
{
    return m_pv;
}

TestPlanEnum CfgJsonRecElement::plan() const
{
    return m_plan;
}

quint8 CfgJsonRecElement::numOfMotor() const
{
    return m_numOfMotor;
}

QModbus2DataUnit::MotorTypeEnum CfgJsonRecElement::motorType() const
{
    return m_motorType;
}
