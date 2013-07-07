#include "unit.h"
#include "building.h"
#include "globals.h"
#include "astarsearch.h"
#include <cmath>
#include <stdio.h>
#include <cstdlib>

using namespace std;

//! \file unit.cpp This contains class unit

unit::unit(float h, short ma, short ra, short a, short ba, float LOS, float s, short as, short fc, /*short ft, short wt, short gt, short st, short tt=0, */short sn, float px, float py, float ar, short pid, float c, short p, short i, short high, short w, short bs, short mxhld, unsigned char wisit, unsigned char ming, unsigned char maxg, float m, float ts, short vetlvl, float xp, float mx, float my, short si, char us, bool sotr, float dx, float dy, short drad, short spec, /*short tr,*/ short regid, bool lieut) : baseunit(h, ma, ra, a, ba, LOS, s, as, fc, sn, ar, pid, c, high, w, bs, mxhld, wisit, ming, maxg)
{
	/*health=h;
	meleeattack=ma;
	rangedattack=ra;
	armor=a;
	buildingattack=ba;
	los=LOS;
	speed=s;
	attackspeed=as;
	foodconsumed=fc;
	sleepneeded=sn;
	attackrange=ar;
	id=pid;
	camouflage=c;
	height=high;
	width=w;
	buildspeed=bs;
	maxhold=mxhld;
	whatisit=wisit;
	mingarrison=ming;
	maxgarrison=maxg;*/
	x=px;
	y=py;
	player=p;
	index=i;

	moral=m;
	timeslept=ts;
	veterancylvl=vetlvl;
	exp=xp;
	if(mx!=-1 && my!=-1)
	{
		movetox=mx;
		movetoy=my;
	}
	else
	{
		movetox=x;
		movetoy=y;
	}
	siegeindex=si;
	unitstance=us;
	stayoutoftowerrange=sotr;
	dstancecoox=dx;
	dstancecooy=dy;
	dstancerad=drad;
	specialty=spec;
	//takeresourcesfrom=tr;
	regimentid=regid;
	lieutenant=lieut;

	selected=false;
	//actualtomovex=-1;
	//actualtomovey=-1;
	attackingunitplayer=-1;
	attackingunitindex=-1;
	userordered=false;
	attackingwhat=false;
	prevsiegeindex=-1;
	garrisoned=0;
	gatheringx=-1;
	gatheringy=-1;
	gatheringwhat=-1;
	nearesthold=-1;
	for(int i=0; i<4; i++)
		holding[i]=0;
	for(int i=0; i<8; i++)
		specexperience[i]=0;
	//boundingbox.right=boundingbox.left=boundingbox.top=boundingbox.bottom=0;
	boundingbox.left=x;
	boundingbox.right=x+width;
	boundingbox.top=y;
	boundingbox.bottom=y+height;
	training=false;
	goingobstaclecounter=0;
	firstobstacleattempt=true;
	lieutexp=0;
	lieuttrain=false;
	recording=false;
	transferring=0;
	transferfrom=-1;
	transferto=-1;
    unitbuildingwhat=-1;
    buildingx=-1;
    buildingy=-1;
    buildingResourcesFrom=-1;
    selfRecording=false;
}
unit::unit(unsigned char hlding[4], float se[8], myrect bb, float h, short ma, short ra, short a, short ba, float LOS, float s, short as, short fc, /*short ft, short wt, short gt, short st, short tt, */short sn, float px, float py, float ar, short pid, float c, short p, short i, short high, short w, short bs, short mxhld, unsigned char wisit, unsigned char ming, unsigned char maxg, float m, float ts, short vetlvl, float xp, float mx, float my, short si, char us, bool sotr, float dx, float dy, short drad, short spec, /*short tr,*/ short regid, bool lieut) : baseunit(h, ma, ra, a, ba, LOS, s, as, fc, sn, ar, pid, c, high, w, bs, mxhld, wisit, ming, maxg)
{
	/*health=h;
	meleeattack=ma;
	rangedattack=ra;
	armor=a;
	buildingattack=ba;
	los=LOS;
	speed=s;
	attackspeed=as;
	foodconsumed=fc;
	sleepneeded=sn;
	attackrange=ar;
	id=pid;
	camouflage=c;
	height=high;
	width=w;
	buildspeed=bs;
	maxhold=mxhld;
	whatisit=wisit;
	mingarrison=ming;
	maxgarrison=maxg;*/
	x=px;
	y=py;
	player=p;
	index=i;

	moral=m;
	timeslept=ts;
	veterancylvl=vetlvl;
	exp=xp;
	if(mx!=-1 && my!=-1)
	{
		movetox=mx;
		movetoy=my;
	}
	else
	{
		movetox=x;
		movetoy=y;
	}
	siegeindex=si;
	unitstance=us;
	stayoutoftowerrange=sotr;
	dstancecoox=dx;
	dstancecooy=dy;
	dstancerad=drad;
	specialty=spec;
	//takeresourcesfrom=tr;
	regimentid=regid;
	lieutenant=lieut;

	selected=false;
	//actualtomovex=-1;
	//actualtomovey=-1;
	attackingunitplayer=-1;
	attackingunitindex=-1;
	userordered=false;
	attackingwhat=false;
	prevsiegeindex=-1;
	garrisoned=0;
	gatheringx=-1;
	gatheringy=-1;
	gatheringwhat=-1;
	nearesthold=-1;
	for(int i=0; i<4; i++)
		holding[i]=hlding[i];
	for(int i=0; i<8; i++)
		specexperience[i]=se[i];
	boundingbox=bb;
	training=false;
	goingobstaclecounter=0;
	firstobstacleattempt=true;
	lieutexp=0;
	lieuttrain=false;
	recording=false;
	transferring=0;
	transferfrom=-1;
	transferto=-1;
    unitbuildingwhat=-1;
    buildingx=-1;
    buildingy=-1;
    buildingResourcesFrom=-1;
    selfRecording=false;
}
unit::unit(basicunit u, float px, float py, short p, short i, float m, float ts, short vetlvl, float xp, float mx, float my, short si, char us, bool sotr, float dx, float dy, short drad, short spec, /*short tr,*/ short regid, bool lieut) : baseunit(u.health, u.meleeattack, u.rangedattack, u.armor, u.buildingattack, u.los, u.speed, u.attackspeed, u.foodconsumed, u.sleepneeded, u.attackrange, u.id, u.camouflage, u.height, u.width, u.buildspeed, u.maxhold, u.whatisit, u.mingarrison, u.maxgarrison, u.attackarea, u.chanceHit)
{
	x=px;
	y=py;
	player=p;
	index=i;

	moral=m;
	timeslept=ts;
	veterancylvl=vetlvl;
	exp=xp;
	if(mx!=-1 && my!=-1)
	{
		movetox=mx;
		movetoy=my;
	}
	else
	{
		movetox=x;
		movetoy=y;
	}
	siegeindex=si;
	unitstance=us;
	stayoutoftowerrange=sotr;
	dstancecoox=dx;
	dstancecooy=dy;
	dstancerad=drad;
	specialty=spec;
	regimentid=regid;
	lieutenant=lieut;

	selected=false;
	//actualtomovex=-1;
	//actualtomovey=-1;
	attackingunitplayer=-1;
	attackingunitindex=-1;
	userordered=false;
	attackingwhat=false;
	prevsiegeindex=-1;
	garrisoned=0;
	gatheringx=-1;
	gatheringy=-1;
	gatheringwhat=-1;
	nearesthold=-1;
	for(int i=0; i<4; i++)
		holding[i]=0;
	for(int i=0; i<8; i++)
		specexperience[i]=0;
	//boundingbox.right=boundingbox.left=boundingbox.top=boundingbox.bottom=0;
	boundingbox.left=x;
	boundingbox.right=x+width;
	boundingbox.top=y;
	boundingbox.bottom=y+height;
	training=false;
	//takeresourcesfrom=tr;
	goingobstaclecounter=0;
	firstobstacleattempt=true;
	lieutexp=0;
	lieuttrain=false;
	recording=false;
	transferring=0;
	transferfrom=-1;
	transferto=-1;
    unitbuildingwhat=-1;
    buildingx=-1;
    buildingy=-1;
    buildingResourcesFrom=-1;
    selfRecording=false;
}
void unit::attackgoingtobstacle(char canmoveto[6])
{
	float attackx=0;
	float attacky=0;
	float attackh=0;
	float attackw=0;
	if(attackingwhat==true)
	{
		attackx=allunits[attackingunitplayer][attackingunitindex]->x;
		attacky=allunits[attackingunitplayer][attackingunitindex]->y;
		attackh=allunits[attackingunitplayer][attackingunitindex]->height;
		attackw=allunits[attackingunitplayer][attackingunitindex]->width;
	}
	else
	{
		attackx=allbuildings[attackingunitplayer][attackingunitindex].x;
		attacky=allbuildings[attackingunitplayer][attackingunitindex].y;
		attackh=allbuildings[attackingunitplayer][attackingunitindex].height;
		attackw=allbuildings[attackingunitplayer][attackingunitindex].width;
	}
	float attackcenterx=attackx+(attackw/2.0f);
	float attackcentery=attacky+(attackh/2.0f);
	float startx=0;
	float starty=0;
	bool dir=true; //true==counterclockwise, false==clockwise
    
    float userange=attackrange;
    for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
    {
        if(allResearches[alreadyResearched[player][i]].checkResearch(id))
        {
            userange+=allResearches[alreadyResearched[player][i]].range;
        }
    }
    
	if(movetox>=attackcenterx)
	{
		startx=attackcenterx+(attackw/2)+userange;
		starty=attackcentery;
		if(movetoy>=attackcentery)
			dir=true;
		else if(movetoy<attackcentery)
			dir=false;
	}
	else if(movetox<=attackcenterx)
	{
		startx=attackcenterx-(attackw/2)-userange;
		starty=attackcentery;
		if(movetoy>=attackcentery)
			dir=false;
		else if(movetoy<attackcentery)
			dir=true;
	}
	float i=startx;
	float j=starty;
	int numswitch=0;
	int changerad=0;
	while(true)
	{
		for(int k=0; k<6; k++)
		{
			if((map[(int)j][(int)i].unitonMineon&1)==1)
				continue;
			if(map[(int)j][(int)i].tilestyle==canmoveto[k] || (map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle==TS_GATE && map[(unsigned int)movetoy][(unsigned int)movetox].buildingplayer==players[player]))
			{
				movetox=i;
				movetoy=j;
				return;
			}
		}
		if(startx>=attackcenterx)
			j-=0.5f;
		else
			j+=0.5f;
		if(j==startx+2*(attackw+userange-changerad) || j==startx-2*(attackw+userange-changerad))
		{
			dir=!dir;
			numswitch++;
			if(numswitch%2==0)
			{
				if(changerad<(attackw+userange))
				{
					changerad++;
					if(changerad>=(attackw+userange))
					{
						changerad=0;
						j=startx;
					}
					else if(startx>=attackcenterx)
						j--;
					else
						j++;
				}
				if(changerad>=(attackw+userange))
				{
					changerad--;
					if(startx>=attackcenterx)
						j++;
					else
						j--;
				}
			}
		}
		if(dir==true)
			i=attackcentery+sqrt(pow((attackcenterx+attackw+userange-changerad), 2)-pow(j-attackcenterx,2));
		else
			i=attackcentery-sqrt(pow((attackcenterx+attackw+userange-changerad), 2)-pow(j-attackcenterx,2));				
	}
}
void unit::goingtobstacle0(char canmoveto[6])
{
	for(int rad=1; ; rad++)
	{
		for(int i=0; i<rad; i++)
		{
			for(int k=1; k>=-1; k-=2)
			{
				for(int h=1; h>=-1; h-=2)
				{
					float newy=float(sqrt((rad*rad)-pow(/*movetox+*/float(i*k), 2)));
					if((int)newy>=newy+.0005 || (int)newy<=newy-.0005)
						continue;
					bool good=false;
					for(int j=0; j<6; j++)
					{
						if(movetox+(i*h)>=0 && movetox+(i*h)<map.size() && movetoy+newy>=0 && movetoy+newy<MAPSIZE)
						{
							if(map[(unsigned int)(newy+movetoy)][(unsigned int)(movetox+(i*h))].tilestyle==canmoveto[j] && (map[(unsigned int)(newy+movetoy)][(unsigned int)(movetox+(i*h))].unitonMineon&1)==0)
							{
								good=true;
								break;
							}
						}
					}
					if(good==true)
					{
						movetox=movetox+(i*h);
						movetoy=newy+movetoy;
						return;
					}
				}
			}
		}
	}
}
void unit::goingtobstacle() //horizontal line
{
	for(int j=0; j+movetoy<map.size(); j++)
	{
		bool good=false;
		for(int i=0; i<2; i++)
		{
			if(chkmvp1(map[(unsigned int)movetoy+j][(unsigned int)movetox+i], false)==true && chkmvp1(map[(unsigned int)movetoy+j][(unsigned int)movetox+i+width],false)==true && chkmvp1(map[(unsigned int)movetoy+j+height][(unsigned int)movetox+i],false)==true && chkmvp1(map[(unsigned int)movetoy+j+height][(unsigned int)movetox+i+width],false)==true)
			{
				good=true;
				if(goingobstaclecounter<10)
				{
					goingobstaclecounter++;
					allobstacles[player][index].push_back(point((float)-i,(float)-j));
				}
				else
					goingobstaclecounter=0;
				movetox+=i;
				movetoy+=j;
				break;
			}
		}
		if(good==true)
			break;
	}
}
void unit::train()
{
	float trainercoefficient=1; //its a coefficient, so I don't want to default it to 0
	if(allbuildings[player][(-garrisoned)-1].trainer!=-1 && allunits[player][allbuildings[player][(-garrisoned)-1].trainer]->veterancylvl>veterancylvl)
		trainercoefficient=(float)(allunits[player][allbuildings[player][(-garrisoned)-1].trainer]->veterancylvl-veterancylvl)/2.0f+1;
	exp+=(1.0f/FPS)*(trainercoefficient*(FPS*10)/(veterancylvl+10));
}
void unit::trainlieut()
{
	if(lieutenant==0)
	{
		if(veterancylvl>=3)
			lieutexp+=1;
		if(allbuildings[player][(-garrisoned)-1].trainer!=-1 && allunits[player][allbuildings[player][(-garrisoned)-1].trainer]->lieutenant>lieutenant)
			lieutexp+=pow(allunits[player][allbuildings[player][(-garrisoned)-1].trainer]->lieutenant+1.0f,.2f);
	}
	else
	{
		float trainercoefficient=1; //its a coefficient, so I don't want to default it to 0
		if(allbuildings[player][(-garrisoned)-1].trainer!=-1 && allunits[player][allbuildings[player][(-garrisoned)-1].trainer]->lieutenant>lieutenant)
			trainercoefficient=(float)(allunits[player][allbuildings[player][(-garrisoned)-1].trainer]->lieutenant-lieutenant)/2.0f+1;
		exp+=(1.0f/FPS)*(trainercoefficient*(FPS*10)/(lieutenant+10));
	}
}
bool unit::chkmvp1(tile &checkwhat, bool checkelev)
{
	if(stayoutoftowerrange==true && userordered==false && checkwhat.whichplayer.get(true)!=players[player]) //if its walking onto the range of an enemy building and its not supposed to
		return false;
	if(checkelev==true && (map[(unsigned int)y][(unsigned int)x].elevation>checkwhat.elevation+1 || checkwhat.elevation>map[(unsigned int)y][(unsigned int)x].elevation+1))
		return false;
	short walkable[6]={0};
	if(whatisit!=3)//if we walk on land
	{
		walkable[0]=TS_GRASS;
		walkable[1]=TS_GOLD;
		walkable[2]=TS_STONE;
		walkable[3]=TS_ROAD;
		walkable[4]=TS_WATERBUILDING;
		walkable[5]=TS_BUSHES;
	}
	else//ship, on water
	{
		walkable[0]=TS_WATER;
		walkable[1]=TS_FISH;
	}
	bool good=false;
	for(int i=0; i<6; i++)//check all the legal squares
	{
		if(checkwhat.tilestyle==walkable[i])
		{
			good=true;//if the square is legal
			break;//stop
		}
	}
	if(checkwhat.tilestyle==TS_GATE && checkwhat.buildingplayer==players[player])
		good=true;
	if(checkwhat.unitplayer==player && allunits[checkwhat.unitplayer][checkwhat.unitindex]->whatisit==3 && (checkwhat.unitonMineon&1)==1)
		return true;
	if(good==true && (checkwhat.unitonMineon&1)==1 && (checkwhat.unitindex!=index || checkwhat.unitplayer!=player))
		good=false;	
	return good;
}
short unit::build(short buildwhat, int buildatx, int buildaty)
{
	short bindex=-1;
	short takeresourcesfrom=-1;
    if(buildingResourcesFrom==-1) //its a building
        takeresourcesfrom=map[(int)y][(int)x].inbuildingrad;
	if(map[(int)y][(int)x].whichplayer.get(false)!=player)
		takeresourcesfrom=-1;
    if(buildingAllowed[player][buildwhat]) //allowed to build this
	{
        if(takeresourcesfrom!=-1 || buildingResourcesFrom>=0) //good building or good unit
        {
            bool good=false;
            if(takeresourcesfrom!=-1 && allbuildings[player][takeresourcesfrom].holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allbuildings[player][takeresourcesfrom].holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allbuildings[player][takeresourcesfrom].holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allbuildings[player][takeresourcesfrom].holding[3]>=allbuildablebuildings[buildwhat].stonetobuild)
            {
                good=true;
                allbuildings[player][takeresourcesfrom].holding[0]-=allbuildablebuildings[buildwhat].foodtobuild;
                allbuildings[player][takeresourcesfrom].holding[1]-=allbuildablebuildings[buildwhat].woodtobuild;
                allbuildings[player][takeresourcesfrom].holding[2]-=allbuildablebuildings[buildwhat].goldtobuild;
                allbuildings[player][takeresourcesfrom].holding[3]-=allbuildablebuildings[buildwhat].stonetobuild;
            }
            else if(buildingResourcesFrom>=0 && allunits[player][buildingResourcesFrom]->holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allunits[player][buildingResourcesFrom]->holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allunits[player][buildingResourcesFrom]->holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allunits[player][buildingResourcesFrom]->holding[3]>=allbuildablebuildings[buildwhat].stonetobuild)
            {
                good=true;
                allunits[player][buildingResourcesFrom]->holding[0]-=allbuildablebuildings[buildwhat].foodtobuild;
                allunits[player][buildingResourcesFrom]->holding[1]-=allbuildablebuildings[buildwhat].woodtobuild;
                allunits[player][buildingResourcesFrom]->holding[2]-=allbuildablebuildings[buildwhat].goldtobuild;
                allunits[player][buildingResourcesFrom]->holding[3]-=allbuildablebuildings[buildwhat].stonetobuild;
            }
            if(good==true)
            {
                resources[player][0]-=allbuildablebuildings[buildwhat].foodtobuild;
                resources[player][1]-=allbuildablebuildings[buildwhat].woodtobuild;
                resources[player][2]-=allbuildablebuildings[buildwhat].goldtobuild;
                resources[player][3]-=allbuildablebuildings[buildwhat].stonetobuild;
                if(overwritebuildings[player].size()>0)
                {
                    bindex=overwritebuildings[player][overwritebuildings[player].size()-1];
                    overwritebuildings[player].pop_back();
                    allbuildings[player][overwritebuildings[player][overwritebuildings[player].size()-1]]=building(allbuildablebuildings[buildwhat], (float)buildatx, (float)buildaty, player, overwritebuildings[player][overwritebuildings[player].size()-1], (float)buildaty+allbuildablebuildings[buildwhat].height, (float)buildaty, (float)buildatx, (float)buildatx+allbuildablebuildings[buildwhat].width,0,0,0,0,allbuildablebuildings[buildwhat].timetobuild); //CHANGE y+1 and the like to something from GUI, so that you can specify where you want it
                }
                else
                {
                    bindex=allbuildings[player].size();
                    allbuildings[player].push_back(building(allbuildablebuildings[buildwhat], (float)buildatx, (float)buildaty, player, allbuildings[player].size(), (float)buildaty+allbuildablebuildings[buildwhat].height, (float)buildaty, (float)buildatx, (float)buildatx+allbuildablebuildings[buildwhat].width,0,0,0,0, allbuildablebuildings[buildwhat].timetobuild)); 	
                    creationqueueunits[0].resize(creationqueueunits[0].size()+1);
                }
                int ts=TS_BUILDING;
                if(allbuildings[player][bindex].landorwater==false) //water
                    ts=TS_WATERBUILDING;
                for(float k=allbuildings[player][bindex].y; k<allbuildings[player][bindex].y+allbuildings[player][bindex].height; k+=.25)
                {
                    for(float h=allbuildings[player][bindex].x; h<allbuildings[player][bindex].x+allbuildings[player][bindex].width; h+=.25)
                    {
                        if(allbuildings[player][bindex].id==23 && (map[(int)k][(int)h].unitonMineon>>3)==0) //mine (i.e. digging) //IF unitonMineon is EVER REUSED, UPDATE THIS
                        {
                            map[(int)k][(int)h].unitonMineon|=4; //Say a mine is being built
                            map[(int)k][(int)h].unitonMineon|=(map[(int)k][(int)h].tilestyle<<3);
                        }
                        map[(int)k][(int)h].tilestyle=ts;
                        map[(int)k][(int)h].buildingplayer=player;
                        map[(int)k][(int)h].buildingindex=bindex;
                        map[(int)k][(int)h].whichplayer.set(false, (unsigned char)player);
                        map[(int)k][(int)h].whichplayer.set(true, players[player]);
                    }
                }
                return bindex;
            }
            /*else if(player==0)
            {
                currErr=allErr[FEWRESOURCES];
                redraw=1;
            }*/
            else //not enough resources at current building
            {
                point p=searchbuildingresources(buildwhat);
                if(p.x==-1.0f && p.y==-1.0f)
                {
                    if(player==0)
                    {
                        currErr=allErr[NORESOURCELOC];
                        redraw=1;
                    }
                    return -1;
                }
                else //good loc
                {
                    movetox=p.x;
                    movetoy=p.y;
                    unitbuildingwhat=buildwhat;
                    buildingx=buildatx;
                    buildingy=buildaty;
                    return -1;
                }
            }    
        }
        /*else if(player==0)
        {
            currErr=allErr[NORESOURCELOC];
            redraw=1;
        }*/
        else
        {
            point p=searchbuildingresources(buildwhat);
            if(p.x==-1.0f && p.y==-1.0f)
            {
                if(player==0)
                {
                    currErr=allErr[NORESOURCELOC];
                    redraw=1;
                }
                return -1;
            }
            else //good loc
            {
                movetox=p.x;
                movetoy=p.y;
                unitbuildingwhat=buildwhat;
                buildingx=buildatx;
                buildingy=buildaty;
                return -1;
            }
        }
	}
    else if(player==0)
    {
        currErr=allErr[EARLYBUILD];
        redraw=1;
    }
	return -1;
}
void unit::revealmapcreation()
{
    short uselos=los;
    for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
    {
        if(allResearches[alreadyResearched[player][i]].checkResearch(id))
            uselos+=allResearches[alreadyResearched[player][i]].los;
    }
	for(float i=-uselos; i<uselos; i++)
	{
		for(float j=-uselos; j<uselos; j++)
		{
			if(i+y<map.size() && i+y>=0 && x+j<map.size() && j+x>=0)
			{
				if(sqrt(pow(i,2)+pow(j,2))<uselos)
				{
                    if(generals[player]==index) //this is the general, he can see
                        minimapseen[player][(unsigned int)(i+y)][(unsigned int)(j+x)]=true;
                    else if(selfRecording) //this unit is recording stuff
                    {
                        bool good=true;
                        for(unsigned int k=0; k<rep.seenTiles.size(); k++)
                        {
                            if((int)(i+y)==rep.seenTiles[k].y && (int)(j+x)==rep.seenTiles[k].x) //repeat
                            {
                                good=false;
                                break;
                            }
                        }
                        if(good)
                            rep.seenTiles.push_back(point((int)(j+x), (int)(i+y)));
                    }
				}
			}
		}
	}
}
pointex unit::getcandidate(int x, int y, double dist)
{
	if(!(checkdimensions((float)x,(float)y)))
		return pointex(-1,-1);
	tile temp(map[y][x]);
    short addLOS=0;
    for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
    {
        if(allResearches[alreadyResearched[player][i]].checkResearch(id))
            addLOS+=allResearches[alreadyResearched[player][i]].los;
    }
	if((temp.unitonMineon&1)==1)//if the space in question has a unit
	{
		if(players[temp.unitplayer]!=players[player]) //if this object is not allied to the player whose unit this is
		{
			if(dist<=los+addLOS-allunits[temp.unitplayer][temp.unitindex]->camouflage-tilecameo[temp.tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
				return pointex(x,y, dist, true,temp.unitplayer,temp.unitindex); //record its position and that its a unit(true)
		}
	}
	else if(temp.tilestyle==TS_BUILDING || temp.tilestyle==TS_WATERBUILDING)//if the space in question has a building
	{
		if(players[temp.buildingplayer]!=players[player]) //if this object is not allied to the player whose building this is
		{
			if(dist<=los+addLOS-tilecameo[temp.tilestyle]) //if we can actually see the enemy building, after camouflage from it and the map
				return pointex(x,y, dist, false,temp.buildingplayer,temp.buildingindex); //record its position and that its a building(false)
		}
	}
	return pointex(-1, -1);
}
bool unit::pointonlinesegment(double x, double movetox, double newx) //checks whether a point on a line is on a certain line segment that is part of said line
{
	if(movetox>x) //if we're going to an x(or y) greater than this one
	{
		if(newx>=x && newx<=movetox) //are we bigger than orig and smaller than dest.
			return true; //all good
	}
	else //else, going to x smaller than this one
	{
		if(newx<=x && newx>=movetox) //are we smaller than orig and bigger than dest.
			return true;//all good
	}
	return false;//not in between
}
//void unit::revealmapmvmt(double newx, double newy, double actspeed) //this reveals the new land that the player can see on the mini map and actual map
//{
//	double intersectx=(x+((.5*actspeed)*(newx-x)/actspeed))+(sqrt((los*los)+((.5*actspeed)*(.5*actspeed)))*(newy-y)/actspeed); //the intersection of the two circles
//	double intersecty=(y+((.5*actspeed)*(newy-y)/actspeed))+(sqrt((los*los)+((.5*actspeed)*(.5*actspeed)))*(newx-x)/actspeed);

//	double loopuptox=x+(los*(newx-x)/actspeed);
//	double loopuptoy=y+(los*(newx-x)/actspeed);
////	double loopuptoy=y+(los*(newy-y)/actspeed);

////	double slopeofloop=(loopuptoy-intersecty)/(loopuptox-intersectx);
//	double slopeofloop=(loopuptox-intersectx)/(loopuptoy-intersecty);
//	double bofloop=intersecty-(slopeofloop*intersectx);

////	double slopeofshift=(newy-y)/(newx-x);//WHICH ONE IS RIGHT?????
//	double slopeofshift=(newx-x)/(newy-y);
//	double bofshift=y-(slopeofshift*x);

//	for(int i=(int)intersecty+1; i>(x+((.5*actspeed)*(newx-x)/actspeed)); i--) //the y coordinate I'm checking
//	{
//		for(int j=(int)((i-bofloop)/slopeofloop); ; j++) //the x coordinate I'm checking
//		{
//			if(sqrt(pow((j-newx), 2)+pow((i-newy), 2))>=los)
//				break;
//			if(j<0)
//				continue;
//			if(i<0)
//				break;
//			minimapseen[player][i][j]=true;
//			double d=(j + ((i - bofshift)*slopeofshift))/(1 + pow(slopeofshift,2));
//			double nj=2*d-j;
//			double ni=(2*d*slopeofshift)-i+(2*bofshift);
//			if(ni>=0 && nj>=0)
//				minimapseen[player][(unsigned int)ni][(unsigned int)nj]=true;
//		}	
//	}
//}
void unit::gather(char gatheringwhat) // 0=food, 1=wood, 2=gold, 3=stone
{
	if(map[(unsigned int)gatheringy][(unsigned int)gatheringx].resources[(unsigned int)gatheringwhat]>0)
	{
        short addspeed=0;
        for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
        {
            if(allResearches[alreadyResearched[player][i]].checkResearch(id))
                addspeed+=allResearches[alreadyResearched[player][i]].gather[(unsigned int)gatheringwhat];
        }
		if(frames%(GATHERING_RATE-addspeed)==0)//time to gather
		{
			unsigned char sum=0;
			for(int i=0; i<4; i++)
				sum+=holding[i];
			if(sum<maxhold)
			{
				map[(unsigned int)gatheringy][(unsigned int)gatheringx].resources[(unsigned int)gatheringwhat]--;
				if(map[(unsigned int)gatheringy][(unsigned int)gatheringx].resources[(unsigned int)gatheringwhat]==0) //no more resources on that tile
				{
					int ts=map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle;
					if(gatheringwhat==0 && ts==TS_BERRIES)
						map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle=TS_BUSHES;
					else if(gatheringwhat==1)
						map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle=TS_GRASS;
					//now, search 7 by 7 square and pick nearest
					searchresources();
				}
				holding[(unsigned int)gatheringwhat]++;
				if(selected==true && player==0) //this unit is selected, so how much stuff it is carrying needs to be displayed
					redraw=true;
				sum++;
			}
			if(sum>=maxhold)//go to nearest storage building (basically else)
			{
				int sum=0;
				if(nearesthold!=-1)
				{
					for(int i=0; i<4; i++)
						sum+=allbuildings[player][nearesthold].holding[i];
				}
				if(nearesthold!=-1 && allbuildings[player][nearesthold].health>0 && sum<allbuildings[player][nearesthold].maxhold)
				{
					movetox=allbuildings[player][nearesthold].x-1;
					movetoy=allbuildings[player][nearesthold].y;
				}
				else //no building to deliver to currently exists
				{
					bool good=false;
					for(int i=3; i<=60; i++)//searches in ever-increasing squares for a building to deliver supplies to. squares up to 21 tile side length
					{
						for(int j=i/-2; j<=i/2; j++)
						{
							if(x+j>=0 && x+j<MAPSIZE && (unsigned int)(y-(i/2))>=0 && (unsigned int)(y-(i/2))<MAPSIZE)
							{
								if(map[(unsigned int)(y-(i/2))][(unsigned int)(x+j)].tilestyle==TS_BUILDING && map[(unsigned int)(y-(i/2))][(unsigned int)(x+j)].buildingplayer==player && allbuildings[map[(unsigned int)(y-(i/2))][(unsigned int)(x+j)].buildingplayer][map[(unsigned int)(y-(i/2))][(unsigned int)x+j].buildingindex].beingbuilt<=0) //searches above the unit
								{
									int sum=0;
									for(int k=0; k<4; k++)
										sum+=allbuildings[map[(unsigned int)(y-(i/2))][(unsigned int)(x+j)].buildingplayer][map[(unsigned int)(y-(i/2))][(unsigned int)x+j].buildingindex].holding[k];
									if(sum<allbuildings[map[(unsigned int)(y-(i/2))][(unsigned int)x+j].buildingplayer][map[(unsigned int)(y-(i/2))][(unsigned int)x+j].buildingindex].maxhold)
									{
										nearesthold=map[(unsigned int)(y-(i/2))][(unsigned int)x+j].buildingindex;
										good=true;
										break;
									}
								}
							}
							if(x+j>=0 && x+j<MAPSIZE && (unsigned int)(y+(i/2))>=0 && (unsigned int)(y+(i/2))<MAPSIZE)
							{
								if(map[(unsigned int)(y+(i/2))][(unsigned int)x+j].tilestyle==TS_BUILDING && map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingplayer==player && allbuildings[map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingplayer][map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingindex].beingbuilt<=0) //below the unit
								{
									int sum=0;
									for(int k=0; k<4; k++)
										sum+=allbuildings[map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingplayer][map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingindex].holding[k];
									if(sum<allbuildings[map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingplayer][map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingindex].maxhold)
									{
										nearesthold=map[(unsigned int)(y+(i/2))][(unsigned int)x+j].buildingindex;
										good=true;
										break;
									}
								}
							}
							if(y+j>=0 && y+j<MAPSIZE && (unsigned int)(x-(i/2))>=0 && (unsigned int)(x-(i/2))<MAPSIZE)
							{
								if(map[(unsigned int)y+j][(unsigned int)(x-(i/2))].tilestyle==TS_BUILDING && map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingplayer==player && allbuildings[map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingplayer][map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingindex].beingbuilt<=0) //left of the unit
								{
									int sum=0;
									for(int k=0; k<4; k++)
										sum+=allbuildings[map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingplayer][map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingindex].holding[k];
									if(sum<allbuildings[map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingplayer][map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingindex].maxhold)
									{
										nearesthold=map[(unsigned int)y+j][(unsigned int)(x-(i/2))].buildingindex;
										good=true;
										break;
									}
								}
							}
							if(y+j>=0 && y+j<MAPSIZE && (unsigned int)(x+(i/2))>=0 && (unsigned int)(x+(i/2))<MAPSIZE)
							{
								if(map[(unsigned int)y+j][(unsigned int)(x+(i/2))].tilestyle==TS_BUILDING && map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingplayer==player && allbuildings[map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingplayer][map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingindex].beingbuilt<=0) //right of the unit
								{
									int sum=0;
									for(int k=0; k<4; k++)
										sum+=allbuildings[map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingplayer][map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingindex].holding[k];
									if(sum<allbuildings[map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingplayer][map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingindex].maxhold)
									{
										nearesthold=map[(unsigned int)y+j][(unsigned int)(x+(i/2))].buildingindex;
										good=true;
										break;
									}
								}
							}
						}
						if(good==true)
							break;
					}
					if(nearesthold!=-1) //in the else, check again
					{
						for(int i=0; i<4; i++)
							sum+=allbuildings[player][nearesthold].holding[i];
					}
					if(nearesthold!=-1 && allbuildings[player][nearesthold].health>0 && sum<allbuildings[player][nearesthold].maxhold)
					{						
						movetox=allbuildings[player][nearesthold].x-1;
						movetoy=allbuildings[player][nearesthold].y;
					}
				}
			}
		}
	}
	else
		searchresources();
}
void unit::movement(bool siegesent) //the first time I put a class function outside of the class definition, this happened because it needed the definition of both unit and siege to function
{
	if(checknomove(siegesent)==false) //already done
		return;
	if(whatisit!=3 && map[(int)movetoy][(int)movetox].unitplayer==player && allunits[player][map[(int)movetoy][(int)movetox].unitindex]->whatisit==3)
    {
        short addgarrison=0;
        for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
        {
            if(allResearches[alreadyResearched[player][i]].checkResearch(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->id))
                addgarrison+=allResearches[alreadyResearched[player][i]].armor;
        }
        if((int)allunits[player][map[(int)movetoy][(int)movetox].unitindex]->unitsinside.size()<allunits[player][map[(int)movetoy][(int)movetox].unitindex]->maxgarrison+addgarrison)//if garrisonning ship, do so
        {
            bool good=false;
            for(int i=0; i<allunits[player][map[(int)movetoy][(int)movetox].unitindex]->width; i++)
            {
                if(sqrt(pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->x+i-x,2)+pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->y-y,2))<1.414f)
                {
                    x=movetox;
                    y=movetoy;
                    allunits[player][map[(int)movetoy][(int)movetox].unitindex]->unitsinside.push_back(index);
                    good=true;
                    break;
                }
                if(sqrt(pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->x+i-x,2)+pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->y+allunits[player][map[(int)movetoy][(int)movetox].unitindex]->height-y,2))<1.414f)
                {
                    x=movetox;
                    y=movetoy;
                    allunits[player][map[(int)movetoy][(int)movetox].unitindex]->unitsinside.push_back(index);
                    good=true;
                    break;
                }
            }
            if(good==false)
            {
                for(int j=0; j<allunits[player][map[(int)movetoy][(int)movetox].unitindex]->height; j++)
                {
                    if(sqrt(pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->x-x,2)+pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->y+j-y,2))<1.414f)
                    {
                        x=movetox;
                        y=movetoy;
                        allunits[player][map[(int)movetoy][(int)movetox].unitindex]->unitsinside.push_back(index);
                        good=true;
                        break;
                    }
                    if(sqrt(pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->x+allunits[player][map[(int)movetoy][(int)movetox].unitindex]->width-x,2)+pow(allunits[player][map[(int)movetoy][(int)movetox].unitindex]->y+j-y,2))<1.414f)
                    {
                        x=movetox;
                        y=movetoy;
                        allunits[player][map[(int)movetoy][(int)movetox].unitindex]->unitsinside.push_back(index);
                        good=true;
                        break;
                    }
                }
            }
            if(good==true)
                return;
        }
	}
	
	if(transferring==1 && (pow(x-allbuildings[player][transferfrom].x,2)+pow(y-allbuildings[player][transferfrom].y,2))<pow(allbuildings[player][transferfrom].radiustodistribute,2)) //transferring and within transfer radius of building to transfer from
	{
		int sum=0;
		int tosend=0;
		for(int i=0; i<4; i++)
		{
			sum+=holding[i];
			tosend+=allbuildings[player][transferfrom].transfer[i];
		}
		if(tosend==0) //Done!
		{
			movetox=x;
			movetoy=y;
			allobstacles[player][index].clear();
			transferring=0;
		}
		else
		{
			bool somethingsent=false;
			for(int i=0; i<4; i++)
			{
				if(allbuildings[player][transferfrom].transfer[i]>0 && allbuildings[player][transferfrom].holding[i]>=0) //Need to transfer this resource and have this resource
				{
					if(allbuildings[player][transferfrom].holding[i]>=maxhold-sum) //enough of this resource to fill units holding
					{
						if(allbuildings[player][transferfrom].transfer[i]>=maxhold-sum) //need to transfer more than can hold, transfer as much as possible
						{
							allbuildings[player][transferfrom].holding[i]-=(maxhold-sum);
							allbuildings[player][transferfrom].transfer[i]-=(maxhold-sum);
							holding[i]+=(maxhold-sum);
							transferring=2;
							movetox=allbuildings[player][transferto].x;
							movetoy=allbuildings[player][transferto].y;
							allobstacles[player][index].clear();
							somethingsent=false; //no need
							if(player==0)
								redraw=1;
							break;
						}
						else //need to transfer less than can hold
						{
							allbuildings[player][transferfrom].holding[i]-=allbuildings[player][transferfrom].transfer[i];
							holding[i]+=allbuildings[player][transferfrom].transfer[i];
							sum+=allbuildings[player][transferfrom].transfer[i];
							somethingsent=true;
							allbuildings[player][transferfrom].transfer[i]=0;
							if(player==0)
								redraw=1;
						}
					}
					else //not enough, fill as much as I can and empty the building
					{
						if(allbuildings[player][transferfrom].transfer[i]>=allbuildings[player][transferfrom].holding[i]) //need to transfer more than I have
						{
							holding[i]+=allbuildings[player][transferfrom].holding[i];
							sum+=allbuildings[player][transferfrom].holding[i];
							allbuildings[player][transferfrom].transfer[i]-=allbuildings[player][transferfrom].holding[i];
							allbuildings[player][transferfrom].holding[i]=0;
							somethingsent=true;
							if(player==0)
								redraw=1;
						}
						else
						{
							allbuildings[player][transferfrom].holding[i]-=allbuildings[player][transferfrom].transfer[i];
							holding[i]+=allbuildings[player][transferfrom].transfer[i];
							sum+=allbuildings[player][transferfrom].transfer[i];
							somethingsent=true;
							allbuildings[player][transferfrom].transfer[i]=0;
							if(player==0)	
								redraw=1;
						}
					}
				}
			}
			if(somethingsent)
			{
				transferring=2;
				movetox=allbuildings[player][transferto].x;
				movetoy=allbuildings[player][transferto].y;
				allobstacles[player][index].clear();
			}
		}
	}
	if(transferring==2 && (pow(x-allbuildings[player][transferto].x,2)+pow(y-allbuildings[player][transferto].y,2))<pow(allbuildings[player][transferto].radiustodistribute,2)) //transferring and within transfer radius of building to transfer from
	{
		bool success=true;
		int sum=0;
		for(int i=0; i<4; i++)
			sum+=allbuildings[player][transferto].holding[i];
		for(int i=0; i<4; i++)
		{
			if(holding[i]+sum<allbuildings[player][transferto].maxhold)
			{
				allbuildings[player][transferto].holding[i]+=holding[i];
				sum+=holding[i];
				holding[i]=0;
				if(player==0)
					redraw=1;
			}
			else
			{
				if(allbuildings[player][transferto].maxhold-sum>0)
				{
					holding[i]-=(allbuildings[player][transferto].maxhold-sum);
					allbuildings[player][transferto].holding[i]+=(allbuildings[player][transferto].maxhold-sum);
					if(player==0)
						redraw=1;
				}
				success=false; //failed
				break; //failed
			}
		}
		if(success)
		{
			transferring=1; //turn around
			movetox=allbuildings[player][transferfrom].x;
			movetoy=allbuildings[player][transferfrom].y;
			allobstacles[player][index].clear();
		} //else keep trying to deposit
	}
	
	float actspeed=speed; //actual speed
	if((int)(movetox+0.00001f)!=(int)(movetox))
		movetox+=0.00001f;
	if((int)(movetoy+0.00001f)!=(int)(movetoy))
		movetoy+=0.00001f;
	if((int)(y+0.00001f)!=(int)(y))
		y+=0.00001f;
	if((int)(x+0.00001f)!=(int)(x))
		x+=0.00001f;
	if(whatisit==2)//siege
	{//calculates actual speed for siege unit
		float totspeed=0;
		for(unsigned int i=0; i<allsiegeunits[player][siegeindex].manning.size(); i++)
			totspeed+=allunits[player][allsiegeunits[player][siegeindex].manning[i]]->speed;
		actspeed=speed*sqrt(totspeed);
	}
	double decreaseby=0.0096*(100*health/allbuildableunits[id].health)*(100*health/allbuildableunits[id].health)-2.2609*(100*health/allbuildableunits[id].health)+128.43; //decrease speed for low health
	if(decreaseby>=100)
	{
		movetox=x;
		movetoy=y;
		return;
	}
	if(decreaseby<=0)
		decreaseby=0;
	actspeed*=float(1-(.01*decreaseby)); //decreased speed for low health
	actspeed+=tilespeed[map[(unsigned int)y][(unsigned int)x].tilestyle-1]; //adjust speed for what tile the unit is standing on
	if(sqrt(pow((movetox-x), 2)+pow((movetoy-y), 2))>actspeed+0.05) //start moving
	{
		if(abs((double)(movetox-x))>=0.0001)//if the line isn't vert
		{
			float slope=(movetoy-y)/(movetox-x); //slope
			float b=y-(slope*x); //y-intercept
			float newx1=(sqrt(-(b*b)+(2*b*y)-(2*b*slope*x)-(y*y)+(2*y*slope*x)+((actspeed*actspeed)*(slope*slope))+(actspeed*actspeed)-((slope*slope)*(x*x)))-(b*slope)+(y*slope)+x)/((slope*slope)+1);
			float newx2=(-sqrt(-(b*b)+(2*b*y)-(2*b*slope*x)-(y*y)+(2*y*slope*x)+((actspeed*actspeed)*(slope*slope))+(actspeed*actspeed)-((slope*slope)*(x*x)))-(b*slope)+(y*slope)+x)/((slope*slope)+1);
			float newy1=(slope*newx1)+b; //we got the x-coordinate above, heres the y coordinate
			float newy2=(slope*newx2)+b; //2 x's, 2 y's
			int conditionsmet=0; //was the point both between the start and finish in x and in y?
			bool firstoneisgood=false; //is the first one correct
			conditionsmet+=pointonlinesegment(x, movetox, newx1); //check x
			conditionsmet+=pointonlinesegment(y, movetoy, newy1); //check y
			if(conditionsmet==2)//if both correct
				firstoneisgood=true;//say so
			conditionsmet=0;//for x2 and y2 results
			conditionsmet+=pointonlinesegment(x, movetox, newx2);
			conditionsmet+=pointonlinesegment(y, movetoy, newy2);
			if(conditionsmet==2 && firstoneisgood==true && newy1!=newy2) //if both are good
				printf("Major Error. Search -94");
				//exit(-94);
			else if(conditionsmet!=2 && firstoneisgood==false) //if neither is good
				printf("Major Error. Search -24");
				//exit(-24);
			if(firstoneisgood==true) // return the proper one
			{
				short ret=checkmove(point(newx1, newy1),actspeed);
				if(ret==1)//all clear
				{
					//revealmapmvmt(newx1, newy1, actspeed);
					for(float k=y; k<y+height; k+=.25) //leave old tile
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon&=254; //this sets the rightmost bit to 0 while leaving the rest the same
							map[(int)k][(int)h].unitindex=0;
							map[(int)k][(int)h].unitplayer=0;
						}
					}
					x=newx1;
					y=newy1;
					revealmapcreation();
					for(float k=y; k<y+height; k+=.25) //step on new tile
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon|=1;
							map[(int)k][(int)h].unitindex=index;
							map[(int)k][(int)h].unitplayer=player;
						}
					}
					boundingbox.top=y;
					boundingbox.left=x;
					boundingbox.right=x+width;
					boundingbox.bottom=y+height;
					if(whatisit==2) //siege unit
					{
						for(unsigned int i=0; i<allsiegeunits[player][siegeindex].manning.size(); i++)//run the movement() of all manning units
							allunits[player][allsiegeunits[player][siegeindex].manning[i]]->movement(true);
					}
					else if(whatisit==3)
					{ //make sure all of the units in the ship stay in the ship, rather than 'fall out' and stand on the water behind it as it moves
						for(unsigned int i=0; i<allunits[player][index]->unitsinside.size(); i++)
						{
							/*for(float k=allunits[player][allunits[player][index]->unitsinside[i]]->y; k<allunits[player][allunits[player][index]->unitsinside[i]]->y+allunits[player][allunits[player][index]->unitsinside[i]]->height; k+=.25)
							{
								for(float h=allunits[player][allunits[player][index]->unitsinside[i]]->x; h<allunits[player][allunits[player][index]->unitsinside[i]]->x+allunits[player][allunits[player][index]->unitsinside[i]]->width; h+=.25)
								{
									map[(int)k][(int)h].uniton=false;
									map[(int)k][(int)h].index=0;
									map[(int)k][(int)h].player=0;
								}
							}*/
							allunits[player][allunits[player][index]->unitsinside[i]]->x=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->y=y;
							//for(float k=y; k<y+allunits[player][allunits[player][index]->unitsinside[i]]->height; k+=.25)
							//{
							//	for(float h=x; h<x+allunits[player][allunits[player][index]->unitsinside[i]]->width; h+=.25)
							//	{
							//		map[(int)k][(int)h].uniton=true;
							//		map[(int)k][(int)h].index=i;
							//		map[(int)k][(int)h].player=player;
							//	}
							//}
							//allunits[player][allunits[player][index]->unitsinside[i]]->boundingbox.top=allunits[player][allunits[player][index]->unitsinside[i]]->y;
							//allunits[player][allunits[player][index]->unitsinside[i]]->boundingbox.left=allunits[player][allunits[player][index]->unitsinside[i]]->x;
							//allunits[player][allunits[player][index]->unitsinside[i]]->boundingbox.right=allunits[player][allunits[player][index]->unitsinside[i]]->x+allunits[player][allunits[player][index]->unitsinside[i]]->width;
							//allunits[player][allunits[player][index]->unitsinside[i]]->boundingbox.bottom=allunits[player][allunits[player][index]->unitsinside[i]]->y+allunits[player][allunits[player][index]->unitsinside[i]]->height;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetox=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetoy=y;
						}
					}
					if(selected==true && sqrt(pow(allunits[player][generals[player]]->x-x,2)+pow(allunits[player][generals[player]]->y-y,2))>allunits[player][generals[player]]->los)
					{
						selected=false;
						for(unsigned int i=0; i<selectedunits[player].size(); i++)
						{
							if(selectedunits[player][i]==index)
							{
								selectedunits[player].erase(selectedunits[player].begin()+i);
								break;
							}
						}
						redraw=1;
					}
					return;
				}
				else if(ret==2)
				{
					userordered=false;
					movement(); //try moving with my new movetox/movetoy
					return;
				}
				else if(ret==0)
				{
					if(allobstacles[player][index].size()<=0)
					{
						//movetox=x;
						//movetoy=y;
						return;
					}
					//actualtomovex=movetox;//go to the square specified by allobstacles, found in threadgoaroundobstacle()
					//actualtomovey=movetoy;
					movetox=x+allobstacles[player][index][0].x;
					movetoy=y+allobstacles[player][index][0].y;
					allobstacles[player][index].erase(allobstacles[player][index].begin());
					userordered=false;
					movement();
					return;
				}
			}
			else
			{
				short ret=checkmove(point(newx2, newy2),actspeed);
				if(ret==1)//all clear
				{
					//revealmapmvmt(newx2, newy2, actspeed);
					for(float k=y; k<y+height; k+=.25) //leave old spot
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon&=254;
							map[(int)k][(int)h].unitindex=0;
							map[(int)k][(int)h].unitplayer=0;
						}
					}
					x=newx2;
					y=newy2;
					revealmapcreation();
					for(float k=y; k<y+height; k+=.25) //walk onto new spot
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon|=1;
							map[(int)k][(int)h].unitindex=index;
							map[(int)k][(int)h].unitplayer=player;
						}
					}
					boundingbox.top=y;
					boundingbox.left=x;
					boundingbox.right=x+width;
					boundingbox.bottom=y+height;
					if(whatisit==2) //siege unit
					{
						for(unsigned int i=0; i<allsiegeunits[player][siegeindex].manning.size(); i++)//run the movement() of all manning units
							allunits[player][allsiegeunits[player][siegeindex].manning[i]]->movement(true);
					}
					else if(whatisit==3)
					{ //make sure all of the units in the ship stay in the ship, rather than 'fall out' and stand on the water behind it as it moves
						for(unsigned int i=0; i<allunits[player][index]->unitsinside.size(); i++)
						{
							allunits[player][allunits[player][index]->unitsinside[i]]->x=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->y=y;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetox=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetoy=y;
						}
					}
					if(selected==true && sqrt(pow(allunits[player][generals[player]]->x-x,2)+pow(allunits[player][generals[player]]->y-y,2))>allunits[player][generals[player]]->los)
					{
						selected=false;
						for(unsigned int i=0; i<selectedunits[player].size(); i++)
						{
							if(selectedunits[player][i]==index)
							{
								selectedunits[player].erase(selectedunits[player].begin()+i);
								break;
							}
						}
						redraw=1;
					}
					return;
				}
				else if(ret==2)
				{
					userordered=false;
					movement(); //try moving with my new movetox/movetoy
					return;
				}
				else if(ret==0)
				{
					if(allobstacles[player][index].size()<=0)
					{
						//movetox=x;
						//movetoy=y;
						return;
					}
					//actualtomovex=movetox;
					//actualtomovey=movetoy;
					movetox=x+allobstacles[player][index][0].x;
					movetoy=y+allobstacles[player][index][0].y;
					allobstacles[player][index].erase(allobstacles[player][index].begin());
					userordered=false;
					movement();
					return;
				}
			}
		}
		else
		{
			if(pointonlinesegment(y, movetoy, y+actspeed)==true)//if the line is vert. return either the point above it or the one below it
			{
				short ret=checkmove(point(x,y+actspeed),actspeed);
				if(ret==1)
				{
					//revealmapmvmt(x, y+speed, actspeed);
					for(float k=y; k<y+height; k+=.25) //leave old tile
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon&=254;
							map[(int)k][(int)h].unitindex=0;
							map[(int)k][(int)h].unitplayer=0;
						}
					}
					y+=actspeed;
					revealmapcreation();
					for(float k=y; k<y+height; k+=.25) //walk onto new tile
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon|=1;
							map[(int)k][(int)h].unitindex=index;
							map[(int)k][(int)h].unitplayer=player;
						}
					}
					boundingbox.top=y;
					boundingbox.left=x;
					boundingbox.right=x+width;
					boundingbox.bottom=y+height;
					if(whatisit==2) //siege unit
					{
						for(unsigned int i=0; i<allsiegeunits[player][siegeindex].manning.size(); i++)//run the movement() of all manning units
							allunits[player][allsiegeunits[player][siegeindex].manning[i]]->movement(true);
					}
					else if(whatisit==3)
					{ //make sure all of the units in the ship stay in the ship, rather than 'fall out' and stand on the water behind it as it moves
						for(unsigned int i=0; i<allunits[player][index]->unitsinside.size(); i++)
						{
							allunits[player][allunits[player][index]->unitsinside[i]]->x=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->y=y;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetox=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetoy=y;
						}
					}
					if(selected==true && sqrt(pow(allunits[player][generals[player]]->x-x,2)+pow(allunits[player][generals[player]]->y-y,2))>allunits[player][generals[player]]->los)
					{
						selected=false;
						for(unsigned int i=0; i<selectedunits[player].size(); i++)
						{
							if(selectedunits[player][i]==index)
							{
								selectedunits[player].erase(selectedunits[player].begin()+i);
								break;
							}
						}
						redraw=1;
					}
					return;
				}
				else if(ret==2)
				{
					userordered=false;
					movement(); //try moving with my new movetox/movetoy
					return;
				}
				else if(ret==0)
				{
					if(allobstacles[player][index].size()==0)
					{
						//movetox=x;
						//movetoy=y;
						return;
					}
				//	actualtomovex=movetox;
				//	actualtomovey=movetoy;
					movetox=x+allobstacles[player][index][0].x;
					movetoy=y+allobstacles[player][index][0].y;
					allobstacles[player][index].erase(allobstacles[player][index].begin());
					userordered=false;
					movement();
					return;
				}
			}
			else
			{
				short ret=checkmove(point(x,y-actspeed),actspeed);
				if(/*(int)(y-speed)==(int)y || */ret==1)
				{
					//revealmapmvmt(x, y-speed, actspeed);
					for(float k=y; k<y+height; k+=.25) //leave old tile
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon&=254;
							map[(int)k][(int)h].unitindex=0;
							map[(int)k][(int)h].unitplayer=0;
						}
					}
					y-=actspeed;
					revealmapcreation();
					for(float k=y; k<y+height; k+=.25)
					{
						for(float h=x; h<x+width; h+=.25)
						{
							map[(int)k][(int)h].unitonMineon|=1;
							map[(int)k][(int)h].unitindex=index;
							map[(int)k][(int)h].unitplayer=player;
						}
					}
					boundingbox.top=y;
					boundingbox.left=x;
					boundingbox.right=x+width;
					boundingbox.bottom=y+height;
					if(whatisit==2) //siege unit
					{
						for(unsigned int i=0; i<allsiegeunits[player][siegeindex].manning.size(); i++)//run the movement() of all manning units
							allunits[player][allsiegeunits[player][siegeindex].manning[i]]->movement(true);
					}
					else if(whatisit==3)
					{ //make sure all of the units in the ship stay in the ship, rather than 'fall out' and stand on the water behind it as it moves
						for(unsigned int i=0; i<allunits[player][index]->unitsinside.size(); i++)
						{
							allunits[player][allunits[player][index]->unitsinside[i]]->x=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->y=y;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetox=x;
							allunits[player][allunits[player][index]->unitsinside[i]]->movetoy=y;
						}
					}
					if(selected==true && sqrt(pow(allunits[player][generals[player]]->x-x,2)+pow(allunits[player][generals[player]]->y-y,2))>allunits[player][generals[player]]->los)
					{
						selected=false;
						for(unsigned int i=0; i<selectedunits[player].size(); i++)
						{
							if(selectedunits[player][i]==index)
							{
								selectedunits[player].erase(selectedunits[player].begin()+i);
								break;
							}
						}
						redraw=1;
					}
					return;
				}
				else if(ret==2)
				{
					userordered=false;
					movement(); //try moving with my new movetox/movetoy
					return;
				}
				else if(ret==0)
				{
					if(allobstacles[player][index].size()<=0)
					{
						//movetox=x;
						//movetoy=y;
						return;
					}
					//actualtomovex=movetox;
					//actualtomovey=movetoy;
					movetox=x+allobstacles[player][index][0].x;
					movetoy=y+allobstacles[player][index][0].y;
					allobstacles[player][index].erase(allobstacles[player][index].begin());
					userordered=false;
					movement();
					return;
				}
			}
		}
	}
	else //going to square that is less than speed away from me
	{
		if(chkmvp1(map[(int)movetoy][(int)movetox])==true && chkmvp1(map[(int)movetoy][(int)(movetox+width-.001f)])==true && chkmvp1(map[(int)(movetoy+height-.001f)][(int)movetox])==true && chkmvp1(map[(int)(movetoy+height-.001f)][(int)(movetox+width-.001f)])==true)
		{
			//revealmapmvmt(movetox, movetoy, sqrt(pow((movetox-x), 2)+pow((movetoy-y), 2)));
			for(float k=y; k<y+height; k+=.25)
			{
				for(float h=x; h<x+width; h+=.25)
				{
					map[(int)k][(int)h].unitonMineon&=254;
					map[(int)k][(int)h].unitindex=0;
					map[(int)k][(int)h].unitplayer=0;
				}
			}
			x=movetox;
			y=movetoy;
			revealmapcreation();
			for(float k=y; k<y+height; k+=.25)
			{
				for(float h=x; h<x+width; h+=.25)
				{
					map[(int)k][(int)h].unitonMineon|=1;
					map[(int)k][(int)h].unitindex=index;
					map[(int)k][(int)h].unitplayer=player;
				}
			}
			boundingbox.top=y;
			boundingbox.left=x;
			boundingbox.right=x+width;
			boundingbox.bottom=y+height;
			if(whatisit==2) //siege unit
			{
				for(unsigned int i=0; i<allsiegeunits[player][siegeindex].manning.size(); i++)//run the movement() of all manning units
					allunits[player][allsiegeunits[player][siegeindex].manning[i]]->movement(true);
			}
			else if(whatisit==3)
			{ //make sure all of the units in the ship stay in the ship, rather than 'fall out' and stand on the water behind it as it moves
				for(unsigned int i=0; i<allunits[player][index]->unitsinside.size(); i++)
				{
					allunits[player][allunits[player][index]->unitsinside[i]]->x=x;
					allunits[player][allunits[player][index]->unitsinside[i]]->y=y;
					allunits[player][allunits[player][index]->unitsinside[i]]->movetox=x;
					allunits[player][allunits[player][index]->unitsinside[i]]->movetoy=y;
				}
			}
			if(selected==true && sqrt(pow(allunits[player][generals[player]]->x-x,2)+pow(allunits[player][generals[player]]->y-y,2))>allunits[player][generals[player]]->los)
			{
				selected=false;
				for(unsigned int i=0; i<selectedunits[player].size(); i++)
				{
					if(selectedunits[player][i]==index)
					{
						selectedunits[player].erase(selectedunits[player].begin()+i);
						break;
					}
				}
				redraw=1;
			}
			return;
		}
		else
		{
			movetox=x;
			movetoy=y;
		}
	}
}
bool unit::checknomove(bool siegesent)
{
	if(unitstance==US_DONOTHING && userordered==false)
	{
        movetox=x;
        movetoy=y;
        return false;
    }
	if(whatisit==2 && allsiegeunits[player][siegeindex].manning.size()<(unsigned int)allsiegeunits[player][siegeindex].minmanning)
		return false; //not enough units manning the siege unit to move
	if(whatisit==3 && unitsinside.size()<mingarrison)
	{
        movetox=x;
        movetoy=y;
        return false; //not enough units in the ship for it to move
    }
	if(health<=0) //skip the movement of all dead units
		return false;
	if(whatisit!=2 && siegeindex!=-1 && siegesent==false)//skip the movement of all actively manning units, until the siege unit moves
	{
		for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
		{
			if(index==allsiegeunits[player][siegeindex].manning[j])
			{
				switch((j%4)+1)
				{
				case 1:
					if(sqrt(pow(allsiegeunits[player][siegeindex].x-(x+(int)(j/4)), 2)+pow(allsiegeunits[player][siegeindex].y-(y-allsiegeunits[player][siegeindex].height), 2))<=DIST_TO_MAN) //if the unit is within the proper distance of where it should be
						return false;
					break;
				case 2:
					if(sqrt(pow(allsiegeunits[player][siegeindex].x-(x+width-allsiegeunits[player][siegeindex].width-(int)(j/4)),2)+pow(allsiegeunits[player][siegeindex].y-(y-allsiegeunits[player][siegeindex].height),2))<=DIST_TO_MAN)
						return false;
					break;
				case 3:
					if(sqrt(pow(allsiegeunits[player][siegeindex].x-(x+(int)(j/4)), 2)+pow(allsiegeunits[player][siegeindex].y-(y+height),2))<=DIST_TO_MAN)
						return false;
					break;
				case 4:
					if(sqrt(pow(allsiegeunits[player][siegeindex].x-(x+width-allsiegeunits[player][siegeindex].width-(int)(j/4)),2)+pow(allsiegeunits[player][siegeindex].y-(y+height),2))<=DIST_TO_MAN)
						return false;
					break;
				}
			}
		}
	}
	if(movetox==x && movetoy==y)//if I have to stay in place
	{
		bool didsomething=false;
		if(allobstacles[player][index].size()!=0) // if I didn't finish not actually staying in place
		{
			movetox=x+allobstacles[player][index][0].x;//move towards the next spot specified by obstacle avoidance
			movetoy=y+allobstacles[player][index][0].y;//ditto
			allobstacles[player][index].erase(allobstacles[player][index].begin());//delete the next spot on my list from the list
			movement();//actually go there
			return false;
		}
		if(allobstacles[player][index].size()==0)
			userordered=true;
		if(attackingunitindex!=-1 && attackingunitplayer!=-1)// if I'm attacking someone
		{
            short addLOS=0;
            short addRange=0;
            for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
            {
                if(allResearches[alreadyResearched[player][i]].checkResearch(id))
                {
                    addLOS+=allResearches[alreadyResearched[player][i]].los;
                    addRange+=allResearches[alreadyResearched[player][i]].range;
                }
            }
			float dist=sqrt(pow(x-allunits[attackingunitplayer][attackingunitindex]->x, 2)+pow(y-allunits[attackingunitplayer][attackingunitindex]->y, 2)); //the dist from me to them
			if(dist<=los+addLOS-allunits[attackingunitplayer][attackingunitindex]->camouflage-tilecameo[map[(unsigned int)allunits[attackingunitplayer][attackingunitindex]->y][(unsigned int)allunits[attackingunitplayer][attackingunitindex]->x].tilestyle]) //can I see them
			{//if yes
				if(dist<attackrange+addRange) //can I attack them (in range)
				{
					if(unitstance!=US_DONOTHING || userordered==true)
						fight();
				}
				else //if I can't attack them
				{
					if(unitstance!=US_DONTMOVE || userordered==true)
					{
						attackingwhat=true;
						attackmovement(); //move towards them so that I can attack them		
					}
				}
			}
			else//if I can't see them
			{
				attackingunitindex=-1;  //they are no longer my target
				attackingunitplayer=-1;
				attackunitstance();
			}
			didsomething=true;
		}
		else if(whatisit!=2 && siegeindex==-1 && prevsiegeindex!=-1) //if a unit used to man a siege unit and is in sight of that siege unit and isnt doing anything else, man it
		{
			if(sqrt(pow(x-allsiegeunits[player][prevsiegeindex].x,2)+pow(y-allsiegeunits[player][prevsiegeindex].y,2))<=los)
			{
				if(allsiegeunits[player][prevsiegeindex].manning.size()<(unsigned int)allsiegeunits[player][prevsiegeindex].maxmanning)
				{
					allsiegeunits[player][prevsiegeindex].manning.push_back(index);
					siegeindex=prevsiegeindex;
					prevsiegeindex=-1;
				}
				else
					prevsiegeindex=-1;
			}
			didsomething=true;
		}
		else if(garrisoned>0) //garrisoning/building
		{
			if(allbuildings[player][garrisoned-1].beingbuilt>0) //building
			{
                short addbuildspeed=0;
                for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
                {
                    if(allResearches[alreadyResearched[player][i]].checkResearch(id))
                    {
                        addbuildspeed+=allResearches[alreadyResearched[player][i]].buildspeed;
                    }
                }
				allbuildings[player][garrisoned-1].beingbuilt-=buildspeed+addbuildspeed;	
				if(allbuildings[player][garrisoned-1].beingbuilt==0) //built
				{
					for(float k=allbuildings[player][garrisoned-1].y-allbuildings[player][garrisoned-1].radiustodistribute; k<allbuildings[player][garrisoned-1].y+allbuildings[player][garrisoned-1].height+allbuildings[player][garrisoned-1].radiustodistribute; k+=.25)
					{
						for(float h=allbuildings[player][garrisoned-1].x-allbuildings[player][garrisoned-1].radiustodistribute; h<allbuildings[player][garrisoned-1].x+allbuildings[player][garrisoned-1].width+allbuildings[player][garrisoned-1].radiustodistribute; h+=.25)
						{
                            if(pow(k-allbuildings[player][garrisoned-1].y,2)+pow(h-allbuildings[player][garrisoned-1].x,2)<pow(allbuildings[player][garrisoned-1].radiustodistribute,2))
                                map[(int)k][(int)h].inbuildingrad=garrisoned-1;
						}
					}
					if(allbuildings[player][garrisoned-1].maxhold>0)
					{
						for(unsigned int i=0; i<allunits[player].size(); i++)
						{
							if(allunits[player][i]->nearesthold!=-1)
							{
								if(sqrt(pow(allbuildings[player][garrisoned-1].x-allunits[player][i]->gatheringx,2)+pow(allbuildings[player][garrisoned-1].y-allunits[player][i]->gatheringy,2)) < sqrt(pow(allbuildings[player][allunits[player][i]->nearesthold].x-allunits[player][i]->gatheringx, 2)+pow(allbuildings[player][allunits[player][i]->nearesthold].y-allunits[player][i]->gatheringy, 2)))
								{
									allunits[player][i]->nearesthold=garrisoned-1;
								}
							}
						}
					}
					newlybuiltbuildings[player].push_back(garrisoned-1);
					garrisoned=0;
				}
			}
			else if(allbuildings[player][garrisoned-1].beingbuilt==0 || allbuildings[player][garrisoned-1].beingbuilt==-1) //
			{
				garrisoned=0;
			}
			else if(allbuildings[player][garrisoned-1].garrisoned.size()<(unsigned int)allbuildings[player][garrisoned-1].garrison)
			{
				if(((x>=allbuildings[player][garrisoned-1].x-1 && x<allbuildings[player][garrisoned-1].x) || (x>allbuildings[player][garrisoned-1].x+allbuildings[player][garrisoned-1].width && x<=allbuildings[player][garrisoned-1].x+allbuildings[player][garrisoned-1].width+1)) && ((y>=allbuildings[player][garrisoned-1].y+1 && y<allbuildings[player][garrisoned-1].y) || (y>allbuildings[player][garrisoned-1].y+allbuildings[player][garrisoned-1].height && y<=allbuildings[player][garrisoned-1].y+allbuildings[player][garrisoned-1].height-1)))
				{
					map[(int)y][(int)x].unitonMineon&=254; //garrisoning: remove unit, sets rightmost bit to 0, leaves the rest alone
					map[(int)y][(int)x].unitplayer=0;
					map[(int)y][(int)x].unitindex=0;
					x=allbuildings[player][garrisoned-1].x+allbuildings[player][garrisoned-1].width/2;//make the unit not appear in this case.
					y=allbuildings[player][garrisoned-1].y+allbuildings[player][garrisoned-1].height/2;
					movetox=x;
					movetoy=y;
					allbuildings[player][garrisoned-1].garrisoned.push_back(index);
					garrisoned=-garrisoned; //successfully garrisoned
					selected=false;
				}
			}
			didsomething=true;
		}
		else if(gatheringx!=-1 && gatheringy!=-1 && whatisit==1 && sqrt(pow(gatheringx-x,2)+pow(gatheringy-y,2))<=2.1)
		{
			if(map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle==TS_BERRIES)
				gather(0);
			else if(map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle==TS_TREE)
				gather(1);
            else if(map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle==TS_GOLD)
                gather(2);
            else if(map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle==TS_STONE)
                gather(3);
			else if(gatheringwhat!=map[(unsigned int)gatheringy][(unsigned int)gatheringx].tilestyle)
				searchresources();
			didsomething=true;
		}
		if(gatheringx!=-1 && gatheringy!=-1 && nearesthold!=-1 && whatisit==1)
		{
			int sumbuilding=0;
			int sumunit=0;
			for(int i=0; i<4; i++)
			{
				sumbuilding+=allbuildings[player][nearesthold].holding[i];
				sumunit+=holding[i];
			}
			if(sumbuilding+sumunit<=allbuildings[player][nearesthold].maxhold)
			{
				if(distToBuilding(nearesthold,1.2)) //If close enough to deposit into building, do it.
				{
					for(int i=0; i<4; i++)
					{
						resources[player][i]+=holding[i];
						allbuildings[player][nearesthold].holding[i]+=holding[i];
						holding[i]=0;
					}
					if(player==0 && (selected==true || allbuildings[player][nearesthold].selected==true))
						redraw=true;
					movetox=gatheringx;
					movetoy=gatheringy;
				}
			}
			didsomething=true;
		}
        if(unitbuildingwhat!=-1 && garrisoned==0)
        {
            int bindex=build(unitbuildingwhat, buildingx, buildingy);
            if(bindex!=-1) //success
            {
                buildingResourcesFrom=-1;
                unitbuildingwhat=-1;
                buildingx=-1;
                buildingy=-1;
                garrisoned=bindex+1;
                movetox=allbuildings[player][bindex].x;
                movetoy=allbuildings[player][bindex].y;
                didsomething=true;
            }            
            else
            {
                printf("What? Oops. Issue...");
                didsomething=false;
            }
        }
		if(didsomething==false)
		{
			if(dstancecoox==-1 || dstancecooy==-1)
			{
				dstancecoox=x;
				dstancecooy=y;
			}
			userordered=false;
			firstobstacleattempt=true; //at this point, actually meant to be doing nothing, for sure.
			attackunitstance();//possibly only call this every 5th frame, if there are serious performance issues when its large scale. Which there probably will be.
		}
		return false;
	}
	if(garrisoned>0) //garrisoning or building
	{
		if(allbuildings[player][garrisoned-1].beingbuilt>0 && distToBuilding(garrisoned-1,2)) //If you are building the building and you are close enough to do so
		{
			allbuildings[player][garrisoned-1].beingbuilt--;		
			if(allbuildings[player][garrisoned-1].beingbuilt==0)
			{
				for(float k=allbuildings[player][garrisoned-1].y-allbuildings[player][garrisoned-1].radiustodistribute; k<allbuildings[player][garrisoned-1].y+allbuildings[player][garrisoned-1].height+allbuildings[player][garrisoned-1].radiustodistribute; k+=.25)
				{
					for(float h=allbuildings[player][garrisoned-1].x-allbuildings[player][garrisoned-1].radiustodistribute; h<allbuildings[player][garrisoned-1].x+allbuildings[player][garrisoned-1].width+allbuildings[player][garrisoned-1].radiustodistribute; h+=.25)
					{
                        if(pow(k-allbuildings[player][garrisoned-1].y,2)+pow(h-allbuildings[player][garrisoned-1].x,2)<pow(allbuildings[player][garrisoned-1].radiustodistribute,2))
                            map[(int)k][(int)h].inbuildingrad=garrisoned-1;
					}
				}
				if(allbuildings[player][garrisoned-1].maxhold>0)
				{
					for(unsigned int i=0; i<allunits[player].size(); i++)
					{
						if(allunits[player][i]->nearesthold!=-1)
						{
							if(sqrt(pow(allbuildings[player][garrisoned-1].x-gatheringx,2)+pow(allbuildings[player][garrisoned-1].y-gatheringy,2)) < sqrt(pow(allbuildings[player][nearesthold].x-gatheringx, 2)+pow(allbuildings[player][nearesthold].y-gatheringy, 2)))
							{
								nearesthold=garrisoned-1;
							}
						}
					}
				}
				newlybuiltbuildings[player].push_back(garrisoned-1);
				garrisoned=0;
			}
		}
        else if(allbuildings[player][garrisoned-1].beingbuilt>0) ; //a place holder, so that in this scenario the unit still moves
		else if(allbuildings[player][garrisoned-1].beingbuilt==0 || allbuildings[player][garrisoned-1].beingbuilt==-1)
		{
			garrisoned=0;
		}
		else if(allbuildings[player][garrisoned-1].garrisoned.size()<(unsigned int)allbuildings[player][garrisoned-1].garrison)
		{
			if(x>=allbuildings[player][garrisoned-1].x-1 && x<allbuildings[player][garrisoned-1].x+allbuildings[player][garrisoned-1].width+1 && y>=allbuildings[player][garrisoned-1].y-1 && y<=allbuildings[player][garrisoned-1].y+allbuildings[player][garrisoned-1].height+1)
			{
				map[(int)y][(int)x].unitonMineon&=254;//garrisoning: remove unit
				map[(int)y][(int)x].unitplayer=0;
				map[(int)y][(int)x].unitindex=0;
				x=allbuildings[player][garrisoned-1].x+allbuildings[player][garrisoned-1].width/2;//make the unit not appear in this case.
				y=allbuildings[player][garrisoned-1].y+allbuildings[player][garrisoned-1].height/2;
				movetox=x;
				movetoy=y;
				allbuildings[player][garrisoned-1].garrisoned.push_back(index);
				garrisoned=-garrisoned; //successfully garrisoned
				selected=false;
			}
		}
		else
		{
			movetox=x;
			movetoy=y;
		}
	}
    if(gatheringx!=-1 && gatheringy!=-1 && nearesthold!=-1 && whatisit==1)
    {
        int sumbuilding=0;
        int sumunit=0;
        for(int i=0; i<4; i++)
        {
            sumbuilding+=allbuildings[player][nearesthold].holding[i];
            sumunit+=holding[i];
        }
        if(sumunit>=maxhold && sumbuilding+sumunit<=allbuildings[player][nearesthold].maxhold)
        {
            if(distToBuilding(nearesthold,1.2)) //If close enough to deposit into building, do it.
            {
                for(int i=0; i<4; i++)
                {
                    resources[player][i]+=holding[i];
                    allbuildings[player][nearesthold].holding[i]+=holding[i];
                    holding[i]=0;
                }
                if(player==0 && (selected==true || allbuildings[player][nearesthold].selected==true))
                    redraw=true;
                movetox=gatheringx;
                movetoy=gatheringy;
                return false;
            }
        }
    }
    if(unitbuildingwhat!=-1 && garrisoned==0 && buildingResourcesFrom>=0) //going to build building and getting resources from a unit
    {
        if(pow(allunits[player][buildingResourcesFrom]->x-x,2)+pow(allunits[player][buildingResourcesFrom]->y-y,2)<DISTUNITRESOURCE)
        {
            int bindex=build(unitbuildingwhat, buildingx, buildingy);
            if(bindex!=-1) //success
            {
                buildingResourcesFrom=-1;
                unitbuildingwhat=-1;
                buildingx=-1;
                buildingy=-1;
                garrisoned=bindex+1;
                movetox=allbuildings[player][bindex].x;
                movetoy=allbuildings[player][bindex].y;
            }            
            else
            {
                printf("What? Oops. Issue...");
            }
        }
        return true;
    }
	if(userordered==false && unitstance==US_DONTMOVE)
		return false;
	if(userordered==false && (unitstance==US_DEFENSIVE && sqrt(pow(x-dstancecoox, 2)+pow(y-dstancecooy, 2))>dstancerad))
	{
		if((movetox!=dstancecoox || movetoy!=dstancecooy) && dstancecoox!=-1 && dstancecooy!=-1)
		{
			movetox=dstancecoox;
			movetoy=dstancecooy;
			movement();
		}
		return false;
	}
	return true;
}
short unit::checkmove(point checkwhat, float actspeed) //0 is no good, called obstacle avoidance, 1 is good all clear, 2 is no good, called goingtobstacle
{
	if((int)(movetoy+.00001f)!=(int)(movetoy))
		movetoy+=.00001f;
	if((int)(movetox+.00001f)!=(int)(movetox))
		movetox+=.00001f;
	bool good5=chkmvp1(map[(int)movetoy][(int)movetox], false); //changed the order on these, they were second before. Got promoted.
	bool good6=true;
	bool good7=true;
	bool good8=true;
	if(map[(int)movetoy][(int)movetox].unitplayer!=-1 && map[(int)movetoy][(int)movetox].unitindex!=-1)
	{
		good6=chkmvp1(map[(int)movetoy][(int)(movetox+allunits[map[(int)movetoy][(int)movetox].unitplayer][map[(int)movetoy][(int)movetox].unitindex]->width-.001f)], false);
		good7=chkmvp1(map[(int)(movetoy+allunits[map[(int)movetoy][(int)movetox].unitplayer][map[(int)movetoy][(int)movetox].unitindex]->height-.001f)][(int)movetox], false);
		good8=chkmvp1(map[(int)(movetoy+allunits[map[(int)movetoy][(int)movetox].unitplayer][map[(int)movetoy][(int)movetox].unitindex]->height-.001f)][(int)(movetox+allunits[map[(int)movetoy][(int)movetox].unitplayer][map[(int)movetoy][(int)movetox].unitindex]->width-.001f)], false);
	}
	if(good5==false || good6==false || good7==false || good8==false)
	{
		if(map[(int)movetoy][(int)movetox].tilestyle==TS_BUILDING)
		{
			if(map[(int)movetoy][(int)movetox].buildingplayer==player && map[(int)movetoy][(int)movetox].buildingindex==garrisoned-1)
			{
				;//Just a placeholder, intended to be blank
			}
		}
		else if(allunits[map[(int)movetoy][(int)movetox].unitplayer][map[(int)movetoy][(int)movetox].unitindex]->whatisit==3)
			;//Just a placeholder, intended to be blank
		else
		{
			goingtobstacle();
			return 2;
		}
	}
	bool good1=chkmvp1(map[(int)checkwhat.y][(int)checkwhat.x]);
	bool good2=true;
	bool good3=true;
	bool good4=true;
	if(map[(int)checkwhat.y][(int)checkwhat.x].unitplayer!=-1 && map[(int)checkwhat.y][(int)checkwhat.x].unitindex!=-1)
	{
		good2=chkmvp1(map[(int)checkwhat.y][(int)(checkwhat.x+allunits[map[(int)checkwhat.y][(int)checkwhat.x].unitplayer][map[(int)checkwhat.y][(int)checkwhat.x].unitindex]->width-.001f)]);
		good3=chkmvp1(map[(int)(checkwhat.y+allunits[map[(int)checkwhat.y][(int)checkwhat.x].unitplayer][map[(int)checkwhat.y][(int)checkwhat.x].unitindex]->height-.001f)][(int)(checkwhat.x)]);
		good4=chkmvp1(map[(int)(checkwhat.y+allunits[map[(int)checkwhat.y][(int)checkwhat.x].unitplayer][map[(int)checkwhat.y][(int)checkwhat.x].unitindex]->height-.001f)][(int)(checkwhat.x+allunits[map[(int)checkwhat.y][(int)checkwhat.x].unitplayer][map[(int)checkwhat.y][(int)checkwhat.x].unitindex]->width-.001f)]);
	}
	if(good1==false || good2==false || good3==false || good4==false)
	{
		//HANDLE bothhandles[2]; //the handles to the threads that will be created
		//obstaclethreadparam topass((int)checkwhat.x-(int)x, (int)checkwhat.y-(int)y, x, y, 1, player, index, 0); //the first to pass, tells it where the unit is trying to go, where the unit is, and whether to test up/left
		//bothhandles[0] = CreateThread(NULL, NULL, threadgoaroundobstacle, &topass, NULL, NULL); //starts the thread
		//obstaclethreadparam topass2((int)checkwhat.x-(int)x, (int)checkwhat.y-(int)y, x, y, -1, player, index, 1); //same but tells it to test down/right 
		//bothhandles[1] = CreateThread(NULL, NULL, threadgoaroundobstacle, &topass2, NULL, NULL); //same with new param
		//WaitForMultipleObjects(2, bothhandles, TRUE, INFINITE); //wait for the threads to finish
		//CloseHandle(bothhandles[0]);//make sure the threads are totally done
		//CloseHandle(bothhandles[1]);
		/*short walkable[6]={0};
		if(whatisit!=3)//if we walk on land
		{
			walkable[0]=TS_GRASS;
			walkable[1]=TS_GOLD;
			walkable[2]=TS_STONE;
			walkable[3]=TS_ROAD;
			walkable[4]=TS_WATERBUILDING;
			walkable[5]=TS_BUSHES;
		}
		else//ship, on water
		{
			walkable[0]=TS_WATER;
			walkable[1]=TS_FISH;
		}*/
		if(whatisit==2)
		{
			int highest=0;
			for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
			{
				if(j%4==0 || j%4==1)
				{
					if(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height>highest)
						highest=allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
				}
			}
			y+=highest; //'moves' the siege unit lower, really just makeing it the same height as it + the units manning it
			highest=0;
			for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
			{
				if(j%4==2 || j%4==3)
				{
					if(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height>highest)
						highest=allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
				}
			}
			height+=highest;
		}

		float dirx=movetox-checkwhat.x;
		float diry=movetoy-checkwhat.y;
		vector<point> temp=allobstacles[player][index];
		allobstacles[player][index]=astarsearch(problem(point(checkwhat.x,checkwhat.y),dirx>0,diry>0),actspeed,this);
		float difx=0;
		float dify=0;
		for(unsigned int i=0; i<allobstacles[player][index].size(); i++)
		{
			difx+=allobstacles[player][index][i].x;
			dify+=allobstacles[player][index][i].y;
		}
		if(difx!=0 || dify!=0)
		{
			float toaddx=movetox-(x+difx);
			float toaddy=movetoy-(y+dify);
			if(((toaddx+.0001)/abs(toaddx+.0001)==(toaddx/abs(toaddx)) && (toaddx-.0001)/abs(toaddx-.0001)==(toaddx/abs(toaddx))) || ((toaddy+.0001)/abs(toaddy+.0001)==(toaddy/abs(toaddy)) && (toaddy-.0001)/abs(toaddy-.0001)==(toaddy/abs(toaddy)))) //toadd is not approximately 0
				allobstacles[player][index].push_back(point(toaddx,toaddy));
			for(unsigned int i=0; i<temp.size(); i++)
				allobstacles[player][index].push_back(temp[i]);
		}
		else //failed
			firstobstacleattempt=false;
		if(whatisit==2)//change the height/ypos back to normal
		{
			int highest=0;
			for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
			{
				if(j%4==0 || j%4==1)
				{
					if(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height>highest)
						highest=allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
				}
			}
			y-=highest; //'moves' the siege unit back up
			highest=0;
			for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
			{
				if(j%4==2 || j%4==3)
				{
					if(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height>highest)
						highest=allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
				}
			}
			height-=highest;
			for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
			{
				switch((j%4)+1)
				{
				case 1:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height), 2))<=DIST_TO_MAN) //if the unit is with the proper distance of where it should be
					{
						for(unsigned int k=0; k<allobstacles[player][index].size(); k++)
						{
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].x=allobstacles[player][index*2][k].x+(int)(j/4);
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].y=allobstacles[player][index*2][k].y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
							allobstacles[player][allsiegeunits[player][siegeindex].manning[j]].push_back(point(allobstacles[player][index][k].x+(int)(j/4), allobstacles[player][index][k].y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height));
						}
					}
					break;
				case 2:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height),2))<=DIST_TO_MAN)
					{
						for(unsigned int k=0; k<allobstacles[player][index].size(); k++)
						{
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].x=allobstacles[player][index*2][k].x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4);
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].y=allobstacles[player][index*2][k].y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
							allobstacles[player][allsiegeunits[player][siegeindex].manning[j]].push_back(point(allobstacles[player][index][k].x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4), allobstacles[player][index][k].y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height)); 
						}
					}
					break;
				case 3:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
					{
						for(unsigned int k=0; k<allobstacles[player][index].size(); k++)
						{
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].x=allobstacles[player][index*2][k].x+(int)(j/4);
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].y=allobstacles[player][index*2][k].y+height;
							allobstacles[player][allsiegeunits[player][siegeindex].manning[j]].push_back(point(allobstacles[player][index][k].x+(int)(j/4), allobstacles[player][index][k].y+height));
						}
					}
					break;
				case 4:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
					{
						for(unsigned int k=0; k<allobstacles[player][index].size(); k++)
						{
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].x=allobstacles[player][index*2][k].x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4);
							//allobstacles[player][allsiegeunits[player][siegeindex].manning[j]*2][k].y=allobstacles[player][index*2][k].y+height;
							allobstacles[player][allsiegeunits[player][siegeindex].manning[j]].push_back(point(allobstacles[player][index][k].x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4), allobstacles[player][index][k].y+height));
						}
					}
					break;
				}
			}
		}
		return false;
	}
	return true;
}
bool unit::attackunitstance() //called when the unit is not doing anything, to check if there are units for it to attack//for a certain unit, of a certain player
{//true==found something to attack, except for in switch((j%4)+1), there its just so that the caller returns, as well as the first return, for the same reason
	if(whatisit==2 && allsiegeunits[player][siegeindex].manning.size()<(unsigned int)allsiegeunits[player][siegeindex].minmanning)
		return true; //not enough units manning the siege unit to do anything
	if(index==generals[player]) //This function does not run for generals
		return false;
	if(whatisit!=2 && siegeindex!=-1)//don't do anything if its busy manning something
	{
		if(whatisit==2)
		{
			for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
			{
				if(index==allsiegeunits[player][siegeindex].manning[j])
				{
					switch((j%4)+1)
					{
					case 1:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height), 2))<=DIST_TO_MAN) //if the unit is within the proper distance of where it should be
							return true;
						break;
					case 2:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height),2))<=DIST_TO_MAN)
							return true;
						break;
					case 3:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
							return true;
						break;
					case 4:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
							return true;
						break;
					}
				}
			}
		}
	}
	vector<pointex> allseenunits; //this store all the seen units and buildings //The below cycles through each point in the circle with integer coorinates (each map tile in the circle)
	short cyclethrough=0;
	float startx=x;
	float starty=y;
    short uselos=los;
    short addRange=0;
    for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
    {
        if(allResearches[alreadyResearched[player][i]].checkResearch(id))
        {
            uselos+=allResearches[alreadyResearched[player][i]].los;
            addRange+=allResearches[alreadyResearched[player][i]].range;
        }
    }
	if(unitstance==US_AGGRESIVE || userordered==true)
		cyclethrough=(short)uselos;
	else if(unitstance==US_DEFENSIVE && userordered==false)
	{
		startx=dstancecoox;
		starty=dstancecooy;
		if(dstancerad+attackrange+addRange>=uselos)
			cyclethrough=(short)uselos;
		else
			cyclethrough=dstancerad+(short)attackrange+addRange;
	}
	else if(unitstance==US_DONTMOVE && userordered==false)
		cyclethrough=(short)attackrange+addRange;
	else if(unitstance==US_DONOTHING && userordered==false)
		return false;
	else
		printf("Major Error. Search -99999");
		//exit(99999);
	if(garrisoned<=-1 && attackrange+addRange>MELEE)
		cyclethrough=allbuildings[player][garrisoned-1].range;
	else if(garrisoned<=-1 && attackrange+addRange<=MELEE)
		cyclethrough=MELEE;
	checkrad(cyclethrough, startx, starty, allseenunits);
	/*for(int i=1; i<=cyclethrough; i++) //cycle through the horizontal diameter of the circle made by having a circle with center at the unit and radius LOS
	{
		pointex unit=getcandidate((unsigned int)startx+i, (unsigned int)starty, i);
		if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
			allseenunits.push_back(unit);
		unit=getcandidate((unsigned int)startx, (unsigned int)starty+i, i);
		if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
			allseenunits.push_back(unit);
		if(startx-i>=0)
		{
			unit=getcandidate((unsigned int)startx-i, (unsigned int)starty, i);
			if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
				allseenunits.push_back(unit);
		}
		if(starty-i>=0)
		{
			unit=getcandidate((unsigned int)startx, (unsigned int)starty-i, i);
			if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
				allseenunits.push_back(unit);
		}
		for(int j=1; j<cyclethrough; j++)//loop through the vertical diameter of the circle
		{   //if the point that is the intersection of the line made by drawing a vertical line through the point on the horizontal diameter(i) and a horizontal one through the point on the vertical diameter(j)
			double dist=sqrt(double(i*i)+double(j*j)); //get the distance from the unit to the tile in question
			if(dist<=cyclethrough)
			{
				pointex unit2=getcandidate((unsigned int)startx+i, (unsigned int)starty+j, dist); //check whether there is an acceptable candidate unit there
				if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1) //if there is,
					allseenunits.push_back(unit2); //record it
				if(startx-i>=0)
				{
					unit2=getcandidate((unsigned int)startx-i, (unsigned int)starty+j, dist); //check for the other three quadrants
					if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1)
						allseenunits.push_back(unit2);
				}
				if(starty-j>=0)
				{
					unit2=getcandidate((unsigned int)startx+i, (unsigned int)starty-j, dist);
					if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1)
						allseenunits.push_back(unit2);
				}	
				if(startx-i>=0 && starty-j>=0)
				{
					unit2=getcandidate((unsigned int)startx-i, (unsigned int)starty-j, dist);
					if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1)
						allseenunits.push_back(unit2);
				}
			}
		}
	}*/
	int lowestindexunit=-1;
	int lowestindexbuilding=-1;
	double lowestunitdist=1000000;
	double lowestbuildingdist=1000000;
	for(unsigned int i=0; i<allseenunits.size(); i++)
	{
		if(allseenunits[i].dist<lowestunitdist && allseenunits[i].unit==true)
		{
			lowestunitdist=allseenunits[i].dist;
			lowestindexunit=i;
		}
		if(allseenunits[i].dist<lowestbuildingdist && allseenunits[i].unit==false)
		{
			lowestbuildingdist=allseenunits[i].dist;
			lowestindexbuilding=i;
		}
	}
	if(lowestindexunit!=-1)
	{//attack unit
		attackingunitindex=map[(unsigned int)allseenunits[lowestindexunit].y][(unsigned int)allseenunits[lowestindexunit].x].unitindex;
		attackingunitplayer=map[(unsigned int)allseenunits[lowestindexunit].y][(unsigned int)allseenunits[lowestindexunit].x].unitplayer;
		attackingwhat=true;
		attackmovement();
		return true;
	}
	else if(lowestindexbuilding!=-1 && allunits[player][index]->buildingattack!=0)
	{//attack building
		attackingunitindex=map[(unsigned int)allseenunits[lowestindexbuilding].y][(unsigned int)allseenunits[lowestindexbuilding].x].buildingindex;
		attackingunitplayer=map[(unsigned int)allseenunits[lowestindexbuilding].y][(unsigned int)allseenunits[lowestindexbuilding].x].buildingplayer;
		attackingwhat=false;
		attackmovement();
		return true;
	}
	else
		return false;
}
void unit::checkrad(int cyclethrough, float startx, float starty, vector<pointex> &allseenunits)
{
	if(whatisit!=2 && siegeindex!=-1)//don't do anything if its busy manning something
	{
		if(whatisit==2)
		{
			for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
			{
				if(index==allsiegeunits[player][siegeindex].manning[j])
				{
					switch((j%4)+1)
					{
					case 1:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height), 2))<=DIST_TO_MAN) //if the unit is within the proper distance of where it should be
							return;
						break;
					case 2:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height),2))<=DIST_TO_MAN)
							return;
						break;
					case 3:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
							return;
						break;
					case 4:
						if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
							return;
						break;
					}
				}
			}
		}
	}
	for(int i=1; i<=cyclethrough; i++) //cycle through the horizontal diameter of the circle made by having a circle with center at the unit and radius LOS
	{
		pointex unit=getcandidate((unsigned int)startx+i, (unsigned int)starty, i);
		if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
			allseenunits.push_back(unit);
		unit=getcandidate((unsigned int)startx, (unsigned int)starty+i, i);
		if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
			allseenunits.push_back(unit);
		if(startx-i>=0)
		{
			unit=getcandidate((unsigned int)startx-i, (unsigned int)starty, i);
			if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
				allseenunits.push_back(unit);
		}
		if(starty-i>=0)
		{
			unit=getcandidate((unsigned int)startx, (unsigned int)starty-i, i);
			if(unit.x!=-1 && unit.y!=-1 && unit.dist!=-1)
				allseenunits.push_back(unit);
		}
		for(int j=1; j<cyclethrough; j++)//loop through the vertical diameter of the circle
		{   //if the point that is the intersection of the line made by drawing a vertical line through the point on the horizontal diameter(i) and a horizontal one through the point on the vertical diameter(j)
			double dist=sqrt(double(i*i)+double(j*j)); //get the distance from the unit to the tile in question
			if(dist<=cyclethrough)
			{
				pointex unit2=getcandidate((unsigned int)startx+i, (unsigned int)starty+j, dist); //check whether there is an acceptable candidate unit there
				if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1) //if there is,
					allseenunits.push_back(unit2); //record it
				if(startx-i>=0)
				{
					unit2=getcandidate((unsigned int)startx-i, (unsigned int)starty+j, dist); //check for the other three quadrants
					if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1)
						allseenunits.push_back(unit2);
				}
				if(starty-j>=0)
				{
					unit2=getcandidate((unsigned int)startx+i, (unsigned int)starty-j, dist);
					if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1)
						allseenunits.push_back(unit2);
				}	
				if(startx-i>=0 && starty-j>=0)
				{
					unit2=getcandidate((unsigned int)startx-i, (unsigned int)starty-j, dist);
					if(unit2.x!=-1 && unit2.y!=-1 && dist!=-1)
						allseenunits.push_back(unit2);
				}
			}
		}
	}
}
void unit::attackmovement() //the index and player of the unit that is going to a location where it can attack something, as well as whether it is going to attack a unit or a building
{
	if(whatisit==2 && allsiegeunits[player][siegeindex].manning.size()<(unsigned int)allsiegeunits[player][siegeindex].minmanning)
		return; //not enough units manning the siege unit to do anything
	if(whatisit!=2 && siegeindex!=-1)//don't do anything if its busy manning something
	{
		for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
		{
			if(index==allsiegeunits[player][siegeindex].manning[j])
			{
				switch((j%4)+1)
				{
				case 1:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height), 2))<=DIST_TO_MAN) //if the unit is with the proper distance of where it should be
						return;
					break;
				case 2:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height),2))<=DIST_TO_MAN)
						return;
					break;
				case 3:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
						return;
					break;
				case 4:
					if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
						return;
					break;
				}
			}
		}
	}
	if(unitstance==US_DONOTHING && userordered==false)
		return;
	float enemyx=0;//where is the enemy
	float enemyy=0;
    float userange=attackrange;
    for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
    {
        if(allResearches[alreadyResearched[player][i]].checkResearch(id))
        {
            userange+=allResearches[alreadyResearched[player][i]].range;
        }
    }
	if(attackingwhat==true)
	{
		enemyx=allunits[attackingunitplayer][attackingunitindex]->x;
		enemyy=allunits[attackingunitplayer][attackingunitindex]->y;
	}
	else
	{
		enemyx=allbuildings[attackingunitplayer][attackingunitindex].x;
		enemyy=allbuildings[attackingunitplayer][attackingunitindex].y;
		float dist1=sqrt(pow((enemyx-x), 2)+pow((enemyy-y), 2))-userange;
		float enemyx2=allbuildings[attackingunitplayer][attackingunitindex].x+allbuildings[attackingunitplayer][attackingunitindex].width;
		float enemyy2=allbuildings[attackingunitplayer][attackingunitindex].y+allbuildings[attackingunitplayer][attackingunitindex].height;
		float tempx=enemyx;
		if(sqrt(pow((enemyx2-x), 2)+pow((enemyy-y), 2))-userange<dist1)
		{
			dist1=sqrt(pow((enemyx2-x), 2)+pow((enemyy-y), 2))-userange;
			enemyx=enemyx2;
		}
		if(sqrt(pow((enemyx2-x), 2)+pow((enemyy2-y), 2))-userange<dist1)
		{
			dist1=sqrt(pow((enemyx2-x), 2)+pow((enemyy2-y), 2))-userange;
			enemyy=enemyy2;
			enemyx=enemyx2;
		}
		if(sqrt(pow((enemyx-x), 2)+pow((enemyy2-y), 2))-userange<dist1)
		{
			dist1=sqrt(pow((enemyx-x), 2)+pow((enemyy2-y), 2))-userange;
			enemyx=tempx;
			enemyy=enemyy2;
		}
	}
	float range=userange;
	if(garrisoned<=-1 && attackrange>MELEE)
		range=allbuildings[player][(-garrisoned)-1].range;
	if(garrisoned<=-1)
		range+=allbuildings[player][(-garrisoned)-1].height/2;//compensate for it being in the center, so that dist is calculated approx from wall, not from center
	float dist=sqrt(pow((enemyx-x), 2)+pow((enemyy-y), 2))-range; //the distance from unit location to the actual destination, - my range, so that if I'm in range it will be negative
	if(dist<=0.05)
	{
		fight();
		return;
	}
	else
	{
		if(unitstance==US_DONTMOVE && userordered==false)
			return;
		if(garrisoned<=-1)
			return;
	}
	if(userordered==false && unitstance==US_DEFENSIVE && dist+sqrt(pow(x-dstancecoox, 2)+pow(y-dstancecooy, 2))>dstancerad)
	{
		attackingunitindex=-1;
		attackingunitplayer=-1;
		bool found=attackunitstance();
		if(found==false)
		{
			movetox=dstancecoox;//go back to original position
			movetoy=dstancecooy;
			if(whatisit!=3)//check if that is valid (if its a land unit)
			{
				//if(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_GRASS && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_ROAD && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_GOLD && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_STONE && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_WATERBUILDING && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_BERRIES && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_BUSHES && !(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle==TS_GATE && map[(unsigned int)movetoy][(unsigned int)movetox].player==players[player]) && !(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle==TS_GATE && map[(unsigned int)movetoy][(unsigned int)movetox].player==players[player]))
				if(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_GRASS && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_ROAD && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_GOLD && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_STONE && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_WATERBUILDING && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_BERRIES && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_BUSHES && !(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle==TS_GATE && map[(unsigned int)movetoy][(unsigned int)movetox].buildingplayer==players[player]))
				{
					goingtobstacle();
				}
			}
			else
			{
				//if(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_WATER && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_FISH && !(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle==TS_GATE && map[(unsigned int)movetoy][(unsigned int)movetox].player==players[player]) && !(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle==TS_GATE && map[(unsigned int)movetoy][(unsigned int)movetox].player==players[player]))
				if(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_WATER && map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle!=TS_FISH && !(map[(unsigned int)movetoy][(unsigned int)movetox].tilestyle==TS_GATE && map[(unsigned int)movetoy][(unsigned int)movetox].buildingplayer==players[player]))
				{
					goingtobstacle();
				}
			}
			movement();
			return;
		}
		else
			return;
	}
	if(abs(enemyx-x)>=0.0001) //if the line is not vertical
	{
		float slope=(enemyy-y)/(enemyx-x); //slope
		//float b=y-(slope*x); //y-intercept
		//float newx1=(sqrt(-(b*b)+(2*b*y)-(2*b*slope*x)-(y*y)+(2*y*slope*x)+((dist*dist)*(slope*slope))+(dist*dist)-((slope*slope)*(x*x)))-(b*slope)+(y*slope)+x)/((slope*slope)+1);
		//float newx2=(-sqrt(-(b*b)+(2*b*y)-(2*b*slope*x)-(y*y)+(2*y*slope*x)+((dist*dist)*(slope*slope))+(dist*dist)-((slope*slope)*(x*x)))-(b*slope)+(y*slope)+x)/((slope*slope)+1);
		//float newy1=(slope*newx1)+b; //we got the x-coordinate above, heres the y coordinate
		//float newy2=(slope*newx2)+b; //2 x's, 2 y's
        float newx1=(sqrt(speed*speed*(slope*slope+1))+slope*slope*x+x)/(slope*slope+1);
        float newx2=(-sqrt(speed*speed*(slope*slope+1))+slope*slope*x+x)/(slope*slope+1);
        float newy1=slope*(newx1-x)+y;
        float newy2=slope*(newx2-x)+y;
		int conditionsmet=0; //was the point both between the start and finish in x and in y?
		bool firstoneisgood=false; //is the first one correct
		conditionsmet+=pointonlinesegment(x, enemyx, newx1); //check x
		conditionsmet+=pointonlinesegment(y, enemyy, newy1); //check y
		if(conditionsmet==2)//if both correct
			firstoneisgood=true;//say so
		conditionsmet=0;//for x2 and y2 results
		conditionsmet+=pointonlinesegment(x, enemyx, newx2);
		conditionsmet+=pointonlinesegment(y, enemyy, newy2);
		if(conditionsmet==2 && firstoneisgood==true) //if both are good
			printf("Major Error. Search -74");//exit(-74);
		else if(conditionsmet!=2 && firstoneisgood==false) //if neither is good
			printf("Major Error. Search -60");//exit(-60);
		if(firstoneisgood==true)
		{
			movetox=newx1;
			movetoy=newy1;
		}
		else
		{
			movetox=newx2;
			movetoy=newy2;
		}
	}
	else //line is vert
	{
		movetox=x;
		/*if(pointonlinesegment(y, enemyy, y+dist)==true)
			movetoy=y+dist;
		else
			movetoy=y-dist;*/
        if(pointonlinesegment(y, enemyy, y+speed)==true)
			movetoy=y+speed;
		else
			movetoy=y-speed;
	}
    if(!chkmvp1(map[(int)movetoy][(int)movetox]) || !chkmvp1(map[(int)movetoy][(int)(movetox+width-.001f)]) || !chkmvp1(map[(int)(movetoy+height-.001f)][(int)movetox]) || !chkmvp1(map[(int)(movetoy+height-.001f)][(int)(movetox+width-.001f)]))
	{
        if(whatisit!=3)
        {
            char canmoveto[6]={TS_GRASS, TS_ROAD, TS_GOLD, TS_STONE, TS_WATERBUILDING, TS_BUSHES};
			attackgoingtobstacle(canmoveto);
        }
        else
        {
            char canmoveto[6]={TS_WATER, TS_FISH, 0, 0, 0, 0};
			attackgoingtobstacle(canmoveto);
        }
    }
	if(whatisit==2) //siegeunit
	{
		for(unsigned int j=0; j<allsiegeunits[player][siegeindex].manning.size(); j++)
		{
			switch((j%4)+1)
			{
			case 1:
				if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height), 2))<=DIST_TO_MAN) //if the unit is with the proper distance of where it should be
				{
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetox=movetox+(int)(j/4);
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetoy=movetoy-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
				}
				break;
			case 2:
				if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height),2))<=DIST_TO_MAN)
				{
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetox=movetox+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4);
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetoy=movetoy-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->height;
				}
				break;
			case 3:
				if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
				{
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetox=movetox+(int)(j/4);
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetoy=movetoy+height;
				}
				break;
			case 4:
				if(sqrt(pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->x-(x+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][siegeindex].manning[j]]->y-(y+height),2))<=DIST_TO_MAN)
				{
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetox=movetox+width-allunits[player][allsiegeunits[player][siegeindex].manning[j]]->width-(int)(j/4);
					allunits[player][allsiegeunits[player][siegeindex].manning[j]]->movetoy=movetoy+height;
				}
				break;
			}
		}
	}
}
void unit::fight()
{
    int change=0;
    if(attackingwhat==true) //attacking unit
        change=innerFight(allunits[attackingunitplayer][attackingunitindex]);
    else if(attackingwhat==false) //attacking building
        change=innerFight(allbuildings[attackingunitplayer][attackingunitindex]);
    if(attackarea!=0)
    {
        int ax;
        int ay;
        ax=(attackingwhat) ? (int)(allunits[attackingunitplayer][attackingunitindex]->x) : (int)(allbuildings[attackingunitplayer][attackingunitindex].x);
        ay=(attackingwhat) ? (int)(allunits[attackingunitplayer][attackingunitindex]->y) : (int)(allbuildings[attackingunitplayer][attackingunitindex].y);
        for(int i=ax+1; i<ax+attackarea; i++)
        {
            for(int j=ay+1; j<ay+attackarea; j++)
            {
                if(map[j][i].tilestyle==TS_BUILDING)
                {
                    innerFight(allbuildings[map[j][i].buildingplayer][map[j][i].buildingindex]);
                }
                if((map[j][i].unitonMineon&1)==1) //unit on
                {
                    innerFight(allunits[map[j][i].unitplayer][map[j][i].unitindex]);
                }
            }
        }
    }
    if(change==1)
    {
        attackingunitplayer=-1;
        attackingunitindex=-1;
    }
    else if(change==2)
    {
        attackingunitplayer=-1;
        attackingunitindex=-1;
        attackunitstance();
    }
}
int unit::innerFight(unit *what)
{		
    if(what->health<=0)
    {
        //attackingunitindex=-1;
        //attackingunitplayer=-1;
        return 1;
    }
	if(whatisit==2 && allsiegeunits[player][siegeindex].manning.size()<(unsigned int)allsiegeunits[player][siegeindex].minmanning)
		return 0; //not enough units manning the siege unit to do anything
	if(unitstance==US_DONOTHING && userordered==false)
		return 0;
	if(frames%attackspeed==0)
	{
        short addRange=0;
        for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
        {
            if(allResearches[alreadyResearched[player][i]].checkResearch(id))
            {
                addRange+=allResearches[alreadyResearched[player][i]].range;
            }
        }
        float dist=sqrt(pow(x-what->x, 2)+pow(y-what->y, 2));
        float actualrange=attackrange+addRange+0.05f;
        if(garrisoned<=-1)
        {
            if(attackrange!=MELEE)
                actualrange=allbuildings[player][(-garrisoned)-1].range+0.05f;
        }
        if(dist<=actualrange) //in range
        {
            float attack=0;
            if(dist<=MELEE) //must melee (siege machines can't do anything)
            {
                attack=meleeattack;
                if(garrisoned<=-1)
                    attack+=allbuildings[player][(-garrisoned)-1].garrisonedmeleeattack;
                if(whatisit==2) //siegeunit
                {
                    for(unsigned int i=0; i<allsiegeunits[player][siegeindex].manning.size(); i++)
                    {
                        allunits[player][allsiegeunits[player][siegeindex].manning[i]]->siegeindex=-1;
                        if(allunits[player][allsiegeunits[player][siegeindex].manning[i]]->attackingunitindex==-1 || allunits[player][allsiegeunits[player][siegeindex].manning[i]]->attackingunitplayer==-1)
                            allunits[player][allsiegeunits[player][siegeindex].manning[i]]->attackunitstance();
                        else
                            allunits[player][allsiegeunits[player][siegeindex].manning[i]]->fight();
                        allobstacles[player][allsiegeunits[player][siegeindex].manning[i]].push_back(point(allunits[player][allsiegeunits[player][siegeindex].manning[i]]->x, allunits[player][allsiegeunits[player][siegeindex].manning[i]]->y)); //tell it to come back when its done, by making its current location a waypoint in the obstacle avoidance vector
                    }
                    allsiegeunits[player][siegeindex].manning.clear();
                }
            }
            else //must range
            {
                attack=rangedattack;
                if(garrisoned<=-1)
                    attack+=allbuildings[player][(-garrisoned)-1].garrisonedrangedattack;
            }
            attack+=(veterancylvl/**0.5f*/); //add for vet lvl
            if(!(garrisoned<=-1) && regimentid>=0) //if not garrisoned, add for regiment lvl
                attack+=allregiments[player][regimentid].reglvl/2;//4;
            short addAccuracy=0;
            for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
            {
                if(allResearches[alreadyResearched[player][i]].checkResearch(id))
                {
                    attack+=allResearches[alreadyResearched[player][i]].attack;
                    addAccuracy=allResearches[alreadyResearched[player][i]].accuracy;
                }
            }
            for(unsigned int i=0; i<allBonuses.size(); i++) //apply attack bonuses
            {
                for(unsigned int j=0; j<allBonuses[i].againstWhat.size(); j++)
                {
                    if(allBonuses[i].againstWhat[j]==what->id) //bonus applicable
                    {
                        attack+=allBonuses[i].attackBonus;
                        attack*=allBonuses[i].attackPercentBonus;
                        break;
                    }
                }
            }

            float armor=what->armor; //next 4 lines set armor of unit being attacked;
            if(what->garrisoned<=-1)
                armor+=allbuildings[what->player][(-what->garrisoned)-1].garrisonedarmorup; //add for garrisoned
            else if(what->regimentid>=0)
                armor+=allregiments[what->player][what->regimentid].reglvl/3;//5; //add for reg lvl, if not garrisoned
            armor+=what->veterancylvl/2;//3.0f; //add for vet lvl
            for(unsigned int i=0; i<alreadyResearched[what->player].size(); i++) //apply research bonuses
            {
                if(allResearches[alreadyResearched[what->player][i]].checkResearch(what->id))
                    armor+=allResearches[alreadyResearched[what->player][i]].armor;
            }
            for(unsigned int i=0; i<what->allBonuses.size(); i++) //apply defense bonuses
            {
                for(unsigned int j=0; j<what->allBonuses[i].againstWhat.size(); j++)
                {
                    if(what->allBonuses[i].againstWhat[j]==id) //bonus applicable
                    {
                        armor+=what->allBonuses[i].defenseBonus;
                        armor*=what->allBonuses[i].defensePercentBonus;
                        break;
                    }
                }
            }

            float hpercent=(health/allbuildableunits[id].health)*100;
            float dmg=0;
            if(hpercent==100)
                dmg=(float)attack-(float)armor;
            //for this formula and what it does, see the excel document called age of emps health percents, sheet one
            else if(hpercent<100 && hpercent>25)
                dmg=attack-((attack*.01f*((.0159f*hpercent*hpercent)-(3.2933f*hpercent)+171.81f)))-armor;
            if(dmg<=0) //if its <=25, will go here, dmg is 0
                return 0;

            int chance=chanceHit+addAccuracy; //chance of hitting: note, possibly make a chance of blocking for defender and chance of missing for attacker and use those here as well, esp for ranged units
            if(what->garrisoned<=-1) 
                chance-=allbuildings[what->player][(-what->garrisoned)-1].chanceofbeinghit;
            int rndnum = rand()%100;
            if(rndnum<chance)
            {
                what->health-=dmg;
                if(what->player==0 && allunits[0][what->index]->selected==true) //its the humans and its selected - update its health in disp
                    redraw=true;
                if(what->health<=0) //killed it
                {
                    if(regimentid!=-1 && allregiments[player][regimentid].recording==true)
                        allregiments[player][regimentid].rep.updatedkilledunits(what->player, what->index);
                    else if(selfRecording)
                        rep.updatedkilledunits(what->player, what->index);
                    if(what->regimentid!=-1)
                    {
                        allregiments[what->player][what->regimentid].removeunit(what->index);
                        what->regimentid=-1;
                    }
                    overwriteunits[what->player].push_back(what->index);//it died, say its rewritable
                    for(int g=0; g<4; g++)
                    {
                        map[(unsigned int)what->y][(unsigned int)what->x].resources[g]=what->holding[g];
                        what->holding[g]=0;
                        //CREATE BUILDING: PILE (JUST BUNCH OF RESOURCES) belongs to no one  (Not walkable)
                    }
                    for(float k=what->y; k<what->y+what->height; k+=.25)
                    {
                        for(float h=what->x; h<what->x+what->width; h+=.25)
                        {
                            map[(int)k][(int)h].unitonMineon&=254; //sets rightmost bit to 0, leaves the rest alon
                            map[(int)k][(int)h].unitindex=0;
                            map[(int)k][(int)h].unitplayer=0;
                        }
                    }
                    if(what->selected==true) //deselect is required
                    {
                        for(unsigned int i=0; i<selectedunits[what->player].size(); i++)
                        {
                            if(selectedunits[what->player][i]==what->index)
                            {
                                selectedunits[what->player].erase(selectedunits[what->player].begin()+i);
                                break;
                            }
                        }
                        what->selected=false;
                    }
                    //attackingunitplayer=-1;
                    //attackingunitindex=-1;
                    return 1;
                }
            }
        }
        else //not in range
        {
            if(garrisoned<=-1)//garrisoned
            {
                //attackingunitindex=-1;
                //attackingunitplayer=-1;
                attackunitstance();
                return 2;
            }
            if(unitstance==US_DONTMOVE && userordered==false)
            {
                //attackingunitplayer=-1;
                //attackingunitindex=-1;
                return 2;
                //attackunitstance();
            }
            else if(unitstance==US_DEFENSIVE || unitstance==US_AGGRESIVE || userordered==true)
            {
                attackingwhat=true;
                attackmovement();
            }
        }
    }
    return 0;
}
int unit::innerFight(building &what)
{
    if(what.health<=0)
    {
        //attackingunitplayer=-1;
        //attackingunitindex=-1;
        return 1;
    }
	if(whatisit==2 && allsiegeunits[player][siegeindex].manning.size()<(unsigned int)allsiegeunits[player][siegeindex].minmanning)
		return 0; //not enough units manning the siege unit to do anything
	if(unitstance==US_DONOTHING && userordered==false)
		return 0;
	if(frames%attackspeed==0)
	{
        float enemyx=what.x;
        float enemyy=what.y;
        float enemyx2=what.x+what.width;
        float enemyy2=what.y+what.height;
        float dist=sqrt(pow((enemyx-x), 2)+pow((enemyy-y), 2));
        if(sqrt(pow((enemyx2-x), 2)+pow((enemyy-y), 2))<dist)
            dist=sqrt(pow((enemyx2-x), 2)+pow((enemyy-y), 2));
        if(sqrt(pow((enemyx2-x), 2)+pow((enemyy2-y), 2))<dist)
            dist=sqrt(pow((enemyx2-x), 2)+pow((enemyy2-y), 2));
        if(sqrt(pow((enemyx-x), 2)+pow((enemyy2-y), 2))<dist)
            dist=sqrt(pow((enemyx-x), 2)+pow((enemyy2-y), 2));
        short addAccuracy=0;
        short addAttack=0;
        short addRange=0;
        for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
        {
            if(allResearches[alreadyResearched[player][i]].checkResearch(id))
            {
                addAccuracy+=allResearches[alreadyResearched[player][i]].accuracy;
                addAttack+=allResearches[alreadyResearched[player][i]].buildingattack;
                addRange+=allResearches[alreadyResearched[player][i]].range;
            }
        }
        float actualrange=attackrange+addRange+0.05f;
        if(garrisoned<=-1)
        {
            if(attackrange!=MELEE)
                actualrange=allbuildings[player][(-garrisoned)-1].range+0.05f;
        }
        if(dist<=actualrange) //in range
        {
            float hpercent=(health/allbuildableunits[id].health)*100;
            float dmg=0;
            short addArmor=0;
            for(unsigned int i=0; i<alreadyResearched[what.player].size(); i++) //apply research bonuses
            {
                if(allResearches[alreadyResearched[what.player][i]].checkResearch(what.id, true))
                    addArmor+=allResearches[alreadyResearched[what.player][i]].armor;
            }
            if(hpercent==100)
                dmg=(float)buildingattack+addAttack-((what.beingbuilt>0) ? 0 : ((float)what.armor+addArmor));
            //for this formula and what it does, see the excel document called age of emps health percents 
            else if(hpercent<100 && hpercent>25)
                dmg=(buildingattack*.01f*((.0159f*hpercent*hpercent)-(3.2933f*hpercent)+171.81f))-((what.beingbuilt>0) ? 0 : (float)what.armor);
            if(dmg<=0) //if its <=25, will go here, dmg is 0
                return 0;
            if((rand()%100)>chanceHit+addAccuracy)
                return 0; //missed
            what.health-=(short)dmg;
            if(what.player==0 && what.selected==true)
                redraw=true;
            if(what.health<=0)
            {
                overwritebuildings[what.player].push_back(what.index);//it died, say its rewritable
                for(int g=0; g<4; g++)
                {
                    resources[what.player][g]-=what.holding[g];
                    map[(unsigned int)what.y][(unsigned int)what.x].resources[g]=what.holding[g];
                    what.holding[g]=0;
                    //CREATE BUILDING: PILE (JUST BUNCH OF RESOURCES) belongs to no one  (Not walkable)
                }
                for(float k=what.y; k<what.y+what.height; k+=.25)
                {
                    for(float h=what.x; h<what.x+what.width; h+=.25)
                    {
                        map[(int)k][(int)h].tilestyle=TS_GRASS;
                    }
                }
            }
        }
        else
        {
            if(unitstance==US_DONTMOVE && userordered==false)
            {
                //attackingunitplayer=-1;
                //attackingunitindex=-1;
                attackunitstance();
                return 2;
            }
            else if(unitstance==US_DEFENSIVE || unitstance==US_AGGRESIVE || userordered==true)
            {
                attackingwhat=true;
                attackmovement();
                return 0;
            }
		}
	}
    return 0;
}

