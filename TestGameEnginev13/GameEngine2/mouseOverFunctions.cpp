#include <string>
#include <vector>
#include "baseUnitBuildings2.h"
#include "globals.h"

using namespace std;

//extern vector<basicbuilding> allbuildablebuildings;

string inttostring(int conv)
{
	string ret;
	if(conv==0)
	{
		ret="0";
		return ret;
	}
	while(conv>=1)
	{
		ret.push_back(char((conv%10)+48));
		conv/=10;
	}
	for(unsigned int i=0; i<ret.size()/2; i++)
		swap(ret[i],ret[ret.size()-1-i]);
	return ret;
}

string buildBuilding(int index)
{
    index=allMouseOver[index].dispwith+1;
    string s("Food: ");
    s+=inttostring(allbuildablebuildings[index-indexBuildingsbutton].foodtobuild);
    s+=string(" Wood: ");
    s+=inttostring(allbuildablebuildings[index-indexBuildingsbutton].woodtobuild);
    s+=string(" Gold: ");
    s+=inttostring(allbuildablebuildings[index-indexBuildingsbutton].goldtobuild);
    s+=string(" Stone: ");
    s+=inttostring(allbuildablebuildings[index-indexBuildingsbutton].stonetobuild);
    return s;
}

string nullMouseFunc(int index)
{
    return "";
}

string buildUnit(int index)
{
    index=allbuttons[allMouseOver[index].dispwith].unitorbuilding;
    string s("Food: ");
    s+=inttostring(allbuildableunits[index].foodontraining);
    s+=string(" Wood: ");
    s+=inttostring(allbuildableunits[index].woodontraining);
    s+=string(" Gold: ");
    s+=inttostring(allbuildableunits[index].goldontraining);
    s+=string(" Stone: ");
    s+=inttostring(allbuildableunits[index].stoneontraining);
    return s;
}