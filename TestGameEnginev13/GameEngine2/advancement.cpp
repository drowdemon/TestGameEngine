#include "advancement.h"
#include "globals.h"

advancement::advancement(vector<advReq> ur, vector<advReq> br, int ui, int uw)
{
    unitsreq=ur;
    buildingsreq=br;
    
    unlockIndex=ui;
    unlockWhat=uw;
}
bool advancement::checkAdv(short player)
{
    bool metagood=true;
    if(unitsreq.size()!=0)
    {
        vector<short> owned;
        owned.resize(allbuildableunits.size());
        for(unsigned int i=0; i<allunits[player].size(); i++)
        {
            owned[allunits[player][i]->id]++;
        }
        bool good=true;
        for(unsigned int i=0; i<unitsreq.size(); i++)
        {
            if(unitsreq[i].amntreq>owned[unitsreq[i].whatsreq])
            {
                good=false;
                break;
            }
        }
        if(!good)
            metagood=false;
    }
    if(!metagood)
        return false;
    
    if(buildingsreq.size()!=0)
    {
        vector<short> owned;
        owned.resize(allbuildablebuildings.size());
        for(unsigned int i=0; i<allbuildings[player].size(); i++)
        {
            owned[allbuildings[player][i].id]++;
        }
        bool good=true;
        for(unsigned int i=0; i<buildingsreq.size(); i++)
        {
            if(buildingsreq[i].amntreq>owned[buildingsreq[i].whatsreq])
            {
                good=false;
                break;
            }
        }
        if(!good)
            metagood=false;
    }
    if(metagood)
    {
        if(unlockWhat==UNITUNLOCK)
            unitAllowed[player][unlockIndex]=true;
        else if(unlockWhat==BUILDINGUNLOCK)
            buildingAllowed[player][unlockIndex]=true;
    }
    
    return metagood;
}

string advancement::unlockMessage()
{
    string ret;
    if(unlockWhat==UNITUNLOCK)
    {
        ret+="unit ";
        ret+=allbuildableunits[unlockIndex].name;
    }
    else if(unlockWhat==BUILDINGUNLOCK)
    {
        ret+="building ";
        ret+=allbuildablebuildings[unlockIndex].name;
    }
    for(unsigned int i=0; i<ret.length(); i++)
    {
        if(ret[i]=='_')
            ret[i]=' ';
    }
    return ret;
}