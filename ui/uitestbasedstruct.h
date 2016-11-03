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


#endif // UITESTBASEDSTRUCT_H
