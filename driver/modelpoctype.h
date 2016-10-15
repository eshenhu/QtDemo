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
        VoltageModelPoc() = explicit;
        quint32 throttle;
        quint32 vol_start;
        quint32 vol_end;
        quint32 vol_step;
        quint32 duration;
    };
}

Q_DECLARE_METATYPE(ModelPOC::DistanceModelPoc)
Q_DECLARE_METATYPE(ModelPOC::VoltageModelPoc)

#endif // MODELPOCTYPE_H
