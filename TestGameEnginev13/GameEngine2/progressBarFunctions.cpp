#include "unit.h"
#include "building.h"
#include "globals.h"

double progressBuilding(int index)
{
    if(allunits[0][index]->garrisoned>0 && allbuildings[0][allunits[0][index]->garrisoned-1].beingbuilt>0)
    {
        redraw=1;
        return 1.0-(double)allbuildings[0][allunits[0][index]->garrisoned-1].beingbuilt/(double)allbuildablebuildings[allbuildings[0][allunits[0][index]->garrisoned-1].id].timetobuild;
    }
    else
        return -1.0;
}
double progressUnit(int index) //make system for multiple units
{
    if(creationqueueunits[0][index].size()==0)
        return -1.0;
    else
    {
        redraw=1;
        return 1.0-(double)creationqueueunits[0][index][0].timeremaining/(double)allbuildableunits[creationqueueunits[0][index][0].createwhat].timeontraining;
    }
}
double progressResearch(int index)
{
    bool good=false;
    unsigned int i=0;
    for(; i<allCurrResearch[0].size(); i++)
    {
        if(allCurrResearch[0][i].researchingWhere==index) //researching at selected building
        {
            good=true;
            break;
        }
    }
    if(!good)
        return -1;
    else
    {
        redraw=1;
        return 1.0-((double)allCurrResearch[0][i].timeleft/(double)allResearches[allCurrResearch[0][i].researchingWhat].time);
    }
}