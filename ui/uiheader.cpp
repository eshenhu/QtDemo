#include "uiheader.h"
#include <QString>
#include <map>
#include <QColor>

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

std::map<QString, QColor> colorPerTestElement =
{
    { TestUnitName::VOL(), QColor("#0000FF") },
    { TestUnitName::THROTTLE(), QColor("#2E8B57") },
    { TestUnitName::CURRENT(), QColor("#FF0000") },
    { TestUnitName::THRUST(), QColor("#FF00FF") },
    { TestUnitName::TORQUE(), QColor("#A020F0") },
    { TestUnitName::SPEED(), QColor("#20B2AA") },
    { TestUnitName::TEMP1(), QColor("#00FF00") },
    { TestUnitName::TEMP2(), QColor("#7FFFD4") },
    { TestUnitName::EFFI_POWER(), QColor("#FFB5C5") },
    { TestUnitName::EFFI_ELE(), QColor("#FFD700") },
    { TestUnitName::POWER(), QColor("#AFA100") },
    { TestUnitName::MECHAPWR(), QColor("#8B0000") },
    { TestUnitName::MECHAEFFI(), QColor("#B8860B") },
};
