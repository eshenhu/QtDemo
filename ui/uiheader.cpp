#include "uiheader.h"
#include <QString>
#include <map>
#include <QColor>

std::map<int, QString> TestPlanStringMap =
{
    { (int)TestPlanEnum::Invaild,    QStringLiteral("Invalid") },
    { (int)TestPlanEnum::Voltage,    QStringLiteral("Voltage") },
    { (int)TestPlanEnum::Throttle,   QStringLiteral("Throttle")},
    { (int)TestPlanEnum::Distance,   QStringLiteral("Distance") },
    { (int)TestPlanEnum::Multiplue,  QStringLiteral("Multipule")},
    { (int)TestPlanEnum::Aging,      QStringLiteral("Aging") },
    { (int)TestPlanEnum::Calibrate,  QStringLiteral("Calibrate") },
    { (int)TestPlanEnum::Manual,     QStringLiteral("Manual") }
};

std::map<QString, QColor> TestUnitColor::colorPerTestElement =
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

QVector<QColor> TestUnitColor::colorPerGraph = {QColor("#0000FF"),
                                                QColor("#2E8B57"),
                                                QColor("#FF0000"),
                                                QColor("#FF00FF"),
                                                QColor("#A020F0"),
                                                QColor("#20B2AA"),
                                                QColor("#00FF00"),
                                                QColor("#7FFFD4")};


QColor TestUnitColor::getColor(const QString testUnitName, const quint32 idxOfGraph, const bool isCmpEnabled)
{
    QColor color;

    if (isCmpEnabled)
    {
        color = colorPerGraph.value(idxOfGraph, QColor("#B8860B"));
    }
    else
    {
        color = colorPerTestElement[testUnitName];
    }
    return color;
}
