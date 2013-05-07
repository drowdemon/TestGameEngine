#include "regiment.h"
#include "unit.h"
#include "globals.h"

extern vector <vector <unit*> > allunits;

//! \file regiment.cpp This is the file that contains the regiment class

regiment::regiment(short lieu, short p, short i)
{
	lieutenant=lieu;
	player=p;
	index=i;
	reglvl=0;
}
void regiment::calcreglvl()
{
	int min=100000;
	float average=0;
	for(unsigned int i=0; i<unitids.size(); i++)
	{
		if(allunits[player][unitids[i]]->veterancylvl<min)
			min=allunits[player][unitids[i]]->veterancylvl;
		average+=allunits[player][unitids[i]]->veterancylvl;
	}
	average/=(float)unitids.size();
	if(min+2<average)
		reglvl=min+2;
	else
	{
		if(average-floor(average)<0.5)
			floor(average);
		else
			ceil(average);
		reglvl=(short)average;
	}
}
bool regiment::addunits(short id)
{
	if(unitids.size()<MINREG-1)  //FIX THIS IT WAS 19. ITS 2 FOR TESTING ONLY!!!!!//minsize=20, if its 19, adding one will make it 20, so its allowed   
		return false;
	if(unitids.size()>=MAXREG) //too big, can't add more.
		return false;
	if(allunits[player][id]->regimentid!=-1) //already in a regiment
		return false;
	unitids.push_back(id);
	allunits[player][id]->regimentid=index;
	calcreglvl();
	return true;
}
bool regiment::addunits(vector <short> &ids)
{
	if(unitids.size()+ids.size()<MINREG) //FIX THIS IT SHOULD BE 20!!!! THIS IS WRONG. TESTING ONLY//not enough after addition
		return false;
	if(unitids.size()+ids.size()>MAXREG) //Will become too big after addition
		return false;
	for(unsigned int i=0; i<ids.size(); i++)
	{
		if(allunits[player][i]->regimentid!=-1) //already in a regiment
			return false;
	}
	unitids.insert(unitids.end(), ids.begin(), ids.end());
	for(unsigned int i=0; i<ids.size(); i++)
		allunits[player][i]->regimentid=index;
	calcreglvl();
	return true;
}
void regiment::removeunit(short id)
{
	for(unsigned int i=0; i<unitids.size(); i++)
	{
		if(id==unitids[i])
		{
			if(recording==true)
				rep.unitslost.push_back(reportunitinfo(allunits[player][unitids[i]]->id,allunits[player][unitids[i]]->veterancylvl,((allunits[player][unitids[i]]->lieutenant==0) ? false : true),((allunits[player][unitids[i]]->lieutenant==0) ? -1 : allunits[player][unitids[i]]->lieutenant)));
			unitids.erase(unitids.begin()+i);
			break;
		}
	}
}
