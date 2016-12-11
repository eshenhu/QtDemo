#include "uiheader.h"
#include <QString>
#include <map>

std::map<int, QString> TestPlanStringMap =
{
    { (int)TestPlanEnum::Invaild,    QStringLiteral("Invalid Test") },
    { (int)TestPlanEnum::Voltage,    QStringLiteral("Voltage Test") },
    { (int)TestPlanEnum::Throttle,   QStringLiteral("Throttle Test")},
    { (int)TestPlanEnum::Distance,   QStringLiteral("Distance Test") },
    { (int)TestPlanEnum::Multiplue,  QStringLiteral("Multiplue Test")},
    { (int)TestPlanEnum::Aging,      QStringLiteral("Aging Test") },
    { (int)TestPlanEnum::Calibrate,  QStringLiteral("Calibrate Test") },
    { (int)TestPlanEnum::Manual,     QStringLiteral("Manual Test") }
};
