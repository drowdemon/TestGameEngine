#include <cstdlib>
#include "display functions.h"
#include "globals.h"
string inttochar(int conv)
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
	for(int i=0; i<ret.size()/2; i++)
		swap(ret[i],ret[ret.size()-1-i]);
	return ret;
}
string dispresourcesunit(int index)
{
	return inttochar(allunits[0][selectedunits[0][0]]->holding[index-indexResourcedispunit]);
}
string dispresourcesbuilding(int index)
{
	return inttochar(allbuildings[0][selectedunits[0][0]].holding[index-indexResourcedispbuilding]);
}
string disphealth(int index)
{
	return inttochar((int)allunits[0][selectedunits[0][0]]->health);
}
string dispname(int index)
{
	return allbuildableunits[allunits[0][selectedunits[0][0]]->id].name;
}
string dispbuildinghealth(int index)
{
	return inttochar((int)allbuildings[0][selectedunits[0][0]].health);
}
string dispbuildingname(int index)
{
	return allbuildablebuildings[allbuildings[0][selectedunits[0][0]].id].name;
}
string dispgarrissoned(int index)
{
	if((unsigned int)(index-indexGarrisondisp)<allbuildings[0][selectedunits[0][0]].garrisoned.size())
		return allbuildableunits[allunits[0][allbuildings[0][selectedunits[0][0]].garrisoned[index-indexGarrisondisp]]->id].name;
	else
		return "";
}
string dispsailors(int index)
{
	if((unsigned int)(index-indexSailorsdisp)<allunits[0][selectedunits[0][0]]->unitsinside.size())
		return allbuildableunits[allunits[0][allunits[0][selectedunits[0][0]]->unitsinside[index-indexSailorsdisp]]->id].name;
	else
		return "";
}
string dispmoral(int index)
{
	return inttochar((int)allunits[0][selectedunits[0][0]]->moral);
}
string dispfoodrequired(int index)
{
	return inttochar((int)allunits[0][selectedunits[0][0]]->foodconsumed);
}
