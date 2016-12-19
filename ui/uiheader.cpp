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
    { TestUnitName::VOL(), QColor("#FFA100") },
    { TestUnitName::THROTTLE(), QColor("#1EA100") },
    { TestUnitName::CURRENT(), QColor("#2FA100") },
    { TestUnitName::THRUST(), QColor("#3FA100") },
    { TestUnitName::TORQUE(), QColor("#4FA100") },
    { TestUnitName::SPEED(), QColor("#5FA100") },
    { TestUnitName::TEMP1(), QColor("#6FA100") },
    { TestUnitName::TEMP2(), QColor("#7FA100") },
    { TestUnitName::EFFI_POWER(), QColor("#8FA100") },
    { TestUnitName::EFFI_ELE(), QColor("#9FA100") },
    { TestUnitName::POWER(), QColor("#AFA100") }
};
