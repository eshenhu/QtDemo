#ifndef CFGWASHINGDATAINF_H
#define CFGWASHINGDATAINF_H

#include <QVector>
#include "cfg/datajsonrecelement.h"
#include "chartsview/qcustomplot.h"

class CfgMetaElement{
public:
    CfgMetaElement() = default;
    CfgMetaElement(QString lfh_name, quint32 lfh_motorIdx);
    ~CfgMetaElement(){}

public:
    QString getName() const;

    quint8 getMotorIdx() const;

    double getData() const;
    void setData(double value);

private:
    QString name;
    quint8 motorIdx;
    double data;
};

enum class CfgWashingTypeEnum {
    CFGWASHINGTIME_E1,
    CFGWASHINGTIME_E2,
    CFGWASHINGVOL_E1,
    CFGWASHINGVOL_E2,
    CFGWASHINGTHROTTLE_E1,
    CFGWASHINGTHROTTLE_E2,
    CFGWASHINGMULTI_E1,
    CFGWASHINGMULTI_E2,
    CFGWASHINGDISTANCE
};

class CfgWashingDataInf
{
public:
    explicit CfgWashingDataInf(const CfgWashingTypeEnum type);
    virtual ~CfgWashingDataInf() {}
    virtual void wash(const QVector<DataJsonRecElement>&) = 0;
    virtual void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) = 0;
    virtual QVector<CfgMetaElement>& getGUIActionList(quint32) = 0;
    CfgWashingTypeEnum type() const;

private:
    CfgWashingTypeEnum m_type;
};

class KV
{
public:
    KV(){}
    KV(double x, double y): key(x), value(y){}

public:
    double key;
    double value;
};

//Q_DECLARE_INTERFACE(cfgWashingDataInf, "cfgWashingDataInf/1.0")


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

class CfgItemMeasBasedE1WashingOutDataEle
{
public:
    enum class ELEMEASE1CURSOR : quint8
    {
        REC_VOL_POS = 0,
        REC_THRO_POS,
//        REC_DISTANCE_POS,

        REC_CUR1_POS,
        REC_THU1_POS,
        REC_TORQUE1_POS,
        REC_SPEED1_POS,
        REC_MOTOR1TMP1_POS,
        REC_MOTOR1TMP2_POS,
        REC_M1EFFICI1_POS,
        REC_M1EFFICI2_POS,
        REC_M1POWER,
        REC_M1MECHAPWR,
        REC_M1MECHAEFFI,

        ELEMCURSOR_END
    };

public:
    explicit CfgItemMeasBasedE1WashingOutDataEle();
    ~CfgItemMeasBasedE1WashingOutDataEle(){}

    void setData(quint32 idx, double value);
    double getData(quint32 idx) const;

    QString getName(quint32 idx) const;
    quint8 getMotorIdx(quint32 idx) const;

public:
    QVector<CfgMetaElement> m_metaEle;
};

class CfgItemMeasBasedE2WashingOutDataEle
{
public:
    enum class ELEMEASCURSOR : quint8
    {
        REC_VOL_POS = 0,
        REC_THRO_POS,
        REC_DISTANCE_POS,

        REC_CUR1_POS,
        REC_THU1_POS,
        REC_TORQUE1_POS,
        REC_SPEED1_POS,
        REC_MOTOR1TMP1_POS,
        REC_MOTOR1TMP2_POS,
        REC_M1EFFICI1_POS,
        REC_M1EFFICI2_POS,
        REC_M1POWER,
        REC_M1MECHAPWR,
        REC_M1MECHAEFFI,

        REC_CUR2_POS,
        REC_THU2_POS,
        REC_TORQUE2_POS,
        REC_SPEED2_POS,
        REC_MOTOR2TMP1_POS,
        REC_MOTOR2TMP2_POS,
        REC_M2EFFICI1_POS,
        REC_M2EFFICI2_POS,
        REC_M2POWER,
        REC_M2MECHAPWR,
        REC_M2MECHAEFFI,

        ELEMCURSOR_END
    };

public:
    explicit CfgItemMeasBasedE2WashingOutDataEle();
    ~CfgItemMeasBasedE2WashingOutDataEle(){}

    void setData(quint32 idx, double value);
    double getData(quint32 idx) const;

    QString getName(quint32 idx) const;
    quint8 getMotorIdx(quint32 idx) const;

public:
    QVector<CfgMetaElement> m_metaEle;
};

//---------------------------------------------
/*
 * The result shoudle be startwith the Throttle on the X axis. Others on the Y asix.
*/

class CfgManualWashingDataE1Clz : public CfgWashingDataInf
{
public:
    CfgManualWashingDataE1Clz();
    ~CfgManualWashingDataE1Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
    QVector<CfgItemMeasBasedE1WashingOutDataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE1WashingOutDataEle deserialize(const DataJsonRecElement& in);
private:
    QVector<CfgItemMeasBasedE1WashingOutDataEle> m_data;
    QVector<CfgMetaElement> m_guiList;
};

//---------------------------------------------
/*
 * The result shoudle be startwith the Throttle on the X axis. Others on the Y asix.
*/

class CfgManualWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgManualWashingDataE2Clz();
    ~CfgManualWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
    QVector<CfgItemMeasBasedE2WashingOutDataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE2WashingOutDataEle deserialize(const DataJsonRecElement& in);
private:
    QVector<CfgItemMeasBasedE2WashingOutDataEle> m_data;
    QVector<CfgMetaElement> m_guiList;
};

//---------------------------------------------
/*
 * The result shoudle be startwith the Throttle on the X axis. Others on the Y asix.
*/

