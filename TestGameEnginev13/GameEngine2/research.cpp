#include "research.h"
#include "baseUnitBuildings2.h"

using namespace std;

extern vector<basicbuilding> allbuildablebuildings;

Research::Research(short cw, short food, short wood, short gold, short stone, short t, short rw, short arm, short att, short r, short acc, short fs, short ws, short gs, short ss, short ba, short bs, short mg, short l, string n)
{
    changewhat=cw;
    price[0]=food;
    price[1]=wood;
    price[2]=gold;
    price[3]=stone;
    time=t;
    researchwhere=rw;
    armor=arm;
    attack=att;
    range=r;
    accuracy=acc;
    gather[0]=fs;
    gather[1]=ws;
    gather[2]=gs;
    gather[3]=ss;
    buildingattack=ba;
    buildspeed=bs;
    maxgarrison=mg;
    los=l;
    name=n;
}

bool Research::checkResearch(int unitid, bool building)
{
    short whatisit=0;
    if(!building)
    {
        if(unitid==3)
            whatisit|=SHIPS;
        if(unitid>=4 && unitid<=6)
            whatisit|=INFANTRY;
        if(unitid==7 || unitid==8)
            whatisit|=ARCHERS;
        if(unitid>=9 && unitid<=16)
            whatisit|=INFANTRY;
        if(unitid>=17 && unitid<=19)
            whatisit|=ARCHERS;
        if(unitid==20 || unitid==21)
            whatisit|=SCOUTS;
        if(unitid>=22 && unitid<=24)
            whatisit|=ARCHER_CAVALRY;
        if(unitid>=25 && unitid<=27)
            whatisit|=HEAVY_CAVALRY;
        if(unitid>=28 && unitid<=31)
            whatisit|=TRANSPORT;
        if(unitid==32 || unitid==33)
            whatisit|=SPIES;
        if(unitid==33)
            whatisit|=ASSASSINS;
        if((whatisit&changewhat)>0)
            return true;
        else
            return false;
    }
    else //its a building
    {
        if(allbuildablebuildings[unitid].landorwater) //land
            whatisit|=LAND_BUILDINGS;
        else
            whatisit|=WATER_BUILDINGS;
        if((whatisit&changewhat)>0)
            return true;
        else
            return false;
    }
}