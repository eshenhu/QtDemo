#ifndef UITESTBASEDSTRUCT_H
#define UITESTBASEDSTRUCT_H


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
                               EFFICIENCY,
                               VIBRATE,
                               POWER};

enum TestCasePrimType : quint8 { TCINVALID,
                                 TCVOLTAGE,
                                 TCTHROTTLE,
                                 TCDISTANCE,
                                 TCMULTIPULE,
                                 TCAGING,
                                 TCMANUAL};

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
    TestCasePrimType type;
    UiMeasData data;
};




#endif // UITESTBASEDSTRUCT_H
