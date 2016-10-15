#ifndef CFGRESHANDLERINF_H
#define CFGRESHANDLERINF_H

#include <qglobal.h>
#include <QtPlugin>

class CfgResHandlerInf
{
public:
    virtual ~CfgResHandlerInf() {}

    virtual quint32 boot_delay() const = 0;
    virtual quint32 boot_rape() const = 0;
    virtual quint32 duration() const = 0;

    virtual quint32 vane() const = 0;
    virtual quint32 HZ() const = 0;
};

Q_DECLARE_INTERFACE(CfgResHandlerInf, "CfgResHandlerInf/1.0")

#endif // CFGRESHANDLERINF_H
