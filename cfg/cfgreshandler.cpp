#include "cfgreshandler.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDebug>

#include "ui/testinfoconfig.h"

CfgResHandler::CfgResHandler(QObject *parent) : QObject(parent),
  m_setting(QCoreApplication::applicationDirPath() + QStringLiteral("/config.ini"), QSettings::IniFormat)
{
    qDebug() << "[CONFIG] file save to " << m_setting.fileName();
    m_bootCfg = new CfgMotorBootCfgModel(m_setting);
    m_deviceCfg = new CfgDeviceCfgModel(m_setting);
    m_prodCfg = new CfgProductVersionCfgModel(m_setting);
    m_calibrateCfg = new CfgCalibrateCfgModel(m_setting);
}

CfgResHandler::~CfgResHandler()
{
    if(m_bootCfg)
        delete m_bootCfg;
    if(m_deviceCfg)
        delete m_deviceCfg;
    if(m_prodCfg)
        delete m_prodCfg;
    if(m_calibrateCfg)
        delete m_calibrateCfg;
}

CfgMotorBootCfgModel *CfgResHandler::bootCfg() const
{
    return m_bootCfg;
}

CfgDeviceCfgModel *CfgResHandler::deviceCfg() const
{
    return m_deviceCfg;
}

CfgProductVersionCfgModel *CfgResHandler::prodCfg() const
{
    return m_prodCfg;
}

CfgCalibrateCfgModel *CfgResHandler::calibrateCfg() const
{
    return m_calibrateCfg;
}

CfgMotorBootCfgModel::CfgMotorBootCfgModel(QSettings& set):
    m_boot_delay(5),
    m_boot_rape(10),
    m_bootVol(5),
    m_set(set)
{
    loadSetting();
}

void CfgMotorBootCfgModel::loadSetting()
{
    m_set.beginGroup("cfg/motor");
    m_boot_delay = m_set.value("boot_delay", 2).toInt();
    m_boot_rape  = m_set.value("boot_rape", 2).toInt();
    m_PRP = m_set.value("PRP", 2).toInt();
    m_bootVol = m_set.value("boot_vol", 5).toInt();
    m_set.endGroup();
}

quint32 CfgMotorBootCfgModel::bootVol() const
{
    return m_bootVol;
}

void CfgMotorBootCfgModel::setBootVol(const quint32 &bootVol)
{
    m_bootVol = bootVol;
    m_set.setValue("cfg/motor/boot_vol", m_bootVol);
}

quint32 CfgMotorBootCfgModel::PRP() const
{
    return m_PRP;
}

void CfgMotorBootCfgModel::set_PRP(const quint32 &PRP)
{
    m_PRP = PRP;
    m_set.setValue("cfg/motor/PRP", m_PRP);
}

void CfgMotorBootCfgModel::set_boot_delay(const quint32 &boot_delay)
{
    m_boot_delay = boot_delay;
    m_set.setValue("cfg/motor/boot_delay", m_boot_delay);
}

void CfgMotorBootCfgModel::set_boot_rape(const quint32 &boot_rape)
{
    m_boot_rape = boot_rape;
    m_set.setValue("cfg/motor/boot_rape", m_boot_rape);
}

quint32 CfgMotorBootCfgModel::boot_delay() const
{
    return m_boot_delay;
}

quint32 CfgMotorBootCfgModel::boot_rape() const
{
    return m_boot_rape;
}

CfgDeviceCfgModel::CfgDeviceCfgModel(QSettings &set):
    m_vane(1),
    m_HZ(50),
    m_set(set)
{
    loadSetting();
}

quint32 CfgDeviceCfgModel::vane() const
{
    return m_vane;
}

void CfgDeviceCfgModel::setVane(const quint32 &vane)
{
    m_vane = vane;
    m_set.setValue("cfg/device/vanes", vane);
}

quint32 CfgDeviceCfgModel::HZ() const
{
    return m_HZ;
}

void CfgDeviceCfgModel::setHZ(const quint32 &HZ)
{
    m_HZ = HZ;
    m_set.setValue("cfg/device/HZ", HZ);
}

void CfgDeviceCfgModel::loadSetting()
{
    m_set.beginGroup("cfg/device");
    m_path = m_set.value("licensePath", "").toString();
    m_vane = m_set.value("vanes", 1).toInt();
    m_HZ  = m_set.value("HZ", DeviceInfoConfig::Freq::B50HZ).toInt();
    m_lowThroLimit = m_set.value("ThroLowLimit", 0).toInt();
    m_highThroLimit = m_set.value("ThroHighLimit", 90).toInt();
    m_SerialNumber = m_set.value("SN").toString();
    m_set.endGroup();
}

QString CfgDeviceCfgModel::path() const
{
    return m_path;
}

void CfgDeviceCfgModel::setPath(const QString &path)
{
    m_path = path;
    m_set.setValue("cfg/device/licensePath", path);
}


/*
 *  creep changes 12bit + head (4bit) = 16 bit
 */
void CfgDeviceCfgModel::setKey(const QByteArray &key)
{
    m_key = key.mid(4,4) + key.left(12);
}

QByteArray CfgDeviceCfgModel::key() const
{
    return m_key;
}

QString CfgDeviceCfgModel::SerialNumber() const
{
    return m_SerialNumber;
}

void CfgDeviceCfgModel::setSerialNumber(const QString &SerialNumber)
{
    m_SerialNumber = SerialNumber;
    m_set.setValue("cfg/device/SN", SerialNumber);
}

quint32 CfgDeviceCfgModel::highThroLimit() const
{
    return m_highThroLimit;
}

void CfgDeviceCfgModel::setHighThroLimit(const quint32 &highThroLimit)
{
    m_highThroLimit = highThroLimit;
    m_set.setValue("cfg/device/ThroHighLimit", m_highThroLimit);
}

