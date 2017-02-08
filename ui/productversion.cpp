#include "productversion.h"
#include <QFileDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QMessageBox>
#include "ui_productversion.h"
#include "cfg/cfgreshandler.h"
#include "util/aes.h"
#include <QCryptographicHash>

static const unsigned char const_key[] = { 0xb0, 0x7d, 0x7f, 0xc8,
                              0xcf, 0x37, 0x08, 0xc7,
                              0x08, 0x09, 0x0A, 0x0B,
                              0x0C, 0x0D, 0x0E, 0x0F};

ProductVersion::ProductVersion(CfgDeviceCfgModel* cfg, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ProductVersion),
    m_key(16, ' '),
    m_cfg(cfg)
{
    ui->setupUi(this);
    m_cfg->setKey(m_key);
    setupVersionInformation();
    setupSignalAndSlot();
}

ProductVersion::~ProductVersion()
{
    delete ui;
}

void ProductVersion::setupVersionInformation()
{
    ui->version_LE->setText(VERSION);
    QString path(m_cfg->path());

    if (!(path.isNull() || path.isEmpty()))
    {
        if (!loadCfg(path))
        {
            qWarning() << "Failed to open key file!";
        }
        else
        {
            if (m_key.size() != 32)
            {
                qWarning() << "Invalid License number!";
            }
            else
            {
                QByteArray decryp;
                if (decrypto(m_key, decryp))
                {
                    ui->import_LE->setText(m_key.toHex());
                    m_cfg->setKey(decryp);
                }
                else
                {
                    qWarning() << "ID was corrupt!";
                    ui->import_LE->setText(QStringLiteral("000000000000"));
                }
            }
        }
    }
}

void ProductVersion::setupSignalAndSlot()
{
    connect(ui->import_btn, &QPushButton::clicked, [=](bool checked){
        Q_UNUSED(checked)

        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open license key file"), "./", tr("Key Files (*.json)"));

        bool isError = true;
        QString errorMsg;
        //        QString key = QStringLiteral("Invalid");
        if (!fileName.isNull())
        {
            if (!loadCfg(fileName))
            {
                errorMsg = QStringLiteral("Failed to open key file!");
                qWarning() << errorMsg;
            }
            else
            {
                if (m_key.size() != 32)
                {
                    errorMsg = QStringLiteral("Invalid License number!");
                    qWarning() << errorMsg;
                }
                else
                {
                    QByteArray decryp;
                    if (decrypto(m_key, decryp))
                    {
                        isError = false;
                        ui->import_LE->setText(m_key.toHex());
                        m_cfg->setKey(decryp);
                        m_cfg->setPath(fileName);
                    }
                    else
                    {
                        errorMsg = QStringLiteral("Corrupt data import!");
                        qWarning() << errorMsg;
                    }
                }
            }
        }
        else
        {
            errorMsg = QStringLiteral("Filename is Null");
            qWarning() << errorMsg;
        }

        if (isError)
        {
            int ret = QMessageBox::warning(this, tr("Import License File"), errorMsg,
                                             QMessageBox::Ok);
        }
    });
}

bool ProductVersion::loadCfg(const QString& str)
{
    QFile loadFile(str);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open key file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    return read(loadDoc.object());
}

bool ProductVersion::read(const QJsonObject &json)
{
    bool result = true;
    m_key = QByteArray::fromHex(json["key"].toString().toLatin1());
    return result;
}

bool ProductVersion::decrypto(const QByteArray& encrypto, QByteArray& plain)
{
    QByteArray decrypto(32, '0');
    crypto_aes_ctx ctx;
    crypto_aes_expand_key(&ctx, (const unsigned char*)(&const_key[0]), AES_KEYSIZE_128);
    aes_decrypt(&ctx, (unsigned char*)decrypto.data(), (const unsigned char*)encrypto.constData());

    plain = decrypto.left(16);
    QByteArray MD5Check = decrypto.right(16);
    QByteArray MD5Result = QCryptographicHash::hash(plain, QCryptographicHash::Md5).toHex();

    return MD5Check == MD5Result;
}

QByteArray ProductVersion::key() const
{
    return m_key;
}
