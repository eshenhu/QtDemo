#ifndef TESTINFOCONFIG_H
#define TESTINFOCONFIG_H

#include <QFrame>

QT_BEGIN_NAMESPACE

namespace Ui {
class DeviceInfoConfig;
}
QT_END_NAMESPACE

class DeviceInfoConfig : public QFrame
{
    Q_OBJECT

public:
    explicit DeviceInfoConfig(QWidget *parent = 0);
    ~DeviceInfoConfig();

private:
    Ui::DeviceInfoConfig *ui;
};

#endif // TESTINFOCONFIG_H
