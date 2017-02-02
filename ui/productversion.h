#ifndef PRODUCTVERSION_H
#define PRODUCTVERSION_H

#include <QFrame>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class ProductVersion;
}

class CfgDeviceCfgModel;

class ProductVersion : public QFrame
{
    Q_OBJECT

public:
    explicit ProductVersion(CfgDeviceCfgModel* cfg, QWidget *parent = 0);
    ~ProductVersion();

public:
    const QString VERSION = QStringLiteral("v0.30");

    QString key() const;

private:
    void setupVersionInformation();
    void setupSignalAndSlot();
    bool loadCfg(const QString& str);
    bool read(const QJsonObject &json);
private:
    Ui::ProductVersion *ui;
    QString m_key;
    CfgDeviceCfgModel* m_cfg;
};

#endif // PRODUCTVERSION_H
