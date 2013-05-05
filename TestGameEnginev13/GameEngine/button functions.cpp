#include "globals.h"

//! \file "button functions.cpp" This contains functions for the function pointer in the button class

void makereg(buttonparam b)
{
	if(selectedunits[b.player].size()>=MINREG && selectedunits[b.player].size()<=MAXREG)
	{
		allregiments[b.player].push_back(regiment(designatedunit[b.player],b.player,allregiments[b.player].size()));
		if(!allregiments[b.player][allregiments[b.player].size()-1].addunits(selectedunits[b.player])) //add the selected units to the regiment, if failed delete regiment
			allregiments[b.player].pop_back();
	}
}
void setstance(buttonparam b)
{
	for(unsigned int i=0; i<selectedunits[0].size(); i++)
		allunits[0][selectedunits[0][i]]->unitstance=b.index-indexStancebutton+US_AGGRESIVE; //sets the unit stance to the correct value according to the button pressed. indexStancebutton indicates what index the first button is at, and US_AGGRESIVE makes it start with the stances, instead of before them
}
void designate(buttonparam b)
{
	designatedunit[b.player]=selectedunits[b.player][0];
}
void build(buttonparam b)
{
	buildinghover=true;
	buildingwidth=(int)allbuildablebuildings[b.unitorbuilding].width;
	buildingheight=(int)allbuildablebuildings[b.unitorbuilding].height;
	buildingwhat=b.unitorbuilding;
}
void createnewunit(buttonparam b)
{
	allbuildings[b.player][selectedunits[b.player][0]].createunit(b.unitorbuilding, -1, -1);
}
void createunit(buttonparam b)
{
	for(unsigned int i=0; i<allgarrisonedselectedunits[0].size(); i++)
	{
		allbuildings[b.player][-allunits[b.player][allgarrisonedselectedunits[b.player][i]]->garrisoned-1].createunit(b.unitorbuilding,allgarrisonedselectedunits[b.player][i]);
	}
}
void selectgarrison(buttonparam b)
{
	for(unsigned int i=0; i<allgarrisonedselectedunits[b.player].size(); i++)
	{
		if(allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]==allgarrisonedselectedunits[b.player][i])
		{
			allgarrisonedselectedunits[b.player].erase(allgarrisonedselectedunits[b.player].begin()+i);
			return;
		}
	}
	allgarrisonedselectedunits[b.player].push_back(allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]);
}
void ungarrisonall(buttonparam b)
{
	redraw=true;
	for(unsigned int i=0; i<allbuildings[b.player][selectedunits[b.player][0]].garrisoned.size(); i++)
	{
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->garrisoned=0;
		point p=allbuildings[b.player][selectedunits[b.player][0]].checksurroundingarea(allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->whatisit,allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->width,allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->height);
		if(p.x==-1 && p.y==-1)
			return; //failed. PRINT FAIL MESSAGE
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x=p.x;
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y=p.y;
		//allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x=(int)allbuildings[b.player][selectedunits[b.player][0]].x+((i*2)%(int)allbuildings[b.player][selectedunits[b.player][0]].width);//+((int)allbuildings[b.player][selectedunits[b.player][0]].x+(i*2))%(int)allbuildings[b.player][selectedunits[b.player][0]].width-1;//(int)allbuildings[b.player][selectedunits[b.player][0]].width%2;
		//allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y=allbuildings[b.player][selectedunits[b.player][0]].y+(int)(i/allbuildings[b.player][selectedunits[b.player][0]].width*2)+allbuildings[b.player][selectedunits[b.player][0]].height+1;
		map[(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y][(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x].uniton=true;
		map[(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y][(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x].player=b.player;
		map[(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y][(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x].index=allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i];
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->movetox=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x;
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->movetoy=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y;
		
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->boundingbox.top=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y;
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->boundingbox.left=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x;
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->boundingbox.right=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->x+allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->width;
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->boundingbox.bottom=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->y+allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->height;
		
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->userordered=false;
		allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[i]]->selected=false;
	}
	allbuildings[b.player][selectedunits[b.player][0]].garrisoned.clear();
}
void ungarrison(buttonparam b)
{
	redraw=true;
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->garrisoned=0;
	point p=allbuildings[b.player][selectedunits[b.player][0]].checksurroundingarea(allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->whatisit,allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->width,allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->height);
	if(p.x==-1 && p.y==-1)
		return; //fail PRINT MESSAGE
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x=p.x;
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y=p.y;
	//allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x=allbuildings[b.player][selectedunits[b.player][0]].x+((b.index-indexGarrisonbutton)*2);
	//allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y=allbuildings[b.player][selectedunits[b.player][0]].y+(int)((b.index-indexGarrisonbutton)/allbuildings[b.player][selectedunits[b.player][0]].width)*2+allbuildings[b.player][selectedunits[b.player][0]].height+1;
	map[(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y][(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x].uniton=true;//(int)allbuildings[b.player][selectedunits[b.player][0]].y+((int)(b.index-indexGarrisonbutton)/allbuildings[b.player][selectedunits[b.player][0]].width)*2+(int)allbuildings[b.player][selectedunits[b.player][0]].height+1][(int)allbuildings[b.player][selectedunits[b.player][0]].x+(b.index-indexGarrisonbutton*2)].uniton=true;
	map[(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y][(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x].player=b.player;//(int)allbuildings[b.player][selectedunits[b.player][0]].y+((int)(b.index-indexGarrisonbutton)/allbuildings[b.player][selectedunits[b.player][0]].width)*2+(int)allbuildings[b.player][selectedunits[b.player][0]].height+1][(int)allbuildings[b.player][selectedunits[b.player][0]].x+(b.index-indexGarrisonbutton*2)].player=b.player;
	map[(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y][(int)allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x].index=allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton];//(int)allbuildings[b.player][selectedunits[b.player][0]].y+((int)(b.index-indexGarrisonbutton)/allbuildings[b.player][selectedunits[b.player][0]].width)*2+(int)allbuildings[b.player][selectedunits[b.player][0]].height+1][(int)allbuildings[b.player][selectedunits[b.player][0]].x+(b.index-indexGarrisonbutton*2)].index=allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton];
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->movetox=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x;
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->movetoy=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y;
	
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->boundingbox.top=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y;
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->boundingbox.left=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x;
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->boundingbox.right=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->x+allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->width;
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->boundingbox.bottom=allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->y+allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->height;

	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->userordered=false;
	allunits[b.player][allbuildings[b.player][selectedunits[b.player][0]].garrisoned[b.index-indexGarrisonbutton]]->selected=false;

	allbuildings[b.player][selectedunits[b.player][0]].garrisoned.erase(allbuildings[b.player][selectedunits[b.player][0]].garrisoned.begin()+(b.index-indexGarrisonbutton));
}
void leaveship(buttonparam b)
{
	redraw=true;
	point p=allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->checksurroundingarea(allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->width,allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->height);
	if(p.x==-1 && p.y==-1)
		return; //fail PRINT MESSAGE
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->x=p.x;
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->y=p.y;
	
	map[(int)allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->y][(int)allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->x].uniton=true;
	map[(int)allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->y][(int)allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->x].player=b.player;
	map[(int)allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->y][(int)allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->x].index=allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton];
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->movetox=allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->x;
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->movetoy=allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->y;

	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->boundingbox.top=allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->y;
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->boundingbox.left=allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->x;
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->boundingbox.right=allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->x+allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->width;
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->boundingbox.bottom=allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->y+allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->height;

	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->userordered=false;
	allunits[b.player][allunits[b.player][selectedunits[b.player][0]]->unitsinside[b.index-indexSailorsbutton]]->selected=false;
	
	allunits[b.player][selectedunits[b.player][0]]->unitsinside.erase(allunits[b.player][selectedunits[b.player][0]]->unitsinside.begin()+(b.index-indexSailorsbutton));
}
//! This begins recording a report concerning all that the selected unit's regiment has seen or done
void beginrecordreport(buttonparam b)
{
	allregiments[b.player][allunits[b.player][selectedunits[b.player][0]]->regimentid].recording=true;
	for(unsigned int i=0; i<allregiments[b.player][allunits[b.player][selectedunits[b.player][0]]->regimentid].unitids.size(); i++)
		allunits[b.player][allregiments[b.player][allunits[b.player][selectedunits[b.player][0]]->regimentid].unitids[i]]->recording=true;
}
//! This gives whatever report the unit has access to
void givereport(buttonparam b)
{
	if(allunits[b.player][selectedunits[b.player][0]]->recording==true)
		allregiments[b.player][allunits[b.player][selectedunits[b.player][0]]->regimentid].rep.give();
	else
		allunits[b.player][selectedunits[b.player][0]]->rep.give();
}
//! This flips to the next page
void nextpage(buttonparam b)
{
	if(pagenum==0)
		pagenum=1;
	else if(pagenum==1)
		pagenum=0;
	redraw=true;
}