class CfgThrottleWashingDataE1Clz : public CfgWashingDataInf
{
public:
    CfgThrottleWashingDataE1Clz();
    ~CfgThrottleWashingDataE1Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
    QVector<CfgItemMeasBasedE1WashingOutDataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE1WashingOutDataEle deserialize(const DataJsonRecElement& in);
    void accumulate(const CfgItemMeasBasedE1WashingOutDataEle& data, CfgItemMeasBasedE1WashingOutDataEle&);
private:
    QVector<CfgItemMeasBasedE1WashingOutDataEle> m_data;
    QVector<CfgMetaElement> m_guiList;
};

//---------------------------------------------
/*
 * The result shoudle be startwith the Throttle on the X axis. Others on the Y asix.
*/

class CfgThrottleWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgThrottleWashingDataE2Clz();
    ~CfgThrottleWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
    QVector<CfgItemMeasBasedE2WashingOutDataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE2WashingOutDataEle deserialize(const DataJsonRecElement& in);
    void accumulate(const CfgItemMeasBasedE2WashingOutDataEle& data, CfgItemMeasBasedE2WashingOutDataEle&);
private:
    QVector<CfgItemMeasBasedE2WashingOutDataEle> m_data;
    QVector<CfgMetaElement> m_guiList;
};

//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

class CfgVolWashingDataE1Clz : public CfgWashingDataInf
{
public:
    CfgVolWashingDataE1Clz();
    ~CfgVolWashingDataE1Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
    QVector<CfgItemMeasBasedE1WashingOutDataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE1WashingOutDataEle deserialize(const DataJsonRecElement& in);
    void accumulate(const CfgItemMeasBasedE1WashingOutDataEle& data, CfgItemMeasBasedE1WashingOutDataEle&);
private:
    QVector<CfgItemMeasBasedE1WashingOutDataEle> m_data;
    QVector<CfgMetaElement> m_guiList;
};

//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

class CfgVolWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgVolWashingDataE2Clz();
    ~CfgVolWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
    QVector<CfgItemMeasBasedE2WashingOutDataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE2WashingOutDataEle deserialize(const DataJsonRecElement& in);
    void accumulate(const CfgItemMeasBasedE2WashingOutDataEle& data, CfgItemMeasBasedE2WashingOutDataEle&);
private:
    QVector<CfgItemMeasBasedE2WashingOutDataEle> m_data;
    QVector<CfgMetaElement> m_guiList;
};

//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

class CfgDistanceWashingDataE2Clz : public CfgWashingDataInf
{
public:
    CfgDistanceWashingDataE2Clz();
    ~CfgDistanceWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
    QVector<CfgItemMeasBasedE2WashingOutDataEle>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32) override { return m_guiList; }
private:
    CfgItemMeasBasedE2WashingOutDataEle deserialize(const DataJsonRecElement& in);
    void accumulate(const CfgItemMeasBasedE2WashingOutDataEle& data, CfgItemMeasBasedE2WashingOutDataEle&);
private:
    QVector<CfgItemMeasBasedE2WashingOutDataEle> m_data;
    QVector<CfgMetaElement> m_guiList;
};


//---------------------------------------------
/*
 * The result shoudle be startwith the Vol on the X axis. Others on the Y asix.
*/

struct AsixDataItem
{
    QString str;
    quint32 idx;
};

struct CfgMultiWashingDataItem
{
    quint32 motorIdx;
    AsixDataItem XKey;
    AsixDataItem YValue;
    QVector<KV> data;
};

class CfgMultiWashingDataE1Clz : public CfgWashingDataInf
{
public:
    static const quint32 MAX_POINT_XASIX = 100;
public:
    CfgMultiWashingDataE1Clz();
    ~CfgMultiWashingDataE1Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
//    QVector<CfgMultiWashingDataItem>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32 idx) override
    {
        if (idx < m_guiList.size())
            return m_guiList[idx];
        else
            return m_guiList[0];
    }
private:
    //CfgItemMeasBasedE2DataEle deserialize(const DataJsonRecElementE2& in);
    //void accumulate(const CfgItemMeasBasedE2DataEle& data, CfgItemMeasBasedE2DataEle&);
    double weightValues(QList<double>& values);
private:
    QVector<QVector<CfgMultiWashingDataItem>> m_data;
    QVector<QVector<CfgMetaElement>> m_guiList;
};

class CfgMultiWashingDataE2Clz : public CfgWashingDataInf
{
public:
    static const quint32 MAX_POINT_XASIX = 100;
public:
    CfgMultiWashingDataE2Clz();
    ~CfgMultiWashingDataE2Clz() {}
    // cfgWashingDataInf interface
public:
    void wash(const QVector<DataJsonRecElement>&) override;
//    QVector<CfgMultiWashingDataItem>& data();
    void generateData(quint32 idx, QVector<QCPGraphData>& pairs, QString& name, quint8& motorIdx) override;
    QVector<CfgMetaElement> &getGUIActionList(quint32 idx) override
    {
        if (idx < m_guiList.size())
            return m_guiList[idx];
        else
            return m_guiList[0];
    }
private:
    //CfgItemMeasBasedE2DataEle deserialize(const DataJsonRecElementE2& in);
    //void accumulate(const CfgItemMeasBasedE2DataEle& data, CfgItemMeasBasedE2DataEle&);
    double weightValues(QList<double>& values);
private:
    QVector<QVector<CfgMultiWashingDataItem>> m_data;
    QVector<QVector<CfgMetaElement>> m_guiList;
};

#endif // CFGWASHINGDATAINF_H
