#include "productversion.h"
#include <QFileDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QMessageBox>
#include "ui_productversion.h"
#include "cfg/cfgreshandler.h"

ProductVersion::ProductVersion(CfgDeviceCfgModel* cfg, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ProductVersion),
    m_key("0"),
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
            ui->import_LE->setText(m_key);
            m_cfg->setKey(m_key);
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
                const QRegExp rx("^[0-9a-fA-F]{32,32}$");
                if (!m_key.contains(rx))
                {
                    errorMsg = QStringLiteral("Invalid License number!");
                    qWarning() << errorMsg;
                }
                else
                {
                    isError = false;
                    ui->import_LE->setText(m_key);
                    m_cfg->setKey(m_key);
                    m_cfg->setPath(fileName);
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
    m_key = json["key"].toString();
    return result;
}

QString ProductVersion::key() const
{
    return m_key;
}
