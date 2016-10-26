#ifndef CFGJSONPRIMARYELEMENT_H
#define CFGJSONPRIMARYELEMENT_H

#include <QJsonObject>
#include <QString>
#include "comm/qmodbusdataunit.h"


class JsonPVConfig{
public:
    JsonPVConfig() = default;
    void read(const QJsonObject &json);

    QModbus2DataUnit::MotorTypeEnum motorType() const;
    quint8 numOfMotor() const;

private:

    QModbus2DataUnit::MotorTypeEnum m_motorType;
    quint8 m_numOfMotor;
};

enum JsonGUIPrimType : quint8 { INVALID,
                               VOLTAGE,
                               CURRENT,
                               FORCE,
                               THROTTLE,
                               SPEED,
                               TEMP,
                               POWEREFFEC,
                               VIBRATE,
                               POWER };

class JsonGUIElement{
public:
    JsonGUIElement(){}
    void read(const QJsonObject &json);

    JsonGUIPrimType type() const;
    QString str() const;
    quint8 idx() const;
    bool isSelected() const;
    qint16 lowLimit() const;
    qint16 upLimit() const;
    QString unit() const;

private:
    JsonGUIPrimType lookup(const QString) const;

private:
    JsonGUIPrimType m_type;
    QString   m_str;
    quint8    m_idx;
    bool      m_isSelected;
    qint16    m_lowLimit;
    qint16    m_upLimit;
    QString   m_unit;
};

class JsonGUIElementList
{
public:
    JsonGUIElementList(){}

    const QList<JsonGUIElement> &elem() const;

    void read(const QJsonObject &json);
    //void write(QJsonObject &json) const;
private:
    QList<JsonGUIElement> m_elem;
};

class CfgJsonReader
{
public:
    CfgJsonReader();
    ~CfgJsonReader();
    CfgJsonReader(const CfgJsonReader& rfs) = delete;

    bool load(const QString str);

    JsonPVConfig *config() const;
    JsonGUIElementList *guiList() const;

private:
    bool read(const QJsonObject &json, const QString& str);
private:
    JsonPVConfig*       m_config;
    JsonGUIElementList* m_guiList;
};


#endif // CFGJSONPRIMARYELEMENT_H