quint32 CfgDeviceCfgModel::lowThroLimit() const
{
    return m_lowThroLimit;
}

void CfgDeviceCfgModel::setLowThroLimit(const quint32 &lowThroLimit)
{
    m_lowThroLimit = lowThroLimit;
    m_set.setValue("cfg/device/ThroLowLimit", lowThroLimit);
}

CfgProductVersionCfgModel::CfgProductVersionCfgModel(QSettings &set):
    m_set(set)
{
    loadSetting();
}

CfgResHandlerInf::MotorType CfgProductVersionCfgModel::motor_type() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].m;
}

quint32 CfgProductVersionCfgModel::num_of_motor() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].numberOfMotor;
}

quint32 CfgProductVersionCfgModel::max_power() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxPower;
}

quint32 CfgProductVersionCfgModel::max_vol() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxVol;
}

quint32 CfgProductVersionCfgModel::max_cur() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxCur;
}

quint32 CfgProductVersionCfgModel::max_torque() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxTorque;
}

quint32 CfgProductVersionCfgModel::max_thrust() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxThrust;
}

quint32 CfgProductVersionCfgModel::max_distance() const
{
    return tabelCfgMotorProdVer[(quint8)m_prod].maxDistance;
}

CfgResHandlerInf::ProductVersion CfgProductVersionCfgModel::prod_version() const
{
    return m_prod;
}

void CfgProductVersionCfgModel::loadSetting()
{
    m_set.beginGroup("cfg/product");

    // eshenhu : need to be more consideration
    //m_prod = static_cast<CfgResHandlerInf::ProductVersion>(m_set.value("version", (quint8)CfgResHandlerInf::ProductVersion::PV11).toInt());
    m_prod = CfgResHandlerInf::ProductVersion::PV4;
    if ((quint8)m_prod >= (quint8)CfgResHandlerInf::ProductVersion::MAX)
        m_prod = CfgResHandlerInf::ProductVersion::INVALID;

    m_set.endGroup();
}

CfgCalibrateCfgModel::CfgCalibrateCfgModel(QSettings &set):
    m_set(set)
{
    for (quint32 i = 0; i < MAX_SUPPORT_MOTOR; ++i) {

         struct CfgCalibrateCfgPerMotor& cfgCali = CfgCalibrateCfgPerMotor[i];
         cfgCali.divisionOnThrust = defaultDivisionOnThrust[i];
         cfgCali.divisionOnTorque = defaultDivisionOnTorque[i];
    }

    loadSetting();

    qDebug() << QString("QSetting: calibrate.divsionOnThrust %1 - %2")
                .arg(CfgCalibrateCfgPerMotor[0].divisionOnThrust)
                .arg(CfgCalibrateCfgPerMotor[1].divisionOnThrust);

    qDebug() << QString("QSetting: calibrate.divsionOnTorque %1 - %2")
                .arg(CfgCalibrateCfgPerMotor[0].divisionOnTorque)
                .arg(CfgCalibrateCfgPerMotor[1].divisionOnTorque);
}

double CfgCalibrateCfgModel::getDivisionThrustCaliOnMotor(const quint32 idxMotor){
    if (idxMotor < MAX_SUPPORT_MOTOR){
        return CfgCalibrateCfgPerMotor[idxMotor].divisionOnThrust;
    }
    else
    {
        qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
        return 1.0;
    }
}

bool CfgCalibrateCfgModel::setDivisionThrustCaliOnMotor(const quint32 idxMotor, const double value){
    bool result = false;
    if (idxMotor < MAX_SUPPORT_MOTOR){
        CfgCalibrateCfgPerMotor[idxMotor].divisionOnThrust = value;
        result = true;
    }
    else
    {
        qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
    }
    return result;
}

double CfgCalibrateCfgModel::getDivisionTorqueCaliOnMotor(const quint32 idxMotor){
    if (idxMotor < MAX_SUPPORT_MOTOR){
        return CfgCalibrateCfgPerMotor[idxMotor].divisionOnTorque;
    }
    else
    {
        qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
        return 1;
    }
}

bool CfgCalibrateCfgModel::setDivisionTorqueCaliOnMotor(const quint32 idxMotor, const double value){
    bool result = false;
    if (idxMotor < MAX_SUPPORT_MOTOR){
        CfgCalibrateCfgPerMotor[idxMotor].divisionOnTorque = value;
        result = true;
    }
    else
    {
        qWarning() << "Could not exceed the maximum supported motor " << MAX_SUPPORT_MOTOR;
    }
    return result;
}

void CfgCalibrateCfgModel::loadSetting()
{
//    int size = 2;
//    m_set.beginWriteArray("cfg/calibrate");
//    for (int i = 0; i < size; ++i) {
//        m_set.setArrayIndex(i);
//        m_set.setValue("divisionOnThrust", defaultDivisionOnThrust[i]);
//        m_set.setValue("divisionOnTorque", defaultDivisionOnThrust[i]);
//    }
//    m_set.endArray();

    quint32 size = static_cast<quint32>(m_set.beginReadArray("cfg/calibrate"));
    size = size > MAX_SUPPORT_MOTOR ? MAX_SUPPORT_MOTOR : size;

    for (quint32 i = 0; i < size; ++i) {
         m_set.setArrayIndex(i);

         struct CfgCalibrateCfgPerMotor& cfgCali = CfgCalibrateCfgPerMotor[i];
         cfgCali.divisionOnThrust = m_set.value("divisionOnThrust", defaultDivisionOnThrust[i]).toDouble();
         cfgCali.divisionOnTorque = m_set.value("divisionOnTorque", defaultDivisionOnTorque[i]).toDouble();
    }
    m_set.endArray();
}
