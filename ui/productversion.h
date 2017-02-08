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
    const QString VERSION = QStringLiteral("v4.7");

    QByteArray key() const;

private:
    void setupVersionInformation();
    void setupSignalAndSlot();
    bool loadCfg(const QString& str);
    bool read(const QJsonObject &json);

/*
 * key : 0000 1111 1111 1111
 * MD5:  MD5(key)
 * AES128 (key + MD5)
*/
    bool decrypto(const QByteArray&, QByteArray&);

private:
    Ui::ProductVersion *ui;
    QByteArray m_key;
    CfgDeviceCfgModel* m_cfg;

    //0xb07d7fc8cf3708c7 08090a0b0c0d0e0f
};

#endif // PRODUCTVERSION_H
