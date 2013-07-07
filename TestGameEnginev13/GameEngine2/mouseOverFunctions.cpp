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
    
    short total[4]={0,0,0,0};
    for(unsigned int k=0; k<allgarrisonedselectedunits[0].size(); k++)
    {
        short price[4]={allbuildableunits[index].foodontraining,allbuildableunits[index].woodontraining,allbuildableunits[index].goldontraining,allbuildableunits[index].stoneontraining};

        for(unsigned int i=0; i<allbuildableunits[allunits[0][allgarrisonedselectedunits[0][k]]->id].allPriceMods.size(); i++)
        {
            if(index==allbuildableunits[allunits[0][allgarrisonedselectedunits[0][k]]->id].allPriceMods[i].forWhat)
            {
                for(int j=0; j<4; j++)
                {
                    price[j]=allbuildableunits[allunits[0][allgarrisonedselectedunits[0][k]]->id].allPriceMods[i].prices[j];
                }
                break;
            }
        }
        for(int i=0; i<4; i++)
            total[i]+=price[i];
    }
    
    string s;
    int sum=0;
    for(int i=0; i<4; i++)
        sum+=total[i];
    if(sum==0) //total is empty
    {
        total[0]=allbuildableunits[index].foodontraining;
        total[1]=allbuildableunits[index].woodontraining;
        total[2]=allbuildableunits[index].goldontraining;
        total[3]=allbuildableunits[index].stoneontraining;
        s+="Generic Cost   ";
    }
    else
        s+="Total   ";
    
    s+="Food: ";
   // s+=inttostring(allbuildableunits[index].foodontraining);
    s+=inttostring(total[0]);
    s+=string(" Wood: ");
    s+=inttostring(total[1]);
    //s+=inttostring(allbuildableunits[index].woodontraining);
    s+=string(" Gold: ");
    s+=inttostring(total[2]);
    //s+=inttostring(allbuildableunits[index].goldontraining);
    s+=string(" Stone: ");
    s+=inttostring(total[3]);
    //s+=inttostring(allbuildableunits[index].stoneontraining);
    return s;
}

string researchSomething(int index)
{
    index=allbuttons[allMouseOver[index].dispwith].unitorbuilding;
    string s("Food: ");
    s+=inttostring(allResearches[index].price[0]);
    s+=string(" Wood: ");
    s+=inttostring(allResearches[index].price[1]);
    s+=string(" Gold: ");
    s+=inttostring(allResearches[index].price[2]);
    s+=string(" Stone: ");
    s+=inttostring(allResearches[index].price[3]);
    return s;
}