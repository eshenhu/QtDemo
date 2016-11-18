#ifndef UITESTBASEDSTRUCT_H
#define UITESTBASEDSTRUCT_H

struct VoltageTstData
{
   quint16 thro;
   quint16 vol_beg;
   quint16 vol_end;
   quint16 vol_step;
   quint16 duration;
};

struct ThrottleTstData
{
   quint16 vol;
   quint16 thro_beg;
   quint16 thro_end;
   quint16 thro_step;
   quint16 duration;
};

struct DistanceTstData
{
    quint16 vol;
    quint16 thro;
    quint16 dis_beg;
    quint16 dis_end;
    quint16 dis_step;
    quint16 duration;
};


enum DistanceTstDataEnum : quint32 {
    ZEROPOS = 0x7FFFFFFF
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
                               POWER,
                               DISTANCE};

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
};

struct UiCompMeasData{
    JsonGUIPrimType type;
    UiMeasData data;
};




#endif // UITESTBASEDSTRUCT_H
