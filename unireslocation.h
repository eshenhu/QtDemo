#ifndef UNIRESLOCATION_H
#define UNIRESLOCATION_H

#include "cfg/cfgjsonprimaryelement.h"
#include "cfg/cfgreshandler.h"

class UniResLocation{

    UniResLocation() = default;

public:
    static CfgResHandler* getCfgResHdl(){
        static CfgResHandler* cfgHdl = new CfgResHandler();
        return cfgHdl;
    }

    static CfgJsonReader* getCfgJsonHdl(){
        static CfgJsonReader* cfgReader = new CfgJsonReader();
        cfgReader->load("PV11");
        return cfgReader;
    }
};

#endif // UNIRESLOCATION_H
