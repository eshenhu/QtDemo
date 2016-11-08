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
                               POWER };

//enum UiMeasType
//{
//    INVALID = 0,
//    VOLTAGE = 1,
//    THROTTLE = 2
//};

union UiMeasData{
    VoltageTstData u;
    ThrottleTstData v;
};

struct UiCompMeasData{
    JsonGUIPrimType type;
    UiMeasData data;
};




#endif // UITESTBASEDSTRUCT_H
