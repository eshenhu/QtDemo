#ifndef UITESTBASEDSTRUCT_H
#define UITESTBASEDSTRUCT_H

#include <QString>
#include <map>
#include <QColor>

enum TestPlanEnum {
    Invaild,
    Voltage,
    Throttle,
    Distance,
    Multiplue,
    Aging,
    Calibrate,
    Manual
};

extern std::map<int, QString> TestPlanStringMap;
extern std::map<QString, QColor> colorPerTestElement;

class TestUnitName{
public:
    TestUnitName() = delete;
public:
    static QString VOL()       { return QStringLiteral("VOLTAGE"); }
    static QString THROTTLE()  { return QStringLiteral("THROTTLE"); }
    static QString CURRENT()  { return QStringLiteral("CURRENT"); }
    static QString THRUST()   { return QStringLiteral("THRUST"); }
    static QString TORQUE()  { return QStringLiteral("TORQUE"); }
    static QString SPEED()  { return QStringLiteral("SPEED"); }
    static QString TEMP1()  { return QStringLiteral("TEMP1"); }
    static QString TEMP2()  { return QStringLiteral("TEMP2"); }
    static QString EFFI_POWER()  { return QStringLiteral("EFFICIENCY-1"); }
    static QString EFFI_ELE()  { return QStringLiteral("EFFICIENCY-2"); }
    static QString POWER()  { return QStringLiteral("POWER"); }
    static QString MECHAPWR()  { return QStringLiteral("MECHAPWR"); }
    static QString MECHAEFFI()  { return QStringLiteral("MECHAEFFI"); }
};

struct VoltageTstData
{
   quint16 thro;
   double vol_beg;
   double vol_end;
   double vol_step;
   quint16 duration;
};

struct ThrottleTstData
{
   double vol;
   quint16 thro_beg;
   quint16 thro_end;
   quint16 thro_step;
   quint16 duration;
};

struct DistanceTstData
{
    double vol;
    quint16 thro;
    quint16 dis_beg;
    quint16 dis_end;
    quint16 dis_step;
    quint16 duration;
};

struct MultipuleTstData
{
   quint16 vol;
   quint16 thro_beg;
   quint16 thro_end;
   quint16 thro_step;
   quint16 duration;
};

struct AgingTstData
{
   quint16 thro;
   double vol;
   quint16 duration;
};

enum DistanceTstDataEnum : quint32 {
    ZEROPOS     = 0,
    GOTOZEROPOS  = 0xFFFFFFEE
};

enum JsonGUIPrimType : quint8 { INVALID,
                               VOLTAGE,
                               CURRENT,
                               THRUST,
                               THROTTLE,
                               TORQUE,
                               SPEED,
                               TEMP,
                               EFFICIENCY_G_W,
                               EFFICIENCY_G_A,
                               VIBRATE,
                               POWER,
                               MECHAPOWER,
                               MECHAEFFI};


static const char* JsonGUIPrimFormat[] = {
    "%10.1f",   //INVALID
    "%10.1f",   //VOLTAGE
    "%10.2f",   //CURRENT
    "%10.1f",   //THRUST
    "%10.0f",     //THROTTLE
    "%10.3f",     //TORQUE
    "%10.0f",     //SPEED
    "%10.1f",     //TEMP
    "%10.1f",     //EFFICIENCY_G_W
    "%10.1f",     //EFFICIENCY_G_A
    "%10.0f",     //VIBRATE
    "%10.1f",     //POWER
    "%10.1f",     //MECHAPOWER
    "%10.1f"     //MECHAEFFI
};

//enum UiMeasType
//{
//    INVALID = 0,
//    VOLTAGE = 1,
//    THROTTLE = 2
//};

union UiMeasData{
    VoltageTstData u;
    ThrottleTstData v;
    DistanceTstData w;
    MultipuleTstData x;
};

struct UiCompMeasData{
    TestPlanEnum type;
    UiMeasData data;
};

#endif // UITESTBASEDSTRUCT_H
