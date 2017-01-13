
#include "cfgjsonprimaryelement.h"

#include <QJsonArray>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>

QModbus2DataUnit::MotorTypeEnum JsonPVConfig::motorType() const
{
    return m_motorType;
}

quint8 JsonPVConfig::numOfMotor() const
{
    return m_numOfMotor;
}

void JsonPVConfig::read(const QJsonObject &json)
{
    QString str(json["motorType"].toString());
    if (str.contains("elec"))
        m_motorType = QModbus2DataUnit::MotorTypeEnum::ELECE;
    else if (str.contains("oil"))
        m_motorType = QModbus2DataUnit::MotorTypeEnum::OILE;
    else
        qWarning() << "com.cfg.json Undefined MotorType" << str;

    m_numOfMotor = static_cast<quint8>(json["numOfMotor"].toInt());
}

const QList<JsonGUIElement> &JsonGUIElementList::elem() const
{
    return m_elem;
}

void JsonGUIElementList::read(const QJsonArray &eleArray)
{
    m_elem.clear();
    for (int eleIndex = 0; eleIndex < eleArray.size(); ++eleIndex) {
        QJsonObject eleObject = eleArray[eleIndex].toObject();
        JsonGUIElement ele;
        ele.read(eleObject);
        m_elem.append(ele);
    }
}

JsonGUIPrimType JsonGUIElement::type() const
{
    return m_type;
}

QString JsonGUIElement::str() const
{
    return m_str;
}

indexOnMotor JsonGUIElement::idx() const
{
    return m_idx;
}

bool JsonGUIElement::isSelected() const
{
    return m_isSelected;
}

qint16 JsonGUIElement::lowLimit() const
{
    return m_lowLimit;
}

qint16 JsonGUIElement::upLimit() const
{
    return m_upLimit;
}

QString JsonGUIElement::unit() const
{
    return m_unit;
}

const char *JsonGUIElement::format(const JsonGUIPrimType type)
{
    return JsonGUIPrimFormat[(quint8)type];
}

void JsonGUIElement::read(const QJsonObject &json)
{
    m_str = json["text"].toString();
    m_type = lookup(m_str);
    quint8 v = static_cast<quint8>(json["index"].toInt());
    m_idx = indexOnMotor(v & 0x0F, (v & 0xF0) >> 4);
    m_isSelected = json["isSelected"].toBool();
    m_lowLimit = static_cast<quint16>(json["lowLimit"].toInt());
    m_upLimit = static_cast<quint16>(json["upLimit"].toInt());
    m_unit = json["unit"].toString();
}

JsonGUIPrimType JsonGUIElement::lookup(const QString& str)
{
    if (str.contains("ENVHUMIDITY"))
        return JsonGUIPrimType::ENVHUMIDITY;
    else if (str.contains("ENVTEMP"))
        return JsonGUIPrimType::ENVTEMP;
    else if (str.contains("ENVPRESSURE"))
        return JsonGUIPrimType::ENVPRESSURE;
    else if (str.contains("ECHODISTANCE"))
        return JsonGUIPrimType::ECHODISTANCE;
    else if (str.contains("VOLTAGE"))
        return JsonGUIPrimType::VOLTAGE;
    else if (str.contains("CURRENT"))
        return JsonGUIPrimType::CURRENT;
    else if (str.contains("THRUST"))
        return JsonGUIPrimType::THRUST;
    else if (str.contains("THROTTLE"))
        return JsonGUIPrimType::THROTTLE;
    else if (str.contains("TORQUE"))
        return JsonGUIPrimType::TORQUE;
    else if (str.contains("SPEED"))
        return JsonGUIPrimType::SPEED;
    else if (str.contains("TEMP"))
        return JsonGUIPrimType::TEMP;
    else if (str.contains("EFFICIENCY-1"))
        return JsonGUIPrimType::EFFICIENCY_G_W;
    else if (str.contains("EFFICIENCY-2"))
        return JsonGUIPrimType::EFFICIENCY_G_A;
    else if (str.contains("VIBRATE"))
        return JsonGUIPrimType::VIBRATE;
    else if (str.contains("POWER"))
        return JsonGUIPrimType::POWER;
    else if (str.contains("MECHAPWR"))
        return JsonGUIPrimType::MECHAPOWER;
    else if (str.contains("MECHAEFFI"))
        return JsonGUIPrimType::MECHAEFFI;
    else
    {
        qWarning() << "com.cfg.json Undefined JsonGUIPrimType" << str;
        return JsonGUIPrimType::INVALID;
    }
}


CfgGUIJsonReader::CfgGUIJsonReader()
{
    m_config = new JsonPVConfig();
    m_guiList = new JsonGUIElementList();
}

CfgGUIJsonReader::~CfgGUIJsonReader()
{
    if (m_config)
        delete m_config;
    if (m_guiList)
        delete m_guiList;
}

bool CfgGUIJsonReader::read(const QJsonObject &json, const QString& str)
{
    QJsonObject pvJsonListObject = json["ProductVersion"].toObject();
    QJsonObject pvJsonObject = pvJsonListObject[str].toObject();

    m_config->read(pvJsonObject["config"].toObject());
    m_guiList->read(pvJsonObject["display"].toArray());
    return true;
}

bool CfgGUIJsonReader::load(const QString str)
{
    const QRegExp rx("^PV\\d\\d?$");   // it should be followed by "PV1, PV16"
    if (!str.contains(rx))
    {
        qWarning() << "com.cfg.json ProductVersion ->" << str << "can't match the rules";
        return false;
    }
    QFile loadFile(QStringLiteral(":/json/json/product_version.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "com.cfg.json Couldn't open save file.";
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    read(loadDoc.object(), str);

    return true;
}

JsonPVConfig *CfgGUIJsonReader::config() const
{
    return m_config;
}

JsonGUIElementList *CfgGUIJsonReader::guiList() const
{
    return m_guiList;
}