vector <pointex2> unit::findallies(int foodneeded) //returns the index of the nearest unit of same player carrying food
{
	short cyclethrough=SEE_ALLIES;
    int uselos=los;
    for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
    {
        if(allResearches[alreadyResearched[player][i]].checkResearch(id))
            uselos+=allResearches[alreadyResearched[player][i]].los;
    }
	if(uselos<cyclethrough)
		cyclethrough=(short)uselos;
	vector <pointex2> allseenunits;
	for(int i=1; i<=cyclethrough; i++) //cycle through the horizontal diameter of the circle made by having a circle with center at the unit and radius LOS
	{
		if(x+i<map.size())
		{
			if((map[(unsigned int)y][(unsigned int)x+i].unitonMineon&1)==1)//if the space in question has a unit
			{
				if(map[(unsigned int)y][(unsigned int)x+i].unitplayer==player) //if this object is the players
				{
					if(i<=uselos-tilecameo[map[(unsigned int)y][(unsigned int)x+i].tilestyle]) //if we can actually see the allied unit, after camouflage from it and the map
						allseenunits.push_back(pointex2(map[(unsigned int)y][(unsigned int)x+i].unitindex, i, allunits[player][map[(unsigned int)y][(unsigned int)x+i].unitindex]->holding[0])); //record its position and that its a unit(true)
				}
			}
		}
		if(y+i<map.size())
		{
			if((map[(unsigned int)y+i][(unsigned int)x].unitonMineon&1)==1)//if the space in question has a unit
			{
				if(map[(unsigned int)y+i][(unsigned int)x].unitplayer==player) //if this object is the players
				{
					if(i<=uselos-tilecameo[map[(unsigned int)y+i][(unsigned int)x].tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
						allseenunits.push_back(pointex2(map[(unsigned int)y+i][(unsigned int)x].unitindex, i, allunits[player][map[(unsigned int)y+i][(unsigned int)x].unitindex]->holding[0])); //record its position and that its a unit(true)
				}
			}
		}
		if(x-i>=0)
		{
			if((map[(unsigned int)y][(unsigned int)x-i].unitonMineon&1)==1)//if the space in question has a unit
			{
				if(map[(unsigned int)y][(unsigned int)x-i].unitplayer==player) //if this object is the players
				{
					if(i<=uselos-tilecameo[map[(unsigned int)y][(unsigned int)x-i].tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
						allseenunits.push_back(pointex2(map[(unsigned int)y][(unsigned int)x-i].unitindex, i, allunits[player][map[(unsigned int)y][(unsigned int)x-i].unitindex]->holding[0])); //record its position and that its a unit(true)
				}
			}
		}
		if(y-i>=0)
		{
			if((map[(unsigned int)y-i][(unsigned int)x].unitonMineon&1)==1)//if the space in question has a unit
			{
				if(map[(unsigned int)y-i][(unsigned int)x].unitplayer==player) //if this object is the players
				{
					if(i<=uselos-tilecameo[map[(unsigned int)y-i][(unsigned int)x].tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
						allseenunits.push_back(pointex2(map[(unsigned int)y-1][(unsigned int)x].unitindex, i, allunits[player][map[(unsigned int)y-i][(unsigned int)x].unitindex]->holding[0])); //record its position and that its a unit(true)
				}
			}
		}
		for(int j=1; j<cyclethrough; j++)//loop through the vertical diameter of the circle
		{   //if the point that is the intersection of the line made by drawing a vertical line through the point on the horizontal diameter(i) and a horizontal one through the point on the vertical diameter(j)
			double dist=sqrt(double(i*i)+double(j*j)); //get the distance from the unit to the tile in question
			if(dist<=cyclethrough)
			{
				if(y+j<map.size())
				{
					if(x+i<map.size())
					{
						if((map[(unsigned int)y+j][(unsigned int)x+i].unitonMineon&1)==1)//if the space in question has a unit
						{
							if(map[(unsigned int)y+j][(unsigned int)x+i].unitplayer==player) //if this object is the players
							{
								if(dist<=uselos-tilecameo[map[(unsigned int)y+j][(unsigned int)x+i].tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
									allseenunits.push_back(pointex2(map[(unsigned int)y+j][(unsigned int)x+i].unitindex, dist, allunits[player][map[(unsigned int)y+j][(unsigned int)x+i].unitindex]->holding[0])); //record its position and that its a unit(true)
							}
						}
					}
					if(x-i>=0)
					{
						if((map[(unsigned int)y+j][(unsigned int)x-i].unitonMineon&1)==1)//if the space in question has a unit
						{
							if(map[(unsigned int)y+j][(unsigned int)x-i].unitplayer==player) //if this object is the players
							{
								if(dist<=uselos-tilecameo[map[(unsigned int)y+j][(unsigned int)x-i].tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
									allseenunits.push_back(pointex2(map[(unsigned int)y+j][(unsigned int)x-i].unitindex, dist, allunits[player][map[(unsigned int)y+j][(unsigned int)x-i].unitindex]->holding[0])); //record its position and that its a unit(true)
							}
						}
					}
				}
				if(y-j>=0)
				{
					if(x+i<map.size())
					{
						if((map[(unsigned int)y-j][(unsigned int)x+i].unitonMineon&1)==1)//if the space in question has a unit
						{
							if(map[(unsigned int)y-j][(unsigned int)x+i].unitplayer==player) //if this object is the players
							{
								if(dist<=uselos-tilecameo[map[(unsigned int)y-j][(unsigned int)x+i].tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
									allseenunits.push_back(pointex2(map[(unsigned int)y-j][(unsigned int)x+i].unitindex, dist, allunits[player][map[(unsigned int)y-j][(unsigned int)x+i].unitindex]->holding[0])); //record its position and that its a unit(true)
							}
						}
					}
					if(x-i>=0 && y-j>=0)
					{
						if((map[(unsigned int)y-j][(unsigned int)x-i].unitonMineon&1)==1)//if the space in question has a unit
						{
							if(map[(unsigned int)y-j][(unsigned int)x-i].unitplayer==player) //if this object is the players
							{
								if(dist<=uselos-tilecameo[map[(unsigned int)y-j][(unsigned int)x-i].tilestyle]) //if we can actually see the enemy unit, after camouflage from it and the map
									allseenunits.push_back(pointex2(map[(unsigned int)y-j][(unsigned int)x-i].unitindex, dist, allunits[player][map[(unsigned int)y-j][(unsigned int)x-i].unitindex]->holding[0])); //record its position and that its a unit(true)
							}
						}
					}
				}
			}
		}
	}
	quicksort(allseenunits, 0, allseenunits.size()-1);
	return allseenunits;
}
point unit::checksurroundingarea(short cwidth, short cheight)
{
	int checkwidth=1;
	while(true)
	{
		int i=(int)x-checkwidth; //left of building
		int j=0;
		int numbad=0;
		if(i>=0 && i<MAPSIZE)
		{
			for(j=(int)y-checkwidth; j<(int)y+(int)height+checkwidth; j++)
			{
				if(j>=MAPSIZE || j<0 || i>=MAPSIZE || i<0)
					break;
				bool metagood=true;
				for(int k=0; k<cwidth; k++)
				{
					for(int h=0; h<cheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=::chkmvp1(map[j+h][i+k],player,-1,0);//-1 is a hack, but a functional one, as is the 0
						if(good==false)
						{
							metagood=false;
							break;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		j=(int)y-checkwidth;
		if(j>=0 && j<MAPSIZE)
		{
			for(i=(int)x; i<(int)x+(int)width; i++)
			{
				bool metagood=true;
				for(int k=0; k<cwidth; k++)
				{
					for(int h=0; h<cheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=::chkmvp1(map[j+h][i+k],player,-1,0);//-1 is a hack, but a functional one
						if(good==false)
						{
							metagood=false;
							break;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		i=(int)x+(int)width+checkwidth;
		if(i>=0 && i<MAPSIZE)
		{
			for(j=(int)y-checkwidth; j<(int)y+(int)height+checkwidth; j++)
			{
				bool metagood=true;
				for(int k=0; k<cwidth; k++)
				{
					for(int h=0; h<cheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=::chkmvp1(map[j+h][i+k],player,-1,0);//-1 is a hack, but a functional one
						if(good==false)
						{
							metagood=false;
							break;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		j=(int)y+(int)height+checkwidth;
		if(j>=0 && j<MAPSIZE)
		{
			for(i=(int)x; i<(int)x+(int)width; i++)
			{
				bool metagood=true;
				for(int k=0; k<cwidth; k++)
				{
					for(int h=0; h<cheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=::chkmvp1(map[j+h][i+k],player,-1,0);//-1 is a hack, but a functional one
						if(good==false)
						{
							metagood=false;
							break;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		if(numbad==4)
			return point(-1.0f,-1.0f); //failed
		checkwidth++;
	}
}
void unit::searchresources()
{
	double mindist=99999999;
	int minx=-1;
	int miny=-1;
	for(int i=-3; i<=3; i++)
	{
		for(int j=-3; j<=3; j++)
		{
			double dist=99999999;
			if(map[(int)(gatheringy+i)][(int)(gatheringx+j)].tilestyle==gatheringwhat && (dist=sqrt(double(j*j+i*i)))<mindist)
			{
				mindist=dist;
				minx=j;
				miny=i;
			}
		}
	}
	gatheringx+=minx;
	gatheringy+=miny;
}
bool unit::distToBuilding(int bindex, double dist)
{
//    if(pow(x-allbuildings[player][bindex].x,2)+pow(y-allbuildings[player][bindex].y,2)<dist*dist)
//        return true;
//    if(pow(x-allbuildings[player][bindex].x+allbuildings[player][bindex].width,2)+pow(y-allbuildings[player][bindex].y,2)<dist*dist)
//        return true;
//    if(pow(x-allbuildings[player][bindex].x+allbuildings[player][bindex].width,2)+pow(y-allbuildings[player][bindex].y+allbuildings[player][bindex].height,2)<dist*dist)
//        return true;
//    if(pow(x-allbuildings[player][bindex].x+allbuildings[player][bindex].height,2)+pow(y-allbuildings[player][bindex].y+allbuildings[player][bindex].height,2)<dist*dist)
   double t1=(pow(x-(allbuildings[player][bindex].x+(allbuildings[player][bindex].width/2)),2))/(pow(allbuildings[player][bindex].width+dist,2));
   double t2=(pow(y-(allbuildings[player][bindex].y+(allbuildings[player][bindex].height/2)),2))/(pow(allbuildings[player][bindex].height+dist,2));
   if((t1+t2)<1)
       return true;
   return false;
}
point unit::searchbuildingresources(short buildwhat)
{
    int takeresourcesfrom=-1;
    for(int side=1; side<30; side++) //side length
    {
        for(int i=(int)x-side; i<=(int)x+side; i++) //loop through width of square
        {
            if((takeresourcesfrom=map[(int)y-side][i].inbuildingrad)!=-1) //in a buildings take resources zone
            {
                if(map[(int)y-side][i].whichplayer.get(false)==player) //my players building
                {
                    if(allbuildings[player][takeresourcesfrom].holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allbuildings[player][takeresourcesfrom].holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allbuildings[player][takeresourcesfrom].holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allbuildings[player][takeresourcesfrom].holding[3]>=allbuildablebuildings[buildwhat].stonetobuild) //if that building has enough resources
                    {
                        return point(i,(int)y-side);
                    }
                }
            }
            if((map[(int)y-side][i].unitonMineon&1)==1 && map[(int)y-side][i].unitplayer==player && (takeresourcesfrom=map[(int)y-side][i].unitindex)!=-1) //near unit that can supply the resources
            {
                if(allunits[player][takeresourcesfrom]->holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allunits[player][takeresourcesfrom]->holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allunits[player][takeresourcesfrom]->holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allunits[player][takeresourcesfrom]->holding[3]>=allbuildablebuildings[buildwhat].stonetobuild)
                {
                    buildingResourcesFrom=takeresourcesfrom;
                    return point(i+1, (int)y-side);
                }
            }
            if((takeresourcesfrom=map[(int)y+side][i].inbuildingrad)!=-1) //in a buildings take resources zone
            {
                if(map[(int)y+side][i].whichplayer.get(false)==player) //my players building
                {
                    if(allbuildings[player][takeresourcesfrom].holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allbuildings[player][takeresourcesfrom].holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allbuildings[player][takeresourcesfrom].holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allbuildings[player][takeresourcesfrom].holding[3]>=allbuildablebuildings[buildwhat].stonetobuild) //if that building has enough resources
                    {
                        return point(i,(int)y+side);
                    }
                }
            }
            if((map[(int)y+side][i].unitonMineon&1)==1 && map[(int)y+side][i].unitplayer==player && (takeresourcesfrom=map[(int)y+side][i].unitindex)!=-1) //near unit that can supply the resources
            {
                if(allunits[player][takeresourcesfrom]->holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allunits[player][takeresourcesfrom]->holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allunits[player][takeresourcesfrom]->holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allunits[player][takeresourcesfrom]->holding[3]>=allbuildablebuildings[buildwhat].stonetobuild)
                {
                    buildingResourcesFrom=takeresourcesfrom;
                    return point(i+1, (int)y+side);
                }
            }
        }
        
        for(int i=(int)y-side+1; i<=(int)y+side-1; i++)
        {
            if((takeresourcesfrom=map[i][(int)x-side].inbuildingrad)!=-1) //in a buildings take resources zone
            {
                if(map[i][(int)x-side].whichplayer.get(false)==player) //my players building
                {
                    if(allbuildings[player][takeresourcesfrom].holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allbuildings[player][takeresourcesfrom].holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allbuildings[player][takeresourcesfrom].holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allbuildings[player][takeresourcesfrom].holding[3]>=allbuildablebuildings[buildwhat].stonetobuild) //if that building has enough resources
                    {
                        return point((int)x-side,i);
                    }
                }
            }
            if((map[i][(int)x-side].unitonMineon&1)==1 && map[i][(int)x-side].unitplayer==player && (takeresourcesfrom=map[i][(int)x-side].unitindex)!=-1) //near unit that can supply the resources
            {
                if(allunits[player][takeresourcesfrom]->holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allunits[player][takeresourcesfrom]->holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allunits[player][takeresourcesfrom]->holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allunits[player][takeresourcesfrom]->holding[3]>=allbuildablebuildings[buildwhat].stonetobuild)
                {
                    buildingResourcesFrom=takeresourcesfrom;
                    return point((int)x-side, i-1);
                }
            }
            if((takeresourcesfrom=map[i][(int)x+side].inbuildingrad)!=-1) //in a buildings take resources zone
            {
                if(map[i][(int)x+side].whichplayer.get(false)==player) //my players building
                {
                    if(allbuildings[player][takeresourcesfrom].holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allbuildings[player][takeresourcesfrom].holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allbuildings[player][takeresourcesfrom].holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allbuildings[player][takeresourcesfrom].holding[3]>=allbuildablebuildings[buildwhat].stonetobuild) //if that building has enough resources
                    {
                        return point((int)x+side,i);
                    }
                }
            }
            if((map[i][(int)x+side].unitonMineon&1)==1 && map[i][(int)x+side].unitplayer==player && (takeresourcesfrom=map[i][(int)x+side].unitindex)!=-1) //near unit that can supply the resources
            {
                if(allunits[player][takeresourcesfrom]->holding[0]>=allbuildablebuildings[buildwhat].foodtobuild && allunits[player][takeresourcesfrom]->holding[1]>=allbuildablebuildings[buildwhat].woodtobuild && allunits[player][takeresourcesfrom]->holding[2]>=allbuildablebuildings[buildwhat].goldtobuild && allunits[player][takeresourcesfrom]->holding[3]>=allbuildablebuildings[buildwhat].stonetobuild)
                {
                    buildingResourcesFrom=takeresourcesfrom;
                    return point((int)x+side, i-1);
                }
            }
        }
    }
    return point(-1,-1);
}