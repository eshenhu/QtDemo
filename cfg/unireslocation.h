#ifndef UNIRESLOCATION_H
#define UNIRESLOCATION_H

#include "cfg/cfgjsonprimaryelement.h"
#include "cfg/cfgreshandler.h"

class UniResLocation{

    UniResLocation() = default;

private:
    static CfgResHandler* cfgHdl;
    static CfgGUIJsonReader* cfgReader;

public:
    static CfgResHandler* getCfgResHdl(){
        if (!cfgHdl)
            cfgHdl = new CfgResHandler();
        return cfgHdl;
    }

    static CfgGUIJsonReader* getCfgJsonHdl(){
        if (!cfgReader)
        {
            cfgReader = new CfgGUIJsonReader();

            const QString pv = CfgResHandlerInf::getPVString(UniResLocation::getCfgResHdl()->prod_version());
            cfgReader->load(pv);
        }
        return cfgReader;
    }
};

#endif // UNIRESLOCATION_H
