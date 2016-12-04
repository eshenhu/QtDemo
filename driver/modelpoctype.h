#ifndef MODELPOCTYPE_H
#define MODELPOCTYPE_H

#include <QMetaType>

namespace ModelPOC
{
    struct DistanceModelPoc
    {
    };

    struct VoltageModelPoc
    {
        quint32 throttle;
        quint32 vol_start;
        quint32 vol_end;
        quint32 vol_step;
        quint32 duration;
    };

    enum struct Phase
    {
        Phase_SoftStart,
        Phase_PRPDelay,
        Phase_HardDelay,
        Phase_NomalRunning
    };
}

Q_DECLARE_METATYPE(ModelPOC::DistanceModelPoc)
Q_DECLARE_METATYPE(ModelPOC::VoltageModelPoc)
Q_DECLARE_METATYPE(ModelPOC::Phase)

#endif // MODELPOCTYPE_H
