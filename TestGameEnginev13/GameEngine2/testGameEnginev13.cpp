#include "globals.h"
#include "dataStructures.h"
#include "tile.h"
#include "baseUnitBuildings2.h"
#include "building.h"
#include "unit.h"
#include "siege.h"
#include "regiment.h"
#include "button.h"
#include "astarsearch.h"
#include "buttonFunctions.h"
#include "displayFunctions.h"
#include "mouseOverFunctions.h"
#include "GUIAndInput.h"
#include "progressBarFunctions.h"
#include "research.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <fstream>
#include <string.h>
#include <GL/glut.h>

using namespace std;

//forward declarations of classes
class creationinfo;
class buildingcreateinfo;
class unit;
class building;
class basebuilding;
class basicbuilding;
class tile;
class point;
class obstaclethreadparam;
class obstaclethreadret;
class pointex;
class pointex2;
class basicunit;
class siege;
class regiment;
class button;

vector<point> astarsearch(problem p, float speed, unit *u)
{
	vector<searchfringe> fringe; //poss make this a heap
	vector<point> empty;
	fringe.push_back(searchfringe(point(u->x,u->y),0,empty));
	vector<point> visited; //make this a hashtable
	unsigned int maxsize=50;
	if(u->firstobstacleattempt==true)
		maxsize=1000;
	while(true)
	{
		if(fringe.size()>=maxsize)//taking too long
			return empty;
		int minindex=-1;
		while(true)
		{
			minindex=-1;
			float min=9999999;
			for(unsigned int i=0; i<fringe.size(); i++)//get min
			{
				if(fringe[i].weighttodate<min)
				{
					min=fringe[i].weighttodate;
					minindex=i;
				}
			}
			if(minindex==-1)//empty fringe
				return empty;
			bool replaced=false;
			for(unsigned int i=0; i<visited.size(); i++)
			{
				if(fringe[minindex].state==visited[i])
				{
					fringe.erase(fringe.begin()+minindex);
					replaced=true;
					break;
				}
			}
			if(replaced==true)
				continue;
			break;
		}
		if(minindex==-1)
		{
			vector<point> temp;
			return temp;
		}
		if(p.goal(fringe[minindex].state)==true)
			return fringe[minindex].movestodate;
		vector <point> add;
		vector <point> moves;
		p.getsuccessors(fringe[minindex].state, add, moves, speed, u);
		vector <float> weights;
		visited.push_back(fringe[minindex].state);
		for(unsigned int i=0; i<add.size(); i++)
		{
			weights.push_back(sqrt(pow(moves[i].x,2)+pow(moves[i].y,2)));
			weights[i]+=p.heuristic(add[i]);
			vector <point> temp=fringe[minindex].movestodate;
			temp.push_back(moves[i]);
			fringe.push_back(searchfringe(add[i],fringe[minindex].weighttodate+weights[i],temp));
		}
		fringe.erase(fringe.begin()+minindex);
	}
}
//VOID CALLBACK hourpassed(HWND hWnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
void hourpassed(int arg)
{
	hours+=hourspersecond;//goes off every 1 sec.
	if((int)hours%24==0 && abs((long double)(hours-int(hours+0.00001)))<=0.0001) //one day passed
	{
		for(unsigned int i=0; i<allunits.size(); i++)//loops through all the players
		{
			for(unsigned int j=0; j<allunits[i].size(); j++)//loops through the units of each player
			{
				if(allunits[i][j]->health<=0)
					continue;
				if(allunits[i][j]->timeslept<allunits[i][j]->sleepneeded)//didn't get enough sleep
					allunits[i][j]->moral-=((allunits[i][j]->sleepneeded-allunits[i][j]->timeslept)/4); //so moral goes down just a little
				allunits[i][j]->timeslept=0; //reset timeslept, since it must be daily
				int amntleft=allunits[i][j]->foodconsumed;
				if(allunits[i][j]->holding[0]>=amntleft)
				{
					allunits[i][j]->holding[0]-=amntleft;
					amntleft=0;
				}
				else
				{
					amntleft-=allunits[i][j]->holding[0];
					allunits[i][j]->holding[0]=0;
				}
				if(amntleft>0)
				{
					if(map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].inbuildingrad!=-1 && map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].whichplayer.get(false)==i)
					{
						if(allbuildings[i][map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].inbuildingrad].holding[0]>=amntleft)
						{
							resources[i][0]-=amntleft;
							allbuildings[i][map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].inbuildingrad].holding[0]-=amntleft;
							amntleft=0;
						}
						else
						{
							resources[i][0]-=allbuildings[i][map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].inbuildingrad].holding[0];
							amntleft-=allbuildings[i][map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].inbuildingrad].holding[0];
							allbuildings[i][map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].inbuildingrad].holding[0]=0;
						}
					}
				}
				if(amntleft>0)
				{
					vector <pointex2> availunits=allunits[i][j]->findallies(amntleft);
					for(unsigned int k=0; k<availunits.size(); k++)
					{
						if(((unsigned int)availunits[k].index<j && availunits[k].food>=amntleft) || ((unsigned int)availunits[k].index>j && availunits[k].food>=amntleft+allunits[i][availunits[k].index]->foodconsumed))
						{
							allunits[i][availunits[k].index]->holding[0]-=amntleft;
							amntleft=0;
							break;
						}
						else if((unsigned int)availunits[k].index<j && availunits[k].food<amntleft)
						{
							amntleft-=availunits[k].food;
							allunits[i][availunits[k].index]->holding[0]=0;
						}
						else if((unsigned int)availunits[k].index>j && availunits[k].food<amntleft+allunits[i][availunits[k].index]->foodconsumed && availunits[k].food>allunits[i][availunits[k].index]->foodconsumed)
						{
							amntleft-=availunits[k].food-allunits[i][availunits[k].index]->foodconsumed;
							allunits[i][availunits[k].index]->holding[0]=allunits[i][availunits[k].index]->foodconsumed;
						}
					}
					allunits[i][j]->moral-=(float)amntleft/2.0f;   //subtract whats left from moral and health
					allunits[i][j]->health-=(float)amntleft/8.0f;
					if(allunits[i][j]->health<=0)
					{
						for(int g=0; g<4; g++)
						{
							map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].resources[g]=allunits[i][j]->holding[g];
							allunits[i][j]->holding[g]=0;
							//CREATE BUILDING: PILE (JUST BUNCH OF RESOURCES) belongs to no one  (Not walkable)
							overwriteunits[i].push_back(j);//it died, say its rewritable
							for(float k=allunits[i][j]->y; k<allunits[i][j]->y+allunits[i][j]->height; k+=.25)
							{
								for(float h=allunits[i][j]->x; h<allunits[i][j]->x+allunits[i][j]->width; h+=.25)
								{
									map[(int)k][(int)h].unitonMineon&=254; //marks it dead: unsigned char & 254 makes the rightmost bit a 0, but leavees the rest the same
									map[(int)k][(int)h].unitindex=0;
									map[(int)k][(int)h].unitplayer=0;
								}
							}
						}
					}
				}
			}
			//if(resources[0][i]>foodneeded)//if there's enough food
			//	resources[0][i]-=foodneeded;//the food was eaten
			//else
			//{
			//	double dif=foodneeded-resources[0][i];//else, dif is what we need
			//	resources[0][i]=0;//amnt food owned is 0
			//	dif/=allunits[i].size(); //how much food each unit will be missing, on average
			//	for(unsigned int k=0; k<allunits[i].size(); k++)
			//	{
			//		allunits[i][k]->moral-=float(dif/2);//subtract the difference from moral and health
			//		allunits[i][k]->health-=float(dif/8);
			//	}
			//}
		}
	}
	glutTimerFunc(1000,hourpassed,0);
}
void selectone(int player, point &clicked) //select one unit, no shift key pressed, if its a siege unit, select everyone manning it as well
{
	if(player==0)
		redraw=1;//mod buttons likely
    allgarrisonedselectedunits[player].clear(); //no people inside buildings selected anymore
	if(sqrt(pow(clicked.x-allunits[player][generals[player]]->x,2)+pow(clicked.y-allunits[player][generals[player]]->y,2))>allunits[player][generals[player]]->los) //out of range
	{
		if(whatsselected[player]==true)
		{
			for(unsigned int i=0; i<selectedunits[player].size(); i++)
				allunits[player][selectedunits[player][i]]->selected=false;
		}
		else
		{
			for(unsigned int i=0; i<selectedunits[player].size(); i++)
				allbuildings[player][selectedunits[player][i]].selected=false;
		}
		selectedunits[player].clear();
	}
	else if((map[(int)clicked.y][(int)clicked.x].unitonMineon&1)==1) //there is a unit on it
	{
		if(clicked.y<allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->boundingbox.bottom && clicked.y>allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->boundingbox.top && clicked.x>allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->boundingbox.left && clicked.x<allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->boundingbox.right) //if the user clicked on the unit (within the 2d bouding box)
		{
			if(player==map[(int)clicked.y][(int)clicked.x].unitplayer)
			{
				//if(allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->selected==true)
				if(allunits[player][map[(int)clicked.y][(int)clicked.x].unitindex]->selected==true)
				{
					for(unsigned int i=0; i<selectedunits[player].size(); i++)
					{
						if(whatsselected[player]==true)
							allunits[player][selectedunits[player][i]]->selected=false;
						else
							allbuildings[player][selectedunits[player][i]].selected=false;
					}
					selectedunits[player].clear();
				}
				else
				{
					if(allunits[player][map[(int)clicked.y][(int)clicked.x].unitindex]->health<=0)
						return;
					allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->selected=true;
					for(unsigned int i=0; i<selectedunits[player].size(); i++)
					{
						if(whatsselected[player]==true)
							allunits[player][selectedunits[player][i]]->selected=false;
						else
							allbuildings[player][selectedunits[player][i]].selected=false;
					}
					selectedunits[player].clear();
					whatsselected[player]=true;
					selectedunits[player].push_back(map[(int)clicked.y][(int)clicked.x].unitindex);
					if(allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->whatisit==2) //if its a siege unit
					{
						for(unsigned int i=0; i<allsiegeunits[player][allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->siegeindex].manning.size(); i++)
						{
							allunits[player][allsiegeunits[player][allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->siegeindex].manning[i]]->selected=true;
							selectedunits[player].push_back(allsiegeunits[player][allunits[map[(int)clicked.y][(int)clicked.x].unitplayer][map[(int)clicked.y][(int)clicked.x].unitindex]->siegeindex].manning[i]);
						}
					}
				}
			}
			else
			{
				for(unsigned int i=0; i<selectedunits[player].size(); i++)
				{
					if(whatsselected[player]==true)
						allunits[player][selectedunits[player][i]]->selected=false;
					else
						allbuildings[player][selectedunits[player][i]].selected=false;
				}
				selectedunits[player].clear();
				//some sort of show info thing. for enemy units
			}
		}
	}
	else if(map[(int)clicked.y][(int)clicked.x].tilestyle==TS_BUILDING || map[(int)clicked.y][(int)clicked.x].tilestyle==TS_WATERBUILDING) //selecting a building
	{
		if(clicked.y<allbuildings[map[(int)clicked.y][(int)clicked.x].buildingplayer][map[(int)clicked.y][(int)clicked.x].buildingindex].boundingbox.bottom && clicked.y>allbuildings[map[(int)clicked.y][(int)clicked.x].buildingplayer][map[(int)clicked.y][(int)clicked.x].buildingindex].boundingbox.top && clicked.x>allbuildings[map[(int)clicked.y][(int)clicked.x].buildingplayer][map[(int)clicked.y][(int)clicked.x].buildingindex].boundingbox.left && clicked.x<allbuildings[map[(int)clicked.y][(int)clicked.x].buildingplayer][map[(int)clicked.y][(int)clicked.x].buildingindex].boundingbox.right) //if the user clicked on the building (within the 2d bouding box)
		{
			if(player==map[(int)clicked.y][(int)clicked.x].buildingplayer)
			{
				if(allbuildings[map[(int)clicked.y][(int)clicked.x].buildingplayer][map[(int)clicked.y][(int)clicked.x].buildingindex].selected==true)
				{
					allbuildings[map[(int)clicked.y][(int)clicked.x].buildingplayer][map[(int)clicked.y][(int)clicked.x].buildingindex].selected=false;
					selectedunits[player].clear();
				}
				else
				{
					if(allbuildings[player][map[(int)clicked.y][(int)clicked.x].buildingindex].health<0)
						return;
					allbuildings[map[(int)clicked.y][(int)clicked.x].buildingplayer][map[(int)clicked.y][(int)clicked.x].buildingindex].selected=true;
					for(unsigned int i=0; i<selectedunits[player].size(); i++)
					{
						if(whatsselected[player]==true)
							allunits[player][selectedunits[player][i]]->selected=false;
						else
							allbuildings[player][selectedunits[player][i]].selected=false;
					}
					selectedunits[player].clear();
					whatsselected[player]=false;
					selectedunits[player].push_back(map[(int)clicked.y][(int)clicked.x].buildingindex);
				}
			}
			else
			{
				for(unsigned int i=0; i<selectedunits[player].size(); i++)
				{
					if(whatsselected[player]==true)
						allunits[player][selectedunits[player][i]]->selected=false;
					else
						allbuildings[player][selectedunits[player][i]].selected=false;
				}
				selectedunits[player].clear();
				//some sort of show info thing for enemy building
			}
		}
	}
	else
	{
		if(whatsselected[player]==true)
		{
			for(unsigned int i=0; i<selectedunits[player].size(); i++)
				allunits[player][selectedunits[player][i]]->selected=false;
		}
		else
		{
			for(unsigned int i=0; i<selectedunits[player].size(); i++)
				allbuildings[player][selectedunits[player][i]].selected=false;
		}
		selectedunits[player].clear();
	}
}
void selectmany(int player, myrect &clicked)//select all the units in a box made by clicking and holding and dragging
{
	if(player==0)
		redraw=1; //mod buttons likely
    allgarrisonedselectedunits[player].clear(); //no people inside buildings selected anymore
	if(whatsselected[player]==true)
	{
		for(unsigned int i=0; i<selectedunits[player].size(); i++)
			allunits[player][selectedunits[0][i]]->selected=false;
	}
	else
	{
		for(unsigned int i=0; i<selectedunits[player].size(); i++)
			allbuildings[player][selectedunits[0][i]].selected=false;
	}
	selectedunits[player].clear();
	if(sqrt(pow(clicked.left-allunits[player][generals[player]]->x,2)+pow(clicked.top-allunits[player][generals[player]]->y,2))>allunits[player][generals[player]]->los || sqrt(pow(clicked.right-allunits[player][generals[player]]->x,2)+pow(clicked.bottom-allunits[player][generals[player]]->y,2))>allunits[player][generals[player]]->los) //out of range
	{
		return;
	}
	for(unsigned int j=0; j<allunits[player].size(); j++) //loop through all of the players units
	{
		if(allunits[player][j]->health<=0)
			continue;
		if(clicked.bottom>allunits[player][j]->boundingbox.bottom && clicked.top<allunits[player][j]->boundingbox.top && clicked.left<allunits[player][j]->boundingbox.left && clicked.right>allunits[player][j]->boundingbox.right) // if the unit is in the box
		{
			allunits[player][j]->selected=true;
			selectedunits[player].push_back(j);
			whatsselected[player]=true;
			if(allunits[player][j]->whatisit==2) //if its a siege unit
			{
				for(unsigned int i=0; i<allsiegeunits[player][allunits[player][j]->siegeindex].manning.size(); i++)
				{
					if(allunits[player][allsiegeunits[player][allunits[player][j]->siegeindex].manning[i]]->selected!=true)
					{
						allunits[player][allsiegeunits[player][allunits[player][j]->siegeindex].manning[i]]->selected=true;
						selectedunits[player].push_back(allsiegeunits[player][allunits[player][j]->siegeindex].manning[i]);
					}
				}
			}
		}
	}
}
void move(int player, POINT &gowhere, bool waypoint)
{
	if(whatsselected[player]==true)
	{
		for(unsigned int i=0; i<selectedunits[player].size(); i++)//loop through all of the players selected units
		{
			if(allunits[player][selectedunits[player][i]]->whatisit!=2 && allunits[player][selectedunits[player][i]]->siegeindex!=-1)
				continue;
			if(allunits[player][selectedunits[player][i]]->health<=0) //if its dead, it shouldn't be allowed to move
				continue;
			if(waypoint==false)
			{
				allobstacles[player][selectedunits[player][i]].clear();
				//if(!calcmove(allunits[player][selectedunits[player][i]],point((float)gowhere.x,(float)gowhere.y))) //try to calculate the whole move. if failed:
				{
					allunits[player][selectedunits[player][i]]->movetox=(float)gowhere.x; //tell it to go to the passed point without precalculation
					allunits[player][selectedunits[player][i]]->movetoy=(float)gowhere.y;
				}
				/*else //all good, clear what its doing now
				{
					allunits[player][selectedunits[player][i]]->movetox=allunits[player][selectedunits[player][i]]->x;
					allunits[player][selectedunits[player][i]]->movetoy=allunits[player][selectedunits[player][i]]->y;
				}*/
			}
			else //add it as a waypoint instead //NOTE: waypoints are almost certain not to work for siege units
			{
				if(allobstacles[player][selectedunits[player][i]].size()==0 && (allunits[player][selectedunits[player][i]]->movetox==allunits[player][selectedunits[player][i]]->x && allunits[player][selectedunits[player][i]]->movetoy==allunits[player][selectedunits[player][i]]->y)) //no other waypoints/commands and standing still
					allobstacles[player][selectedunits[player][i]].push_back(point(gowhere.x-allunits[player][selectedunits[player][i]]->x,gowhere.y-allunits[player][selectedunits[player][i]]->y));
				else if(allobstacles[player][selectedunits[player][i]].size()==0 && !(allunits[player][selectedunits[player][i]]->movetox==allunits[player][selectedunits[player][i]]->x && allunits[player][selectedunits[player][i]]->movetoy==allunits[player][selectedunits[player][i]]->y)) //no other waypoints/command, and moving
					allobstacles[player][selectedunits[player][i]].push_back(point(gowhere.x-allunits[player][selectedunits[player][i]]->movetox,gowhere.y-allunits[player][selectedunits[player][i]]->movetoy));
				else if(allobstacles[player][selectedunits[player][i]].size()!=0)//other waypoints/commands
				{
					float sumx=0;
					float sumy=0;
					for(unsigned int k=0; k<allobstacles[player][selectedunits[player][i]].size(); k++)
					{
						sumx+=allobstacles[player][selectedunits[player][i]][k].x;
						sumy+=allobstacles[player][selectedunits[player][i]][k].y;
					}
					if(allunits[player][selectedunits[player][i]]->movetox==allunits[player][selectedunits[player][i]]->x && allunits[player][selectedunits[player][i]]->movetoy==allunits[player][selectedunits[player][i]]->y) //standing still
						allobstacles[player][selectedunits[player][i]].push_back(point(gowhere.x-(allunits[player][selectedunits[player][i]]->x+sumx),gowhere.y-(allunits[player][selectedunits[player][i]]->y+sumy)));
					else //moving
						allobstacles[player][selectedunits[player][i]].push_back(point(gowhere.x-(allunits[player][selectedunits[player][i]]->movetox+sumx),gowhere.y-(allunits[player][selectedunits[player][i]]->movetoy+sumy)));
				}
			}
			allunits[player][selectedunits[player][i]]->userordered=true;
			allunits[player][selectedunits[player][i]]->dstancecoox=-1;
			allunits[player][selectedunits[player][i]]->dstancecooy=-1;
			allunits[player][selectedunits[player][i]]->gatheringx=-1;
			allunits[player][selectedunits[player][i]]->gatheringy=-1;
			allunits[player][selectedunits[player][i]]->gatheringwhat=-1;
			allunits[player][selectedunits[player][i]]->firstobstacleattempt=true; // at this point, definitely haven't tried to avoid an obstacle
			if((map[(int)gowhere.y][(int)gowhere.x].unitonMineon&1)==1) //if there is a unit on the square where we are going
			{
				if(gowhere.x>allunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][map[(int)gowhere.y][(int)gowhere.x].unitindex]->boundingbox.left && gowhere.x<allunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][map[(int)gowhere.y][(int)gowhere.x].unitindex]->boundingbox.right && gowhere.y>allunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][map[(int)gowhere.y][(int)gowhere.x].unitindex]->boundingbox.top && gowhere.y<allunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][map[(int)gowhere.y][(int)gowhere.x].unitindex]->boundingbox.bottom) //if the current unit is going to be attacked
				{
					if(map[(int)gowhere.y][(int)gowhere.x].unitplayer!=players[player])
					{
						if(allunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][map[(int)gowhere.y][(int)gowhere.x].unitindex]->whatisit==2)//its a siege unit
						{
							if(allsiegeunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][allunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size()==0)
							{ //its unmanned
								//make this push back a new unit into whatever stores the actual units for the current player, making sure the new unit has the same exact stats as this one. xp, loc etc. then 'kill this unit'
								//in effect, once this is done, the siege unit has been 'captured' by the attacking player.
								//then say that they are manning the siege unit.
								//Call capture() ????
							}
							else
							{
								for(unsigned int j=0; j<allsiegeunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][allunits[map[(int)gowhere.y][(int)gowhere.x].unitplayer][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size(); j++)
								{
									//same as above, BUT says if they are out of LOS range, then capture, else can't, must attack
								}
							}
						}
						allunits[player][selectedunits[player][i]]->attackingunitplayer=map[(int)gowhere.y][(int)gowhere.x].unitplayer; //record which unit is being attacked
						allunits[player][selectedunits[player][i]]->attackingunitindex=map[(int)gowhere.y][(int)gowhere.x].unitindex;
						allunits[player][selectedunits[player][i]]->attackingwhat=true;
						allunits[player][selectedunits[player][i]]->attackmovement(); //figure out how far to go to be in range
						continue;
					}
					else
					{
						if(player==map[(int)gowhere.y][(int)gowhere.x].unitplayer)
						{
							if(allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->whatisit==2) //going to my siege unit, so man it if neccesary
							{
								if(allunits[player][selectedunits[player][i]]->siegeindex==-1 && allunits[player][selectedunits[player][i]]->whatisit!=2 && allunits[player][selectedunits[player][i]]->whatisit!=3)//not already manning anything, and its not a siege unit, and its not a ship
								{
									if(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size()<(unsigned int)allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].maxmanning) //needs to be manned by more units
									{
										switch((allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size()%4)+1)
										{
										case 1: //first unit to man
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->x+(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->y-allunits[player][selectedunits[player][i]]->height;
											break;
										case 2:
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->x+allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->width-allunits[player][selectedunits[player][i]]->width-(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->y-allunits[player][selectedunits[player][i]]->height;
											break;
										case 3:
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->x+(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->y+allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->height;
											break;
										case 4:
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->x+allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->width-allunits[player][selectedunits[player][i]]->width-(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->y+allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->height;
											break;
										};
										allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex].manning.push_back(selectedunits[player][i]);
										allunits[player][selectedunits[player][i]]->siegeindex=allunits[player][map[(int)gowhere.y][(int)gowhere.x].unitindex]->siegeindex;
										continue;
									}
								}
							}
							/*else if(allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->whatisit==3) //going to my ship, so man it if neccesary //SHIPS HANDLED ELSEWHERE
							{
								if(allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->unitsinside.size()>allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->maxgarrison) //if can't man, don't, else just go to the ship
								{
									allunits[player][selectedunits[player][i]]->movetox=allunits[player][selectedunits[player][i]]->x;
									allunits[player][selectedunits[player][i]]->movetoy=allunits[player][selectedunits[player][i]]->y;
								}
							}*/
						}
					}
				}
			}
			else if(map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_BUILDING || map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_WATERBUILDING)
			{
				if(gowhere.x>allbuildings[map[(int)gowhere.y][(int)gowhere.x].buildingplayer][map[(int)gowhere.y][(int)gowhere.x].buildingindex].boundingbox.left && gowhere.x<allbuildings[map[(int)gowhere.y][(int)gowhere.x].buildingplayer][map[(int)gowhere.y][(int)gowhere.x].buildingindex].boundingbox.right && gowhere.y>allbuildings[map[(int)gowhere.y][(int)gowhere.x].buildingplayer][map[(int)gowhere.y][(int)gowhere.x].buildingindex].boundingbox.top && gowhere.y<allbuildings[map[(int)gowhere.y][(int)gowhere.x].buildingplayer][map[(int)gowhere.y][(int)gowhere.x].buildingindex].boundingbox.bottom) //if the current unit is going to be attacked
				{
					if(map[(int)gowhere.y][(int)gowhere.x].buildingplayer!=players[player])
					{
						if(allunits[player][selectedunits[player][i]]->buildingattack!=0)
						{
							allunits[player][selectedunits[player][i]]->attackingunitindex=map[(int)gowhere.y][(int)gowhere.x].buildingindex;
							allunits[player][selectedunits[player][i]]->attackingunitplayer=map[(int)gowhere.y][(int)gowhere.y].buildingplayer;
							allunits[player][selectedunits[player][i]]->attackingwhat=false;
							allunits[player][selectedunits[player][i]]->attackmovement();
							continue;
						}
					}
					else
					{
						if(allbuildings[player][map[(int)gowhere.y][(int)gowhere.x].buildingindex].garrison-allbuildings[player][map[(int)gowhere.y][(int)gowhere.x].buildingindex].garrisoned.size()>0 || allbuildings[player][map[(int)gowhere.y][(int)gowhere.x].buildingindex].beingbuilt>0)
							allunits[player][selectedunits[player][i]]->garrisoned=1+map[(int)gowhere.y][(int)gowhere.x].buildingindex;//garrisoning/building
					}
				}
			}
			else if(map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_TREE || map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_BERRIES || map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_ANIMAL || (map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_GOLD && (map[(int)gowhere.y][(int)gowhere.x].unitonMineon&2)==2) || (map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_STONE && (map[(int)gowhere.y][(int)gowhere.x].unitonMineon&2)==2))
			{
				allunits[player][selectedunits[player][i]]->gatheringx=(short)gowhere.x;
				allunits[player][selectedunits[player][i]]->gatheringy=(short)gowhere.y;
				allunits[player][selectedunits[player][i]]->gatheringwhat=map[(int)gowhere.y][(int)gowhere.x].tilestyle;
			}
			if(allunits[player][selectedunits[player][i]]->whatisit==2) //its a siege unit
			{
				for(unsigned int j=0; j<allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning.size(); j++)
				{
					switch((j%4)+1)
					{
					case 1:
						if(sqrt(pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->x-(allunits[player][selectedunits[player][i]]->x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->y-(allunits[player][selectedunits[player][i]]->y-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->height), 2))<=DIST_TO_MAN) //if the unit is with the proper distance of where it should be
						{
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetox=allunits[player][selectedunits[player][i]]->movetox+(int)(j/4);
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetoy=allunits[player][selectedunits[player][i]]->movetoy-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->height;
						}
						break;
					case 2:
						if(sqrt(pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->x-(allunits[player][selectedunits[player][i]]->x+allunits[player][selectedunits[player][i]]->width-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->y-(allunits[player][selectedunits[player][i]]->y-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->height),2))<=DIST_TO_MAN)
						{
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetox=allunits[player][selectedunits[player][i]]->movetox+allunits[player][selectedunits[player][i]]->width-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->width-(int)(j/4);
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetoy=allunits[player][selectedunits[player][i]]->movetoy-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->height;
						}
						break;
					case 3:
						if(sqrt(pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->x-(allunits[player][selectedunits[player][i]]->x+(int)(j/4)), 2)+pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->y-(allunits[player][selectedunits[player][i]]->y+allunits[player][selectedunits[player][i]]->height),2))<=DIST_TO_MAN)
						{
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetox=allunits[player][selectedunits[player][i]]->movetox+(int)(j/4);
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetoy=allunits[player][selectedunits[player][i]]->movetoy+allunits[player][selectedunits[player][i]]->height;
						}
						break;
					case 4:
						if(sqrt(pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->x-(allunits[player][selectedunits[player][i]]->width-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->width-(int)(j/4)),2)+pow(allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->y-(allunits[player][selectedunits[player][i]]->y+allunits[player][selectedunits[player][i]]->height),2))<=DIST_TO_MAN)
						{
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetox=allunits[player][selectedunits[player][i]]->movetox+allunits[player][selectedunits[player][i]]->width-allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->width-(int)(j/4);
							allunits[player][allsiegeunits[player][allunits[player][selectedunits[player][i]]->siegeindex].manning[j]]->movetox=allunits[player][selectedunits[player][i]]->movetoy+allunits[player][selectedunits[player][i]]->height;
						}
						break;
					}
				}
			}
		}
	}
}
void quicksort(vector <pointex2> &num, int top, int bottom)
{
	// top = subscript of beginning of array
	// bottom = subscript of end of array
	int middle;
	if (top < bottom)
	{
		middle = partition(num, top, bottom);
		quicksort(num, top, middle);   // sort first section
		quicksort(num, middle+1, bottom);    // sort second section
	}
	return;
}
int partition(vector <pointex2> &arr, int top, int bottom)
{
	int x = arr[top].food;
	int i = top - 1;
	int j = bottom + 1;
	int temp;
	do
	{
		do 
		{
			j--;
		}while(x>arr[j].food);
		do  
		{
			i++;
		}while(x<arr[i].food);
		if(i<j)
		{ 
			temp=arr[i].food;    
			arr[i].food=arr[j].food;
			arr[j].food=temp;
		}
	}while (i<j);     
	return j;           // returns middle subscript  
}
void capture(int player, point &clicked)
{
	if((unsigned int)clicked.x>=map.size() && (unsigned int)clicked.y>=map.size() && (unsigned int)clicked.x<0 && (unsigned int)clicked.y<0)
		return;
	if(selectedunits[player].size()==0)
		return;
	if(map[(unsigned int)clicked.y][(unsigned int)clicked.x].tilestyle==TS_BUILDING)
	{
		if(allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingplayer][map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingindex].garrisoned.size()==0)
		{
            int addLOS=0;
            for(unsigned int i=0; i<alreadyResearched[player].size(); i++) //apply research bonuses
            {
                if(allResearches[alreadyResearched[player][i]].checkResearch(allunits[player][selectedunits[player][0]]->id))
                    addLOS+=allResearches[alreadyResearched[player][i]].los;
            }
			if(sqrt(pow(allunits[player][selectedunits[player][0]]->x-(unsigned int)clicked.x,2)+pow(allunits[player][selectedunits[player][0]]->y-(unsigned int)clicked.y, 2))<allunits[player][selectedunits[player][0]]->los-tilecameo[map[(unsigned int)clicked.y][(unsigned int)clicked.x].tilestyle]+addLOS)
			{
				short bindex=0;
				if(overwritebuildings[player].size()>0)
				{
					bindex=overwritebuildings[player][overwritebuildings[player].size()-1];
					overwritebuildings[player].pop_back();
				}
				else
				{
					allbuildings[player].push_back(allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingplayer][map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingindex]);
					bindex=allbuildings[player].size()-1;
				}
				allbuildings[player][bindex]=allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingplayer][map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingindex];//building(allbuildablebuildings[buildwhat], x+1, y+1, player, overwritebuildings[player][overwritebuildings[player].size()-1], y+1+allbuildablebuildings[buildwhat].height, y+1, x+1, x+1+allbuildablebuildings[buildwhat].width,0,0,0,0,allbuildablebuildings[buildwhat].timetobuild);
				allbuildings[player][bindex].index=bindex;
				allbuildings[player][bindex].player=player;
				allbuildings[player][bindex].selected=false;
				allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingplayer][map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingindex].health=0;
				overwritebuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingplayer].push_back(map[(unsigned int)clicked.y][(unsigned int)clicked.x].buildingindex);
				for(float k=allbuildings[player][bindex].y-allbuildings[player][bindex].radiustodistribute; k<allbuildings[player][bindex].y+allbuildings[player][bindex].height+allbuildings[player][bindex].radiustodistribute; k+=.25)
				{
					for(float h=allbuildings[player][bindex].x-allbuildings[player][bindex].radiustodistribute; h<allbuildings[player][bindex].x+allbuildings[player][bindex].width+allbuildings[player][bindex].radiustodistribute; h+=.25)
					{
                        if(pow(k-allbuildings[player][bindex].y,2)+pow(h-allbuildings[player][bindex].x,2)<pow(allbuildings[player][bindex].radiustodistribute,2))
                            map[(int)k][(int)h].inbuildingrad=bindex;
					}
				}
				if(allbuildings[player][bindex].maxhold>0)
				{
					for(unsigned int i=0; i<allunits[player].size(); i++)
					{
						if(allunits[player][i]->nearesthold!=-1)
						{
							if(sqrt(pow(allbuildings[player][bindex].x-allunits[player][i]->gatheringx,2)+pow(allbuildings[player][bindex].y-allunits[player][i]->gatheringy,2)) < sqrt(pow(allbuildings[player][allunits[player][i]->nearesthold].x-allunits[player][i]->gatheringx, 2)+pow(allbuildings[player][allunits[player][i]->nearesthold].y-allunits[player][i]->gatheringy, 2)))
							{
								allunits[player][i]->nearesthold=bindex;
							}
						}
					}
				}
				for(float k=allbuildings[player][bindex].y; k<allbuildings[player][bindex].y+allbuildings[player][bindex].height; k+=.25)
				{
					for(float h=allbuildings[player][bindex].x; h<allbuildings[player][bindex].x+allbuildings[player][bindex].width; h+=.25)
					{
						map[(int)k][(int)h].buildingplayer=player;
						map[(int)k][(int)h].buildingindex=bindex;
						map[(int)k][(int)h].whichplayer.set(false, (unsigned char)player);
						map[(int)k][(int)h].whichplayer.set(true, players[player]);
					}
				}
			}
		}
	}
}
void designate(short player, short index)
{
	designatedunit[player]=index;
}
bool checkdimensions(float x, float y)
{
	return (x>=0 && x<map[0].size() && y>=0 && y<map.size());
}
bool checkdimensions(point p)
{
	return checkdimensions(p.x,p.y);
}
//! This functions check whether or not to display an object

//! It uses a display variable and information about the player. The display variable is a member of the button and display classes, and is set at initialization of the object
//! The flags on the display variable are checked with the bitwise & operator
bool checkdisp(int player, unsigned long long dispwhen)
{
	if(selectedunits[player].size()==0)//nothing selected
		return dispwhen&NOTHING; // returns true if should be displayed when nothing is selected. Note that since NOTHING=1, dispwhen&NOTHING=1 or 0, si no need to convert to bool with arithmetic if as below
	if(whatsselected[player]==true)//units
	{
		if(selectedunits[player].size()==1)//only one unit - none taken care of above 
		{
			if((dispwhen&YOUR_SHIP)==0 && (dispwhen&YOUR_REGIMENT_MEMBER)==0 && (dispwhen&PAGE_1)==0 && (dispwhen&PAGE_2)==0) //Its only YOUR_UNIT, not any of the rest
				return ((dispwhen&YOUR_UNIT)>=1) ? true : false; //same idea as the NOTHING one
			else if((dispwhen&YOUR_SHIP)>0)//your_ship specified
			{
				if((dispwhen&YOUR_UNIT)>=1 && allunits[player][selectedunits[player][0]]->whatisit==3) //its YOUR_UNIT and its a ship
					return true;
				else 
					return false;
			}
			else if((dispwhen&YOUR_REGIMENT_MEMBER)>0)
			{
				if((dispwhen&YOUR_UNIT)>=1 && allunits[player][selectedunits[player][0]]->regimentid!=-1) //its YOUR_UNIT and its in a regiment
					return true;
				else
					return false;
			}
			else if((dispwhen&PAGE_1)>0)
			{
				if((dispwhen&YOUR_UNIT)>=1 && pagenum==0) //Its your unit and its page 1
					return true;
				else
					return false;
			}
			else if((dispwhen&PAGE_2)>0)
			{
				if((dispwhen&YOUR_UNIT)>=1 && pagenum==1)
					return true;
				else
					return false;
			}
			else
				return false; //I don't know when this would be hit
		}
		else //multiple units selected 
		{
			if((dispwhen&YOUR_MULT_UNITS)>=1) //If disp when mult units selected
			{
				if((dispwhen&YOUR_REGIMENT_MEMBER)>=1) //if disp when all selected units in regiment
				{
					for(unsigned int i=0; i<selectedunits.size(); i++) //check
					{
						if(allunits[player][selectedunits[player][0]]->regimentid==-1) //if a unit is not in regiment, ret false
							return false;
					}
					return true; //all units in regiment: ret true
				}
				else if((dispwhen&PAGE_1)>0)
					return (pagenum==0);
				else if((dispwhen&PAGE_2)>1)
					return (pagenum==1);
				else//else, no other flags specified, return true, as there are mult units selected
					return true;
			}
			else //don't display if mult units - ret false
				return false;
		}
	}
	else//buildings
	{
		bool disp = ((((dispwhen&YOUR_BUILDING)>=1) ? true : false) && (allbuildings[player][selectedunits[player][0]].beingbuilt<0)); //one building: since you can't have more than one. Also, the building must have already been built.
		if(!disp)
			return false;
		if((dispwhen&HOUSE_BUILDING)>=1 && allbuildings[player][selectedunits[player][0]].id==1)
			return true;
		else if((dispwhen&BARRACKS_BUILDING)>=1 && allbuildings[player][selectedunits[player][0]].id==2) //else ifs for other dispwhens
			return true;
		else if((dispwhen&DOCK_BUILDING)>=1 && allbuildings[player][selectedunits[player][0]].id==3)
			return true;
        else if((dispwhen&ARCHERY_BUILDING)>=1 && allbuildings[player][selectedunits[player][0]].id==4)
            return true;
        else if((dispwhen&STABLE_BUILDING)>=1 && allbuildings[player][selectedunits[player][0]].id==5)
            return true;
        else if((dispwhen&CASTLE_BUILDING)>=1 && allbuildings[player][selectedunits[player][0]].id==13)
            return true;
        else if((dispwhen&RESEARCH_BUILDING)>=1 && allbuildings[player][selectedunits[player][0]].id==7)
            return true;
		else if((dispwhen&HOUSE_BUILDING)>=1 || (dispwhen&BARRACKS_BUILDING)>=1 || (dispwhen&DOCK_BUILDING)>=1 || (dispwhen&ARCHERY_BUILDING)>=1 || (dispwhen&STABLE_BUILDING)>=1 || (dispwhen&CASTLE_BUILDING)>=1 || (dispwhen&RESEARCH_BUILDING)>=1) //It has a flag but the other condition was false
			return false;
		else //It doesn't have any flags other than YOUR_BUILDING 
			return true;
	}
}
void build(short player, short buildwhat, int x, int y)
{
	int buildelev=map[y][x].elevation;
	bool touchland=false;
	for(int i=y; i<y+allbuildablebuildings[buildwhat].height; i++)
	{
		for(int j=x; j<x+allbuildablebuildings[buildwhat].width; j++)
		{
			if(allbuildablebuildings[buildwhat].landorwater==true) //land
			{
				if((map[i][j].tilestyle!=TS_GRASS && map[i][j].tilestyle!=TS_GOLD && map[i][j].tilestyle!=TS_STONE && map[i][j].tilestyle!=TS_ROAD) || map[i][j].elevation!=buildelev)
				{
					if(player==0)
					{
						currErr=allErr[ILLEGALLOC];
						redraw=1;
					}
					return;
				}
				touchland=true;
			}
			else //water
			{
				if((map[i][j].tilestyle!=TS_WATER && map[i][j].tilestyle!=TS_FISH) || map[i][j].elevation!=buildelev)
				{
					if(player==0)
					{
						currErr=allErr[ILLEGALLOC];
						redraw=1;
					}
					return;
				}
				if(checktouchingland(j,i))
					touchland=true;
			}
		}
	}
	if(touchland==false)
	{
		if(player==0)
		{
			currErr=allErr[ILLEGALLOC];
			redraw=1;
		}
		return;
	}
	short bindex=allunits[player][selectedunits[player][0]]->build(buildwhat,x,y);
	if(bindex==-1)
		return;
	for(unsigned int i=0; i<selectedunits[player].size(); i++)
	{
		allunits[player][selectedunits[player][i]]->garrisoned=bindex+1;
		allunits[player][selectedunits[player][i]]->movetox=allbuildings[player][bindex].x;
		allunits[player][selectedunits[player][i]]->movetoy=allbuildings[player][bindex].y;
	}
}
bool chkmvp1(tile &checkwhat, short player, short index, short utype)
{
	short walkable[6]={0};
	if(utype!=3)//if we walk on land
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
	//if(checkwhat.unitplayer==player && allunits[checkwhat.unitplayer][checkwhat.unitindex]->whatisit==3 && checkwhat.uniton==true) // It is 1 or 3 - there is a unit //AAHHHH WHAT THE HELL IS THIS???? WHAT'S IT DO??? ITS KILLI NEED IT SOMEWHERE ELSE!!!
		//return true; //removing these lines fixed the ungarrissoning sailors problem, but I really really really hope it didn't break anything.
	if(good==true && (checkwhat.unitonMineon&1)==1 && (checkwhat.unitindex!=index || checkwhat.unitplayer!=player))
		good=false;	
	return good;
}
bool checktouchingland(int x, int y)
{
	if(map[y+1][x].tilestyle==TS_GRASS || map[y+1][x].tilestyle==TS_BUSHES || map[y+1][x].tilestyle==TS_ROAD || map[y+1][x].tilestyle==TS_STONE || map[y+1][x].tilestyle==TS_GOLD || map[y+1][x].tilestyle==TS_WATERBUILDING || map[y+1][x].tilestyle==TS_BUILDING || map[y+1][x].tilestyle==TS_GATE)
		return true;
	if(map[y-1][x].tilestyle==TS_GRASS || map[y-1][x].tilestyle==TS_BUSHES || map[y-1][x].tilestyle==TS_ROAD || map[y-1][x].tilestyle==TS_STONE || map[y-1][x].tilestyle==TS_GOLD || map[y-1][x].tilestyle==TS_WATERBUILDING || map[y-1][x].tilestyle==TS_BUILDING || map[y-1][x].tilestyle==TS_GATE)
		return true;
	if(map[y][x+1].tilestyle==TS_GRASS || map[y][x+1].tilestyle==TS_BUSHES || map[y][x+1].tilestyle==TS_ROAD || map[y][x+1].tilestyle==TS_STONE || map[y][x+1].tilestyle==TS_GOLD || map[y][x+1].tilestyle==TS_WATERBUILDING || map[y][x+1].tilestyle==TS_BUILDING || map[y][x+1].tilestyle==TS_GATE)
		return true;
	if(map[y][x-1].tilestyle==TS_GRASS || map[y][x-1].tilestyle==TS_BUSHES || map[y][x-1].tilestyle==TS_ROAD || map[y][x-1].tilestyle==TS_STONE || map[y][x-1].tilestyle==TS_GOLD || map[y][x-1].tilestyle==TS_WATERBUILDING || map[y][x-1].tilestyle==TS_BUILDING || map[y][x-1].tilestyle==TS_GATE)
		return true;
	//below: no diagnols
	/*if(map[y+1][x+1].tilestyle==TS_GRASS || map[y+1][x+1].tilestyle==TS_BUSHES || map[y+1][x+1].tilestyle==TS_ROAD || map[y+1][x+1].tilestyle==TS_STONE || map[y+1][x+1].tilestyle==TS_GOLD || map[y+1][x+1].tilestyle==TS_WATERBUILDING)
		return true;
	if(map[y+1][x-1].tilestyle==TS_GRASS || map[y+1][x-1].tilestyle==TS_BUSHES || map[y+1][x-1].tilestyle==TS_ROAD || map[y+1][x-1].tilestyle==TS_STONE || map[y+1][x-1].tilestyle==TS_GOLD || map[y+1][x-1].tilestyle==TS_WATERBUILDING)
		return true;
	if(map[y-1][x-1].tilestyle==TS_GRASS || map[y-1][x-1].tilestyle==TS_BUSHES || map[y-1][x-1].tilestyle==TS_ROAD || map[y-1][x-1].tilestyle==TS_STONE || map[y-1][x-1].tilestyle==TS_GOLD || map[y-1][x-1].tilestyle==TS_WATERBUILDING)
		return true;
	if(map[y-1][x+1].tilestyle==TS_GRASS || map[y-1][x+1].tilestyle==TS_BUSHES || map[y-1][x+1].tilestyle==TS_ROAD || map[y-1][x+1].tilestyle==TS_STONE || map[y-1][x+1].tilestyle==TS_GOLD || map[y-1][x+1].tilestyle==TS_WATERBUILDING)
		return true;*/
	return false;
}
string inttos(int num)
{
	string ret;
	if(num==0)
	{
		ret.push_back('0');
		return ret;
	}
	while(num!=0)
	{
		ret.push_back(num%10+'0');
		num/=10;
	}
	for(unsigned int i=0; i<ret.size()/2; i++)
	{
		swap(ret[i], ret[ret.size()-i-1]);
	}
	return ret;
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50,30);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH)-80,glutGet(GLUT_SCREEN_HEIGHT)-60);
	//glutInitWindowSize(700,700);

	mainWindow=glutCreateWindow("Game Engine");

	//glutFullScreen();
	WIDTH=glutGet(GLUT_WINDOW_WIDTH);
	HEIGHT=glutGet(GLUT_WINDOW_HEIGHT);

	glutDisplayFunc(renderScene);
	glutMouseFunc(processMouse);
	glutReshapeFunc(changeSize);
	glutMotionFunc(processMouseMove); //this is while mouse button pressed
	glutPassiveMotionFunc(processPassiveMouseMove); //this is while mouse button is not pressed
	glutKeyboardFunc(processKeys);

	initializeGameEngine();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
	return 0;
}
void initPrices()
{
    //Infantry to Infantry
    allbuildableunits[4].allPriceMods.push_back(unitPriceMod(5,30,0,10,0,100)); //militia to swordsman
    allbuildableunits[4].allPriceMods.push_back(unitPriceMod(6,20,0,10,0,80)); //militia to spearman
    allbuildableunits[4].allPriceMods.push_back(unitPriceMod(9,70,0,20,0,200)); //militia to shielded swordsman
    allbuildableunits[4].allPriceMods.push_back(unitPriceMod(10,70,0,20,0,200)); //militia to two-handed swordsman
    allbuildableunits[4].allPriceMods.push_back(unitPriceMod(11,110,0,45,0,250)); //militia to dual swordsman
    allbuildableunits[4].allPriceMods.push_back(unitPriceMod(13,80,0,25,0,200)); //militia to pikeman
    allbuildableunits[4].allPriceMods.push_back(unitPriceMod(15,70,0,30,0,200)); //militia to axe man
    
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(6,20,0,10,0,70)); //swordsman to spearman
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(9,60,0,15,0,120)); //swordsman to shielded swordsman
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(10,60,0,15,0,120)); //swordsman to two handed swordsman
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(11,85,0,35,0,200)); //swordsman to dual swordsman
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(12,120,25,65,0,350)); //swordsman to swordsmaster
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(13,65,0,25,0,130)); //swordsman to pikeman
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(14,105,0,40,0,190)); //swordsman to elite pikeman
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(15,60,0,30,0,130)); //swordsman to axe man
    allbuildableunits[5].allPriceMods.push_back(unitPriceMod(16,125,15,80,0,370)); //swordsman to axe master
    
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(5,22,0,10,0,80)); //spearman to swordsman
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(9,65,0,17,0,160)); //spearman to shielded swordsman
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(10,65,0,17,0,160)); //spearman to two handed swordsman
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(11,95,0,40,0,240)); //spearman to dual swordsman
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(13,63,0,15,0,125)); //spearman to pikeman
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(14,100,0,40,0,180)); //spearman to elite pikeman
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(15,63,0,30,0,160)); //spearman to axe man
    allbuildableunits[6].allPriceMods.push_back(unitPriceMod(16,135,15,80,0,380)); //spearman to axe master
    
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(10,15,0,5,0,40)); //shielded swordsman to two handed swordsman
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(11,42,0,25,0,100)); //shielded swordsman to dual swordsman
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(12,90,25,50,0,275)); //shielded swordsman to swordsmaster
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(13,20,0,10,0,80)); //shielded swordsman to pikeman
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(14,85,0,35,0,175)); //shielded swordsman to elite pikeman
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(15,20,0,15,0,160)); //shielded swordsman to axe man
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(16,100,15,70,0,320)); //shielded swordsman to axe master
    
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(9,15,0,5,0,40)); //two-handed swordsman to shielded swordsman
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(11,40,0,23,0,100)); //two-handed swordsman to dual swordsman
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(12,90,25,50,0,275)); //two-handed swordsman to swordsmaster
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(13,20,0,10,0,80)); //two-handed swordsman to pikeman
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(14,85,0,35,0,175)); //two-handed swordsman to elite pikeman
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(15,18,0,15,0,160)); //two-handed swordsman to axe man
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(16,100,15,70,0,320)); //two-handed swordsman to axe master
    
    allbuildableunits[11].allPriceMods.push_back(unitPriceMod(12,60,25,40,0,150)); //dual swordsman to swordsmaster
    allbuildableunits[11].allPriceMods.push_back(unitPriceMod(13,15,0,5,0,80)); //dual swordsman to pikeman
    allbuildableunits[11].allPriceMods.push_back(unitPriceMod(14,30,0,15,0,150)); //dual swordsman to elite pikeman
    allbuildableunits[11].allPriceMods.push_back(unitPriceMod(15,10,0,10,0,80)); //dual swordsman to axe man
    allbuildableunits[11].allPriceMods.push_back(unitPriceMod(16,65,15,50,0,175)); //dual swordsman to axe master
    
    allbuildableunits[12].allPriceMods.push_back(unitPriceMod(14,30,0,10,0,100)); //swordsmaster to elite pikeman
    allbuildableunits[12].allPriceMods.push_back(unitPriceMod(16,20,0,25,0,100)); //swordsmaster to axe master
    
    allbuildableunits[13].allPriceMods.push_back(unitPriceMod(9,20,0,10,0,100)); //pikeman to shielded swordsman
    allbuildableunits[13].allPriceMods.push_back(unitPriceMod(10,20,0,10,0,100)); //pikeman to two-handed swordsman
    allbuildableunits[13].allPriceMods.push_back(unitPriceMod(11,55,0,35,0,160)); //pikeman to dual swordsman
    allbuildableunits[13].allPriceMods.push_back(unitPriceMod(12,100,25,57,0,340)); //pikeman to swordsmaster
    allbuildableunits[13].allPriceMods.push_back(unitPriceMod(14,50,0,25,0,150)); //pikeman to elite pikeman
    allbuildableunits[13].allPriceMods.push_back(unitPriceMod(15,18,0,20,0,150)); //pikeman to axe man
    allbuildableunits[13].allPriceMods.push_back(unitPriceMod(16,110,15,75,0,325)); //pikeman to axe master
    
    allbuildableunits[14].allPriceMods.push_back(unitPriceMod(12,65,25,45,0,250)); //elite pikeman to swordsmaster
    allbuildableunits[14].allPriceMods.push_back(unitPriceMod(16,65,15,50,0,260)); //elite pikeman to axe master
    
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(9,25,0,7,0,100)); //axe man to shielded swordsman
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(10,25,0,7,0,100)); //axe man to two-handed swordsman
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(11,65,0,30,0,155)); //axe man to dual swordsman
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(12,120,25,55,0,333)); //axe man to swordsmaster
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(13,25,0,5,0,100)); //axe man to pikeman
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(14,55,0,28,0,150)); //axe man to elite pikeman
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(16,100,15,70,0,315)); //axe man to axe master
    
    allbuildableunits[16].allPriceMods.push_back(unitPriceMod(12,30,15,10,0,100)); //axe master to swordsmaster 
    allbuildableunits[16].allPriceMods.push_back(unitPriceMod(14,20,0,5,0,100)); //axe master to elite pikeman
    
    //Archers to Archers
    allbuildableunits[7].allPriceMods.push_back(unitPriceMod(8,15,15,5,0,90)); //archer to javelinist
    allbuildableunits[7].allPriceMods.push_back(unitPriceMod(17,50,25,12,0,150)); //archer to longbow man
    allbuildableunits[7].allPriceMods.push_back(unitPriceMod(18,35,25,50,0,160)); //archer to crossbow man
    allbuildableunits[7].allPriceMods.push_back(unitPriceMod(19,65,50,33,0,170)); //archer to elite javelinist
    
    allbuildableunits[8].allPriceMods.push_back(unitPriceMod(7,15,10,5,0,80)); //javelinist to archer
    allbuildableunits[8].allPriceMods.push_back(unitPriceMod(17,55,20,12,0,160)); //javelinist to longbow man
    allbuildableunits[8].allPriceMods.push_back(unitPriceMod(18,40,20,50,0,160)); //javelinist to crossbow man
    allbuildableunits[8].allPriceMods.push_back(unitPriceMod(19,60,45,30,0,165)); //javelinist to elite javelinist
    
    allbuildableunits[17].allPriceMods.push_back(unitPriceMod(18,20,15,45,0,100)); //longbowman to crossbow man
    allbuildableunits[17].allPriceMods.push_back(unitPriceMod(19,30,35,20,0,110)); //longbowman to elite javelinist
    
    allbuildableunits[18].allPriceMods.push_back(unitPriceMod(17,30,15,10,0,100)); //crossbow man to longbowman
    allbuildableunits[18].allPriceMods.push_back(unitPriceMod(19,40,35,15,0,110)); //crossbow man to elite javelinist
    
    allbuildableunits[19].allPriceMods.push_back(unitPriceMod(17,20,10,15,0,100)); //elite javelinist to longbowman
    allbuildableunits[19].allPriceMods.push_back(unitPriceMod(18,15,10,30,0,110)); //elite javelinist to crossbow man
    
    //Scouts and spies and assassins to same.
    allbuildableunits[20].allPriceMods.push_back(unitPriceMod(21,70,0,15,0,100)); //scout to mounted scout
    allbuildableunits[20].allPriceMods.push_back(unitPriceMod(32,75,0,70,0,210)); //scout to spy
    
    allbuildableunits[21].allPriceMods.push_back(unitPriceMod(20,15,0,10,0,75)); //mounted scout to scout
    allbuildableunits[21].allPriceMods.push_back(unitPriceMod(32,45,0,67,0,200)); //mounted scout to spy
    
    allbuildableunits[32].allPriceMods.push_back(unitPriceMod(33,45,0,115,0,175)); //spy to elite spy
    allbuildableunits[32].allPriceMods.push_back(unitPriceMod(34,85,0,165,0,250)); //spy to assassin
    
    allbuildableunits[33].allPriceMods.push_back(unitPriceMod(34,45,0,110,0,130)); //elite spy to assassin
    
    //archer cavalry to archer cavalry
    allbuildableunits[22].allPriceMods.push_back(unitPriceMod(23,35,35,10,0,225)); //mounted javelinist to mounted archer
    allbuildableunits[22].allPriceMods.push_back(unitPriceMod(24,23,50,10,0,275)); //mounted javelinist to fast mounted archer
    
    allbuildableunits[23].allPriceMods.push_back(unitPriceMod(24,10,30,7,0,100)); //mounted archer to fast mounted archer
    
    allbuildableunits[24].allPriceMods.push_back(unitPriceMod(23,18,10,7,0,100)); //fast mounted archer to mounted archer
    
    //heavy cavalry to heavy cavalry
    allbuildableunits[25].allPriceMods.push_back(unitPriceMod(26,50,0,30,0,150)); //apprentice knight to knight
    allbuildableunits[25].allPriceMods.push_back(unitPriceMod(27,100,20,55,0,275)); //apprentice knight to elite knight
    
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(27,55,20,40,0,150)); //apprentice knight to elite knight
    
    //Transport
    allbuildableunits[28].allPriceMods.push_back(unitPriceMod(29,15,50,13,0,75)); //oxen to cart
    allbuildableunits[28].allPriceMods.push_back(unitPriceMod(30,40,75,23,0,130)); //oxen to pull team
    allbuildableunits[28].allPriceMods.push_back(unitPriceMod(31,80,125,58,0,170)); //oxen to heavy pull team
    
    allbuildableunits[29].allPriceMods.push_back(unitPriceMod(30,40,50,15,0,90)); //cart to pull team
    allbuildableunits[29].allPriceMods.push_back(unitPriceMod(31,80,100,52,0,125)); //cart to heavy pull team
    
    allbuildableunits[30].allPriceMods.push_back(unitPriceMod(31,40,75,47,0,100)); //pull team to heavy pull team
    
    //Archers to Archer cavalry
    allbuildableunits[7].allPriceMods.push_back(unitPriceMod(22,70,20,15,0,100)); //archer to mounted javelinist
    allbuildableunits[7].allPriceMods.push_back(unitPriceMod(23,100,30,22,0,215)); //archer to mounted archer
    allbuildableunits[7].allPriceMods.push_back(unitPriceMod(24,97,50,26,0,250)); //archer to fast mounted archer
    
    allbuildableunits[8].allPriceMods.push_back(unitPriceMod(22,65,15,15,0,80)); //javelinist to mounted javelinist
    allbuildableunits[8].allPriceMods.push_back(unitPriceMod(23,108,25,22,0,230)); //javelinist to mounted archer
    allbuildableunits[8].allPriceMods.push_back(unitPriceMod(24,105,45,26,0,270)); //javelinist to fast mounted archer
    
    allbuildableunits[17].allPriceMods.push_back(unitPriceMod(23,55,15,20,0,125)); //longbow man to mounted archer
    allbuildableunits[17].allPriceMods.push_back(unitPriceMod(24,50,33,20,0,160)); //longbow man to fast mounted archer
    
    allbuildableunits[18].allPriceMods.push_back(unitPriceMod(23,75,17,7,0,130)); //crossbowman man to mounted archer
    allbuildableunits[18].allPriceMods.push_back(unitPriceMod(24,70,35,7,0,165)); //crossbowman man to fast mounted archer
    
    allbuildableunits[19].allPriceMods.push_back(unitPriceMod(23,45,13,12,0,120)); //elite javelinist to mounted archer
    allbuildableunits[19].allPriceMods.push_back(unitPriceMod(24,42,15,12,0,155)); //elite javelinist to fast mounted archer
    
    //archer cavalry to archers 
    allbuildableunits[22].allPriceMods.push_back(unitPriceMod(7,15,15,10,0,100)); //mounted javelinist to archer
    allbuildableunits[22].allPriceMods.push_back(unitPriceMod(8,10,7,7,0,60)); //mounted javelinist to javelinist
    allbuildableunits[22].allPriceMods.push_back(unitPriceMod(17,45,25,15,0,175)); //mounted javelinist to longbowman
    allbuildableunits[22].allPriceMods.push_back(unitPriceMod(18,40,25,50,0,175)); //mounted javelinist to crossbowman
    allbuildableunits[22].allPriceMods.push_back(unitPriceMod(19,45,45,30,0,185)); //mounted javelinist to crossbowman
    
    allbuildableunits[23].allPriceMods.push_back(unitPriceMod(17,45,25,15,0,175)); //mounted archer to longbowman
    allbuildableunits[23].allPriceMods.push_back(unitPriceMod(18,40,25,50,0,175)); //mounted archer to crossbowman
    allbuildableunits[23].allPriceMods.push_back(unitPriceMod(19,45,45,30,0,185)); //mounted archer to elite javelinist
    
    allbuildableunits[24].allPriceMods.push_back(unitPriceMod(17,55,20,15,0,175)); //fast mounted archer to longbowman
    allbuildableunits[24].allPriceMods.push_back(unitPriceMod(18,50,20,50,0,175)); //fast mounted archer to crossbowman
    allbuildableunits[24].allPriceMods.push_back(unitPriceMod(19,55,35,30,0,185)); //fast mounted archer to elite javelinist
    
    //heavy cavalry to infantry
    allbuildableunits[25].allPriceMods.push_back(unitPriceMod(9,15,0,12,0,125)); //apprentice knight to shielded swordsman
    allbuildableunits[25].allPriceMods.push_back(unitPriceMod(10,15,0,12,0,125)); //apprentice knight to two-handed swordsman
    allbuildableunits[25].allPriceMods.push_back(unitPriceMod(11,55,0,38,0,200)); //apprentice knight to dual swordsman
    allbuildableunits[25].allPriceMods.push_back(unitPriceMod(15,12,0,20,0,175)); //apprentice knight to axe man
    
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(9,12,0,10,0,100)); //knight to shielded swordsman
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(10,12,0,10,0,100)); //knight to two-handed swordsman
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(11,25,0,20,0,125)); //knight to dual swordsman
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(12,70,25,65,0,275)); //knight to swordsmaster
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(13,12,0,10,0,100)); //knight to pikeman
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(15,10,0,14,0,100)); //knight to axe man
    allbuildableunits[26].allPriceMods.push_back(unitPriceMod(16,70,15,78,0,275)); //knight to axe master
    
    allbuildableunits[27].allPriceMods.push_back(unitPriceMod(11,20,0,15,0,100)); //elite knight to dual swordsman
    allbuildableunits[27].allPriceMods.push_back(unitPriceMod(12,50,15,40,0,180)); //elite knight to swordsmaster   
    allbuildableunits[27].allPriceMods.push_back(unitPriceMod(14,20,0,15,0,120)); //elite knight to elite pikeman
    allbuildableunits[27].allPriceMods.push_back(unitPriceMod(16,50,10,55,0,180)); //elite knight to axe master
    
    //Infantry to heavy cavalry
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(24,25,0,15,0,125)); //shielded swordsman to apprentice knight
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(25,55,0,20,0,200)); //shielded swordsman to knight
    allbuildableunits[9].allPriceMods.push_back(unitPriceMod(26,100,0,52,0,300)); //shielded swordsman to elite knight
    
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(24,25,0,15,0,125)); //two-handed swordsman to apprentice knight
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(25,55,0,20,0,200)); //two-handed swordsman to knight
    allbuildableunits[10].allPriceMods.push_back(unitPriceMod(26,100,0,52,0,300)); //two-handed swordsman to elite knight
    
    allbuildableunits[11].allPriceMods.push_back(unitPriceMod(25,35,0,15,0,150)); //dual swordsman to knight
    allbuildableunits[11].allPriceMods.push_back(unitPriceMod(26,90,20,42,0,225)); //dual swordsman to elite knight
    
    allbuildableunits[12].allPriceMods.push_back(unitPriceMod(26,25,15,25,0,130)); //swordsmaster to elite knight
    
    allbuildableunits[14].allPriceMods.push_back(unitPriceMod(25,40,0,20,0,200)); //elite pikeman to knight
    allbuildableunits[14].allPriceMods.push_back(unitPriceMod(26,100,20,45,0,275)); //elite pikeman to elite knight
    
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(24,30,0,12,0,125)); //axe man to apprentice knight
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(25,70,0,20,0,200)); //axe man to knight
    allbuildableunits[15].allPriceMods.push_back(unitPriceMod(26,120,20,47,0,300)); //axe man to elite knight
    
    allbuildableunits[16].allPriceMods.push_back(unitPriceMod(26,25,17,18,0,130)); //axe master to elite knight
}  
void initializeGameEngine()
{
	srand((unsigned int)time(0)); //From here: absolutely necessary stuff.
	redraw=5;
	map.resize(MAPSIZE);
	minimapseen.resize(numplayers);
	overwriteunits.resize(numplayers);
	overwritebuildings.resize(numplayers);
	selectedunits.resize(numplayers);
	whatsselected.resize(numplayers);
    resources.resize(numplayers);
    buildingwhat.resize(numplayers);
    for(int i=0; i<numplayers; i++)
    {
        minimapseen[i].resize(MAPSIZE);
        resources[i].resize(4);
        buildingwhat[i]=-1;
    }
	allsiegeunits.resize(numplayers);
	players.push_back(0);
	players.push_back(1);
	newlybuiltbuildings.resize(numplayers);
	designatedunit.resize(numplayers);
	allregiments.resize(numplayers);
	allgarrisonedselectedunits.resize(numplayers);
    unitAllowed.resize(numplayers);
    buildingAllowed.resize(numplayers);
	for(int i=0; i<MAPSIZE; i++)
	{
		map[i].resize(MAPSIZE);
        for(int j=0; j<numplayers; j++)
            minimapseen[j][i].resize(MAPSIZE);
	}
	allunits.resize(numplayers);
	actallunits.resize(numplayers);
	actallunits[0].reserve(100);
	actallunits[1].reserve(100);
	allbuildings.resize(numplayers);
	allobstacles.resize(numplayers);
	//players.push_back(0); //repeat for some reason. 
	//players.push_back(1);
	creationqueueunits.resize(numplayers);
    advComplete.resize(numplayers);
    alreadyResearched.resize(numplayers);
    researchAllowed.resize(numplayers);
    allCurrResearch.resize(numplayers);

	ifstream inf("unit.specs"); //Get allbuildableunits
	if(!inf)
	{
		printf("Unable to open unit.specs");
		exit(-235);
	}
	float args[28]={0};
	string name;
	while(!inf.eof())
	{
		for(int i=0; i<28; i++) //28=number of args to basic unit. Make sure that this is so.
		{
			inf >> args[i];
			inf.get();
		}
		getline(inf, name);
		allbuildableunits.push_back(basicunit(args[0],(short)args[1],(short)args[2],(short)args[3],(short)args[4],args[5],args[6],(short)args[7],(short)args[8],(short)args[9],args[10],(short)args[11],args[12],(short)args[13],(short)args[14],(short)args[15],(short)args[16],(unsigned char)args[17],(unsigned char)args[18],(unsigned char)args[19],(short)args[20],(short)args[21], (char)args[22],(short)args[23],(short)args[24],(short)args[25],(short)args[26],(short)args[27],name));
        if(allbuildableunits.size()>1 && allbuildableunits[allbuildableunits.size()-1].id==allbuildableunits[allbuildableunits.size()-2].id)
        {
            allbuildableunits.erase(allbuildableunits.end()-1);
            break;
        }
	}
	ifstream inf2("building.specs");
	if(!inf2)
	{
		printf("Unable to open building.specs");
		exit(-235);
	}
	while(!inf2.eof())
	{
		for(int i=0; i<21; i++) //21=number of args to basic building. Make sure that this is so.
		{
			inf2 >> args[i];
			inf2.get();
		}
		getline(inf2, name);
		allbuildablebuildings.push_back(basicbuilding((short)args[0],(short)args[1],(short)args[2],args[3],(short)args[4],(short)args[5],(short)args[6],(short)args[7],(short)args[8],(short)args[9],(short)args[10],args[11],args[12],(short)args[13],(short)args[14],(bool)args[15],(short)args[16],(short)args[17],(short)args[18],(short)args[19],(short)args[20],name));
        if(allbuildablebuildings.size()>1 && allbuildablebuildings[allbuildablebuildings.size()-1].id==allbuildablebuildings[allbuildablebuildings.size()-2].id)
        {
            allbuildablebuildings.erase(allbuildablebuildings.end()-1);
            break;
        }
	}
    ifstream inf4("research.specs");
    if(!inf4)
	{
		printf("Unable to open research.specs");
		exit(-242);
	}
    while(!inf4.eof())
	{
		for(int i=0; i<21; i++) //21=number of args to research. Make sure that this is so.
		{
			inf4 >> args[i];
            args[i]+=0.001f; //just in case. All casted to float anyway.
			inf4.get();
		}
		getline(inf4, name);
		allResearches.push_back(Research((short)args[0],(short)args[1],(short)args[2],args[3],(short)args[4],(short)args[5],(short)args[6],(short)args[7],(short)args[8],(short)args[9],(short)args[10],(short)args[11],(short)args[12],(short)args[13],(short)args[14],(short)args[15],(short)args[16],(short)args[17],(short)args[18], (short)args[19], (short)args[20], name));
        if(allResearches.size()>1 && allResearches[allResearches.size()-1].name==allResearches[allResearches.size()-2].name)
        {
            allResearches.erase(allResearches.end()-1);
            break;
        }
	}
	ifstream inf3("map2.specs");
	if(!inf3)
	{
		printf("Unable to open map.specs");
		exit(-235);
	}

    for(int i=0; i<numplayers; i++)
    {
        unitAllowed[i].resize(allbuildableunits.size());
        buildingAllowed[i].resize(allbuildablebuildings.size());
        researchAllowed[i].resize(allResearches.size());
        for(unsigned int j=0; j<unitAllowed[i].size(); j++)
            unitAllowed[i][j]=false;
        for(unsigned int j=0; j<buildingAllowed[i].size(); j++)
            buildingAllowed[i][j]=false;
        for(unsigned int j=0; j<researchAllowed[i].size(); j++)
            researchAllowed[i][j]=false;
    }
    
    //what can train what
    allbuildablebuildings[1].unitsmade.push_back(1); //house can train villager
    allbuildablebuildings[2].unitsmade.push_back(4); //barracks: militia
    allbuildablebuildings[2].unitsmade.push_back(5); //barracks: swordman
    allbuildablebuildings[2].unitsmade.push_back(6); //barracks: spearman
    allbuildablebuildings[3].unitsmade.push_back(3); //port: ship
    allbuildablebuildings[4].unitsmade.push_back(7); //Archery Range: archer
    allbuildablebuildings[4].unitsmade.push_back(8); //Archery Range: javelinist
    allbuildablebuildings[2].unitsmade.push_back(9); //barracks: shielded swordsman
    allbuildablebuildings[2].unitsmade.push_back(10); //barracks: two-handed swordsman
    allbuildablebuildings[2].unitsmade.push_back(11); //barracks: dual-swordsman
    allbuildablebuildings[2].unitsmade.push_back(12); //barracks: swordsmaster
    allbuildablebuildings[2].unitsmade.push_back(13); //barracks: pikeman
    allbuildablebuildings[2].unitsmade.push_back(14); //barracks: elite pikeman
    allbuildablebuildings[2].unitsmade.push_back(15); //barracks: axe man
    allbuildablebuildings[2].unitsmade.push_back(16); //barracks: axe master
    allbuildablebuildings[4].unitsmade.push_back(17); //archery range: longbow man
    allbuildablebuildings[4].unitsmade.push_back(18); //archery range: crossbow man
    allbuildablebuildings[4].unitsmade.push_back(19); //archery range: elite javelinist
    allbuildablebuildings[2].unitsmade.push_back(20); //barracks: scout
    allbuildablebuildings[5].unitsmade.push_back(21); //stable: mounted scout
    allbuildablebuildings[5].unitsmade.push_back(22); //stable: mounted javelinist
    allbuildablebuildings[5].unitsmade.push_back(23); //stable: mounted archer
    allbuildablebuildings[5].unitsmade.push_back(24); //stable: mounted fast archer
    allbuildablebuildings[5].unitsmade.push_back(25); //stable: Apprentice Knight
    allbuildablebuildings[5].unitsmade.push_back(26); //stable: Knight
    allbuildablebuildings[5].unitsmade.push_back(27); //stable: Elite Knight
    allbuildablebuildings[5].unitsmade.push_back(28); //stable: oxen
    allbuildablebuildings[5].unitsmade.push_back(29); //stable: cart
    allbuildablebuildings[5].unitsmade.push_back(30); //stable: pull team
    allbuildablebuildings[5].unitsmade.push_back(31); //stable: heavy pull team
    allbuildablebuildings[13].unitsmade.push_back(32); //castle: spy
    allbuildablebuildings[13].unitsmade.push_back(33); //castle: elite spy
    allbuildablebuildings[13].unitsmade.push_back(34); //castle: assassin
	    
    //allowing things
    for(unsigned int i=0; i<unitAllowed.size(); i++)
    {
        unitAllowed[i][1]=true; //villager allowed
        unitAllowed[i][4]=true; //militia allowed
        unitAllowed[i][3]=true; //ship allowed
        unitAllowed[i][20]=true; //scout allowed
    }
    for(unsigned int i=0; i<buildingAllowed.size(); i++)
    {
        buildingAllowed[i][1]=true; //house allowed
        buildingAllowed[i][2]=true; //barracks allowed
        buildingAllowed[i][3]=true; //port allowed
        buildingAllowed[i][8]=true; //wall
        buildingAllowed[i][10]=true; //storehouse
        buildingAllowed[i][15]=true; //market
        buildingAllowed[i][16]=true; //farm
        buildingAllowed[i][18]=true; //road
    }
    
    //bonuses
    vector<short> unitsaffected;
    for(int i=21; i<=31; i++)
        unitsaffected.push_back(i); //add all units from 21 to 31: all cavalry. Mounted archers, knights, and transport.
    allbuildableunits[6].allBonuses.push_back(powerBonus(unitsaffected,0,2,5,0)); //give spearmen a bonus vs. cavalry
    allbuildableunits[13].allBonuses.push_back(powerBonus(unitsaffected,3,2.5,7,0)); //give pikemen a bonus vs. cavalry
    allbuildableunits[14].allBonuses.push_back(powerBonus(unitsaffected,10,2.5,10,0)); //give elite pikemen a bonus vs. cavalry

    initPrices(); //set prices
    
    //buttons
	allbuttons.push_back(button(WIDTH*2/3+5+300+20,615,92,18,"Make Regiment",makereg,YOUR_MULT_UNITS));
	indexStancebutton=allbuttons.size();
	allbuttons.push_back(button(WIDTH*2/3+5,615,70,18,"Aggressive",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5+75,615,70,18,"Defensive",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5+150,615,70,18,"Don't Move",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5+225,615,70,18,"Do Nothing",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5, 615+22, 70, 18, "Designate",designate,YOUR_UNIT));
	allbuttons.push_back(button(WIDTH*2/3+5, 615+22*3, 70, 18, "Record", beginrecordreport, YOUR_UNIT | YOUR_MULT_UNITS | YOUR_REGIMENT_MEMBER));
    allbuttons.push_back(button(WIDTH*2/3+5, 615+22*3, 70, 18, "Record", unitrecordreport, YOUR_UNIT));
	allbuttons.push_back(button(WIDTH*2/3+5+75, 615+22*3, 70, 18, "Report", givereport, YOUR_MULT_UNITS | YOUR_REGIMENT_MEMBER));
    allbuttons.push_back(button(WIDTH*2/3+5+75, 615+22*3, 70, 18, "Report", givereport, YOUR_UNIT));
	indexTransferbutton=allbuttons.size();
    allbuttons.push_back(button(WIDTH*2/3+5+300, 615+22*3, 113, 18, "Transfer Resources", transferResourcesButton, YOUR_UNIT | YOUR_MULT_UNITS));	
    allMouseOver.push_back(mouseOver("Enter resources to transfer from one building to another in the format: [type of resource][amount] i.e. Food100gold20W40s10 or just g30. At any point it may be the whole word or just the first letter.",5,HEIGHT-105,indexTransferbutton,nullMouseFunc));
    
    indexBuildingsbutton=allbuttons.size();
	for(unsigned int i=1; i<allbuildablebuildings.size(); i++) //starting with 1 skips the pile
	{
		if(i<=12)
			allbuttons.push_back(button(5+103*((i-1)/3), 615+22*((i-1)%3), 100, 18, allbuildablebuildings[i].name, build, YOUR_UNIT | YOUR_MULT_UNITS | PAGE_1,i));
		else
			allbuttons.push_back(button(5+103*((i-13)/3), 615+22*((i-13)%3), 100, 18, allbuildablebuildings[i].name, build, YOUR_UNIT | YOUR_MULT_UNITS | PAGE_2,i));
		allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildBuilding));
	}
    indexBuildingsbuttonend=allbuttons.size();
	allbuttons.push_back(button(5+309, 615+66, 100, 18, "Switch Page", nextpage, YOUR_UNIT | YOUR_MULT_UNITS));
	//allbuttons.push_back(button(5, 615, 60, 18, "House", build, YOUR_UNIT | YOUR_MULT_UNITS,0));
	//allbuttons.push_back(button(5, 615+22, 60, 18, "Barracks", build, YOUR_UNIT | YOUR_MULT_UNITS,2));
	//allbuttons.push_back(button(5, 615+44, 60, 18, "Dock", build, YOUR_UNIT | YOUR_MULT_UNITS,3));

    indexUnitButtons=allbuttons.size(); //unit buttons. Not in loop :(
	
    allbuttons.push_back(button(5, 611, 60, 18, "Villager", createnewunit, YOUR_BUILDING | HOUSE_BUILDING, 1));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
	
    allbuttons.push_back(button(5, 611, 60, 18, "Militia", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 4));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
	
    allbuttons.push_back(button(5, 611, 60, 18, "Ship", createnewunit, YOUR_BUILDING | DOCK_BUILDING, 3));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85+45, 611, 60, 18, "Swordman", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 5));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85+158, 611, 78, 18, "Spearman", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 6));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611, 85, 18, "Archer", createunit, YOUR_BUILDING | ARCHERY_BUILDING, 7));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+90, 611, 85, 18, "Javelinist", createunit, YOUR_BUILDING | ARCHERY_BUILDING, 8));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85, 611+20, 150, 18, "Shielded Swordsman", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 9));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85, 611+40, 150, 18, "Two-Handed Swordsman", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 10));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85, 611+60, 150, 17, "Dual-Swordsman", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 11));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85, 611+79, 150, 17, "Swordsmaster", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 12));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85+158, 611+20, 78, 18, "Pikeman", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 13));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85+158, 611+40, 78, 18, "Elite Pikeman", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 14));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85+158+85, 611+40, 75, 18, "Axe Man", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 15));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+85+158+85, 611+79, 75, 17, "Axe Master", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 16));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+20, 85, 17, "Longbow Man", createunit, YOUR_BUILDING | ARCHERY_BUILDING, 17));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+40, 85, 17, "Crossbowman", createunit, YOUR_BUILDING | ARCHERY_BUILDING, 18));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+90, 611+40, 85, 17, "Elite Javelinist", createunit, YOUR_BUILDING | ARCHERY_BUILDING, 19));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+20, 60, 18, "Scout", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 20));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611, 125, 18, "Mounted Scout", createunit, YOUR_BUILDING | STABLE_BUILDING, 21));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+20, 125, 18, "Mounted Javelinist", createunit, YOUR_BUILDING | STABLE_BUILDING, 22));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+40, 125, 18, "Fast Mounted Archer", createunit, YOUR_BUILDING | STABLE_BUILDING, 24));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+60, 125, 18, "Mounted Archer", createunit, YOUR_BUILDING | STABLE_BUILDING, 23));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+130, 611, 115, 18, "Apprentice Knight", createunit, YOUR_BUILDING | STABLE_BUILDING, 25));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+130, 611+20, 115, 18, "Knight", createunit, YOUR_BUILDING | STABLE_BUILDING, 26));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+130, 611+40, 115, 18, "Elite Knight", createunit, YOUR_BUILDING | STABLE_BUILDING, 27));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+130+120, 611, 105, 18, "Oxen", createunit, YOUR_BUILDING | STABLE_BUILDING, 28));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+130+120, 611+20, 105, 18, "Cart", createunit, YOUR_BUILDING | STABLE_BUILDING, 29));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+130+120, 611+40, 105, 18, "Pull Team", createunit, YOUR_BUILDING | STABLE_BUILDING, 30));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5+130+120, 611+60, 105, 18, "Heavy Pull Team", createunit, YOUR_BUILDING | STABLE_BUILDING, 31));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611, 70, 18, "Spy", createunit, YOUR_BUILDING | CASTLE_BUILDING, 32));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+20, 70, 18, "Elite Spy", createunit, YOUR_BUILDING | CASTLE_BUILDING, 33));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    allbuttons.push_back(button(5, 611+40, 70, 18, "Assassin", createunit, YOUR_BUILDING | CASTLE_BUILDING, 34));
    allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,buildUnit));
    
    indexUnitButtonsend=allbuttons.size();
    
	indexGarrisonbutton=allbuttons.size();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			allbuttons.push_back(button(WIDTH/3+5+j*80, 615+22*i, 75, 18, "", selectgarrison, YOUR_BUILDING));
            allMouseOver.push_back(mouseOver("Left-click to select this unit for a to-be-chosen training. Right-click to ungarrison.", 5, HEIGHT-105,allbuttons.size()-1,nullMouseFunc));
		}
	}
	indexGarrisonbuttonend=allbuttons.size();
	indexSailorsbutton=allbuttons.size();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			allbuttons.push_back(button(WIDTH/3+5+j*80, 615+22*i, 75, 18, "", leaveship, YOUR_UNIT | YOUR_SHIP));
		}
	}
	indexSailorsbuttonend=allbuttons.size();
	allbuttons.push_back(button(WIDTH*2/3+5, 615, 88, 18, "Ungarrison All",ungarrisonall,YOUR_BUILDING));
    indexResearchbutton=allbuttons.size(); //research buttons. UPDATE IF RESEARCHES CREATED AT DIFFERENT BUILDINGS. Also, maybe researches will be unlocked after researching something else, so the button can be in the same place. Also, maybe there will be pagination, like for buildings
    for(unsigned int i=0; i<allResearches.size(); i++)
    {
        allbuttons.push_back(button(5+(allResearches[i].xpos*105),613+(allResearches[i].ypos*19),95,17,allResearches[i].name,beginresearch,YOUR_BUILDING | RESEARCH_BUILDING,i));
        allMouseOver.push_back(mouseOver("",5,HEIGHT-105,allbuttons.size()-1,researchSomething));
    }
    indexResearchbuttonend=allbuttons.size();
    //end research buttons
    
	//display
	indexResourcedispunit=alldisp.size();
	//alldisp.push_back(display("Food", width/3+5, 615, YOUR_UNIT, &allunits[0][selectedunits[0][0]]->holding[0]));
	alldisp.push_back(display("Food: ", WIDTH/3+5, 615+0*18, YOUR_UNIT, dispresourcesunit));
	alldisp.push_back(display("Wood: ", WIDTH/3+5, 615+1*18, YOUR_UNIT, dispresourcesunit));
	alldisp.push_back(display("Gold: ", WIDTH/3+5, 615+2*18, YOUR_UNIT, dispresourcesunit));
	alldisp.push_back(display("Stone: ", WIDTH/3+5, 615+3*18, YOUR_UNIT, dispresourcesunit));
	indexResourcedispbuilding=alldisp.size();
	alldisp.push_back(display("Food: ", WIDTH/3+5+355, 615+1*18, YOUR_BUILDING, dispresourcesbuilding));
	alldisp.push_back(display("Wood: ", WIDTH/3+5+355, 615+2*18, YOUR_BUILDING, dispresourcesbuilding));
	alldisp.push_back(display("Gold: ", WIDTH/3+5+355, 615+3*18, YOUR_BUILDING, dispresourcesbuilding));
	alldisp.push_back(display("Stone: ", WIDTH/3+5+355, 615+4*18, YOUR_BUILDING, dispresourcesbuilding));

	alldisp.push_back(display("Health: ", WIDTH/3+5+358, 615, YOUR_UNIT, disphealth));
	alldisp.push_back(display("", WIDTH/3+5+275, 615, YOUR_UNIT, dispname));
	alldisp.push_back(display("Moral: ", WIDTH/3+5+361, 635, YOUR_UNIT, dispmoral));
	alldisp.push_back(display("Food Required: ", WIDTH/3+5+320, 655, YOUR_UNIT, dispfoodrequired));
	alldisp.push_back(display("Health: ", WIDTH/3+5+355, 615, YOUR_BUILDING, dispbuildinghealth));
	alldisp.push_back(display("", WIDTH/3+5+275, 615, YOUR_BUILDING, dispbuildingname));

	indexGarrisondisp=alldisp.size();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			alldisp.push_back(display("", WIDTH/3+5+j*80, 615+22*i, YOUR_BUILDING, dispgarrissoned));
		}
	}
	indexSailorsdisp=alldisp.size();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			alldisp.push_back(display("", WIDTH/3+5+j*80, 615+22*i, YOUR_UNIT | YOUR_SHIP, dispsailors));
		}
	}
	allErr.push_back(ErrorMSG("Not Enough Resources",WIDTH/3+5+80,HEIGHT-16,2.5*FPS));
	allErr.push_back(ErrorMSG("Invalid String",WIDTH/3+5+80,HEIGHT-16,2.5*FPS));
	allErr.push_back(ErrorMSG("Cannot Place Building There",WIDTH/3+5+80,HEIGHT-16,3*FPS));
	allErr.push_back(ErrorMSG("Not Enough Men to Form Regiment",WIDTH/5+80,HEIGHT-16,3.5*FPS));
	allErr.push_back(ErrorMSG("Too Many Men for One Regiment",WIDTH/3+5+80,HEIGHT-16,3.5*FPS));
	allErr.push_back(ErrorMSG("Cannot Transfer to Self",WIDTH/3+5+80,HEIGHT-16,2.5*FPS));
	allErr.push_back(ErrorMSG("Too Many Resources to Transfer",WIDTH/3+5+80,HEIGHT-16,3.5*FPS));
	allErr.push_back(ErrorMSG("Nowhere to Retrieve Resources From",WIDTH/3+5+80,HEIGHT-16,3.5*FPS));
	allErr.push_back(ErrorMSG("Cannot Build This Building Yet", WIDTH/3+5+80,HEIGHT-16,3.0*FPS));
    allErr.push_back(ErrorMSG("Cannot Train This Unit Yet", WIDTH/3+5+80,HEIGHT-16,3.0*FPS));
    allErr.push_back(ErrorMSG("First Select a Garrisoned Unit to Train", WIDTH/3+5+80,HEIGHT-16,4.0*FPS));
    allErr.push_back(ErrorMSG("This has Already Been Researched", WIDTH/3+5+80,HEIGHT-16,3.0*FPS));
    allErr.push_back(ErrorMSG("This is Currently Being Researched Elsewhere", WIDTH/3+5+80,HEIGHT-16,3.5*FPS));
    
    allProgressBar.push_back(progressBar(85,HEIGHT-20,100,"Building", YOUR_UNIT | YOUR_MULT_UNITS, progressBuilding));
    allProgressBar.push_back(progressBar(85,HEIGHT-20,100,"Unit",YOUR_BUILDING,progressUnit));
    allProgressBar.push_back(progressBar(85,HEIGHT-20,100,"Research",YOUR_BUILDING,progressResearch));
    
    vector<advReq> uinfo;
    vector<advReq> binfo;
    uinfo.push_back(advReq(1,6));
    allAdvancements.push_back(advancement(uinfo,binfo,9,UNITUNLOCK));
    
    for(unsigned int i=0; i<advComplete.size(); i++)
        advComplete[i].resize(allAdvancements.size());
    //abuse of error messaging as just regular messaging, with a different message color
    string msg("Congratulations, you have unlocked the ");
    indexAdvErrors=allErr.size();
    for(unsigned int i=0; i<allAdvancements.size(); i++)
        allErr.push_back(ErrorMSG(msg+allAdvancements[i].unlockMessage()+"!!!", WIDTH/3+5,HEIGHT-16,7.5*FPS,RGB(0,170,0)));
    
    //more abuse, this time not for advancements but for researches. Its here because I wanted it to go after the normal error messages.
    indexResearchErrors=allErr.size();
    for(unsigned int i=0; i<allResearches.size(); i++)
        allErr.push_back(ErrorMSG("You have finished researching " + allResearches[i].name, WIDTH/3+5,HEIGHT-30,4.5*FPS,RGB(0,170,0)));
    
	for(int i=0; i<numplayers; i++)
		generals[i]=0;
	int ts=0;
	for(int i=0; i<MAPSIZE; i++)
	{
		for(int j=0; j<MAPSIZE; j++)
		{
			inf3.get(); //open paren
			inf3 >> ts;
			map[i][j].tilestyle=ts;
			inf3.get(); //comma
			inf3 >> map[i][j].elevation;
			map[i][j].elevation-='0';
			inf3.get(); //comma
			inf3 >> map[i][j].resources[0]; //food
			inf3.get(); //comma
			inf3 >> map[i][j].resources[1]; //wood
			inf3.get(); //comma
			inf3 >> map[i][j].resources[2]; //gold
			inf3.get(); //comma
			inf3 >> map[i][j].resources[3]; //stone
			inf3.get(); //close paren
			for(int k=0; k<4; k++)
			{
				map[i][j].resources[k]*=100;
			}
            if(map[i][j].tilestyle==TS_ANIMAL)
                allAnimals.push_back(animal((rand()%31)+40,((float)(rand()%11)/100.0f)+.22,j,i,0,((rand()%30)+24)*FPS));
		}
		inf3.get(); //newline
	}
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<MAPSIZE; j++)
		{
			map[i][j].tilestyle=TS_USELESS;
			map[MAPSIZE-1-i][j].tilestyle=TS_USELESS;
			map[j][i].tilestyle=TS_USELESS;
			map[j][MAPSIZE-1-i].tilestyle=TS_USELESS;
		}
	}
	//End absolutely necessary stuff. Next is unit creation, building creation, etc.
	for(int i=0; i<6; i++)//unit creation
	{
		/*if(i==5)
		{
			actallunits[0].push_back(unit(allbuildableunits[2], ((float)i*4)+5, 5, 0, i, 100, 0, 0, 0, -1, -1,0));
			//actallunits[0].push_back(unit(100, 50, 0, 1, 0, 10, (float)5/25, 25, 0, 0, ((float)i*4)+5, 5, MELEE, 1, 2, 0, i, 2, 2, 1, 10, 2, 0, 0, 100, 0, 0, 0, -1, -1,0, '\001', false, 0, 0, 10));
			allsiegeunits[0].push_back(siege(actallunits[0][i], 1, 4));
		}*/
		if(i==0)
			actallunits[0].push_back(unit(allbuildableunits[0], ((float)i*4)+70, 60, 0, i));
		else
			actallunits[0].push_back(unit(allbuildableunits[1], ((float)i*4)+70, 60, 0, i));
			//actallunits[0].push_back(unit(100, 50, 0, 1, 0, 10, (float)5/25, 25, 0, 0, ((float)i*4)+5, 5, MELEE, 1, 2, 0, i, 1, 1, 1, 10, 1, 0, 0, 100, 0, 0, 0, -1, -1,-1, '\001', false, 0, 0, 10));
		//actallunits[0][i].boundingbox.top=actallunits[0][i].y;
		//actallunits[0][i].boundingbox.left=actallunits[0][i].x;
		//actallunits[0][i].boundingbox.right=actallunits[0][i].x+actallunits[0][i].width;
		//actallunits[0][i].boundingbox.bottom=actallunits[0][i].y+actallunits[0][i].height;
	}
	actallunits[0][0].veterancylvl=3; //TESTING ONLY
	actallunits[0][0].lieutenant=1;   //TESTING ONLY
	//actallunits[0].push_back(unit(allbuildableunits[2], 50, 20, 0, 6));
	allbuildings[0].push_back(building(allbuildablebuildings[1], 85, 65, 0, 0, 68, 65, 85, 88, 500, 500, 500, 500, -2)); //MUST UPDATE BoundingBox
	creationqueueunits[0].resize(1);
	for(float k=allbuildings[0][0].y; k<allbuildings[0][0].y+allbuildings[0][0].height; k+=.25)
	{
		for(float h=allbuildings[0][0].x; h<allbuildings[0][0].x+allbuildings[0][0].width; h+=.25)
		{
			map[(int)k][(int)h].tilestyle=TS_BUILDING;
			map[(int)k][(int)h].buildingplayer=0;
			map[(int)k][(int)h].buildingindex=0;
			map[(int)k][(int)h].whichplayer.set(false, (unsigned char)0);
			map[(int)k][(int)h].whichplayer.set(true, players[0]);
		}
	}
	for(float k=allbuildings[0][0].y-allbuildings[0][0].radiustodistribute; k<allbuildings[0][0].y+allbuildings[0][0].height+allbuildings[0][0].radiustodistribute; k+=.25)
	{
		for(float h=allbuildings[0][0].x-allbuildings[0][0].radiustodistribute; h<allbuildings[0][0].x+allbuildings[0][0].width+allbuildings[0][0].radiustodistribute; h+=.25)
		{
            if(pow(k-allbuildings[0][0].y,2)+pow(h-allbuildings[0][0].x,2)<pow(allbuildings[0][0].radiustodistribute,2))
                map[(int)k][(int)h].inbuildingrad=0;
		}
	}
	for(int i=0; i<5; i++)
	{
		actallunits[1].push_back(unit(allbuildableunits[1], ((float)i)+70, 80, 1, i));
	}
	allobstacles[0].resize(12);
	allobstacles[1].resize(10);
	for(unsigned int i=0; i<actallunits.size(); i++)
	{
		for(unsigned int j=0; j<actallunits[i].size(); j++)
		{
			allunits[i].push_back(&actallunits[i][j]);
			allunits[i][j]->revealmapcreation();
			for(float k=actallunits[i][j].y; k<actallunits[i][j].y+actallunits[i][j].height; k+=.25)
			{
				for(float h=actallunits[i][j].x; h<actallunits[i][j].x+actallunits[i][j].width; h+=.25)
				{
					map[(int)k][(int)h].unitonMineon|=1;
					map[(int)k][(int)h].unitindex=actallunits[i][j].index;
					map[(int)k][(int)h].unitplayer=actallunits[i][j].player;
				}
			}
		}
	}
    inf.close();
    inf2.close();
    inf3.close();
    inf4.close();
	//map[10][10].tilestyle=TS_BERRIES;
	//map[10][10].resources[0]=100;
	//allunits[1][0]->movetoy=5; //makes unit move up

	//SetTimer(hWnd, 50, 1000/FPS, mainTimerProc);
	glutTimerFunc(50,mainTimerProc,0); //NOTE THAT THIS WILL DO IT ONLY ONCE
	//CreateThread(NULL, 0, ThreadTimerProc, NULL, 0, NULL);
	glutTimerFunc(1000,hourpassed,0);
}
void mainTimerProc(int arg)
{
	glutTimerFunc(50,mainTimerProc,0); //NOTE THAT THIS WILL DO IT ONLY ONCE, which is why it is called here. In effect, time delayed recursion.
	frames++;
	RGB green(0,255,0);
	RGB blue(0,0,255);
	RGB magenta(255,0,255);
	RGB yellow(255,255,0);
	RGB yelloworange(250,225,65);
	RGB black(0,0,0);
	RGB red(255,0,0);
	RGB sea(55,160,225);
	RGB grass(35,228,50);
	RGB tree(106,52,43);
	RGB bushes(0,81,0);
	RGB animal(128,128,0);
	RGB road(0,255,255);
	ARGB elevation(100,100,100,100);
	ARGB softyellow(100,255,255,0);
	RGB colors[]={black,red};
	ARGB lightgrey(50,230,230,230);
	ARGB hoverColor(100,200,200,200);
	RGB terminalColor(48,10,36);
	RGB white(255,255,255);
	RGB darkRed(150,0,0);
    RGB mine(143,62,0);
    RGB stone(50,50,50);
	//Setting up openGL
	// Clear Color and Depth Buffers
	glEnable(GL_SCISSOR_TEST);
	glScissor(0,100,WIDTH,HEIGHT-100);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clears the upper part of the screen
	glScissor(0,0,WIDTH,HEIGHT);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	//end setup
	if(redraw>0) 
	{
		glScissor(0,0,WIDTH,100);
		redraw--;
		
		makeRect(0,HEIGHT-100,WIDTH,100,yelloworange);

		drawLine(WIDTH/3,HEIGHT-100,WIDTH/3,HEIGHT,black);
		drawLine(WIDTH*2/3,HEIGHT-100,WIDTH*2/3,HEIGHT,black);
		for(unsigned int i=0; i<allbuttons.size(); i++) // Print buttons
		{
			if(checkdisp(0,allbuttons[i].dispwhen))
			{
				if(((int)i>=indexGarrisonbutton && (int)i<indexGarrisonbuttonend) && alldisp[indexGarrisondisp+i-indexGarrisonbutton].func(indexGarrisondisp+i-indexGarrisonbutton)=="")
					continue;
                if(((int)i>=indexBuildingsbutton && (int)i<indexBuildingsbuttonend) && !buildingAllowed[0][i-indexBuildingsbutton+1]) //not an allowed building
                    continue;
                if((int)i>=indexUnitButtons && (int)i<indexUnitButtonsend && !unitAllowed[0][allbuttons[i].unitorbuilding]) //not an allowed unit
                    continue;
                if((int)i>=indexResearchbutton && (int)i<indexResearchbuttonend && !researchAllowed[0][allbuttons[i].unitorbuilding]) //not an allowed unit
                    continue;
                if((int)i>=indexResearchbutton && (int)i<indexResearchbuttonend) //don't print already researched/currently researching researches
                {
                    bool good=true;
                    for(unsigned int j=0; j<alreadyResearched[0].size(); j++)
                    {
                        if(i-indexResearchbutton==(unsigned int)alreadyResearched[0][j])
                        {
                            good=false;
                            break;
                        }
                    }
                    if(!good)
                        continue;
                    for(unsigned int j=0; j<allCurrResearch[0].size(); j++)
                    {
                        if(i-indexResearchbutton==(unsigned int)allCurrResearch[0][j].researchingWhat)
                        {
                            good=false;
                            break;
                        }
                    }
                    if(!good)
                        continue;
                }
				char *toprint=new char[allbuttons[i].text.size()+1];
				for(unsigned int j=0; j<allbuttons[i].text.size(); j++)
				{
					if(allbuttons[i].text[j]=='_')
						toprint[j]=' ';
					else
						toprint[j]=allbuttons[i].text[j];
				}
				toprint[allbuttons[i].text.size()]=0;
				renderBitmapString(allbuttons[i].x,allbuttons[i].y+12,0,GLUT_BITMAP_HELVETICA_12,toprint);
				//temp.DrawString(toprint,allbuttons[i].text.size(),&bigfont,Gdiplus::PointF(Gdiplus::REAL(allbuttons[i].x), Gdiplus::REAL(allbuttons[i].y)),&black);
				//temp.DrawRectangle(&(p[0]),allbuttons[i].x,allbuttons[i].y,allbuttons[i].width,allbuttons[i].height);
                drawEmptyRect(allbuttons[i].x,allbuttons[i].y,allbuttons[i].width,allbuttons[i].height,black);
                if((int)i>=indexGarrisonbutton && (int)i<indexGarrisonbuttonend)
                {
                    bool good=false;
                    for(unsigned int j=0; j<allgarrisonedselectedunits[0].size(); j++)
                    {
                        if(allgarrisonedselectedunits[0][j]==allbuildings[0][selectedunits[0][0]].garrisoned[i-indexGarrisonbutton])
                        {
                            good=true;
                            break;
                        }
                    }
                    if(good)
                        drawEmptyRect(allbuttons[i].x,allbuttons[i].y,allbuttons[i].width,allbuttons[i].height,blue);
                }
				delete[] toprint;
			}
		}
		for(unsigned int i=0; i<alldisp.size(); i++) //Print things in disp
		{
			if(checkdisp(0,alldisp[i].dispwhen))
			{
				string var=alldisp[i].func(i);
				char *toprint=new char[alldisp[i].text.size()+var.size()+1];
				for(unsigned int j=0; j<alldisp[i].text.size(); j++)
				{
					if(alldisp[i].text[j]=='_')
						toprint[j]=' ';
					else
						toprint[j]=alldisp[i].text[j];
				}
				for(unsigned int j=0; j<var.size(); j++)
				{
					if(var[j]=='_')
						toprint[j]=' ';
					else
						toprint[j+alldisp[i].text.size()]=var[j];
				}
				toprint[alldisp[i].text.size()+var.size()]=0;
				//temp.DrawString(toprint,alldisp[i].text.size()+var.size(), &bigfont, Gdiplus::PointF(Gdiplus::REAL(alldisp[i].x),Gdiplus::REAL(alldisp[i].y)),&black);
				renderBitmapString(alldisp[i].x,alldisp[i].y+12,0,GLUT_BITMAP_HELVETICA_12,toprint);
				delete[] toprint;
			}
		}
        for(unsigned int i=0; i<allProgressBar.size(); i++)
        {
            if(checkdisp(0,allProgressBar[i].dispwhen)) //must include a select a unit or building or something clause in disp_when, else this will fail
            {
                double progress=allProgressBar[i].func(selectedunits[0][0]);
                if(!(abs(progress+1)<0.0001)) //!(progress==-1)
                {
                    drawEmptyRect(allProgressBar[i].x,allProgressBar[i].y,allProgressBar[i].width,18,black);
                    makeRect(allProgressBar[i].x, allProgressBar[i].y,allProgressBar[i].width*progress,17,green);
                    renderBitmapString(allProgressBar[i].x+3, allProgressBar[i].y+12, 0, GLUT_BITMAP_HELVETICA_12, (char*)allProgressBar[i].text.c_str());
                }
            }
        }
		if(transferResourcesPressed>0)
		{
			makeRect(0,HEIGHT-16,WIDTH,16,terminalColor);
			char *toprint=new char[transferResourcesTyped.length()+1];
			for(unsigned int i=0; i<transferResourcesTyped.length(); i++)
				toprint[i]=transferResourcesTyped[i];
			toprint[transferResourcesTyped.length()]=0;
			renderBitmapString(5,HEIGHT-3,0,GLUT_BITMAP_HELVETICA_12,toprint,white);
			delete[] toprint;
		}
		if(currErr.msg!="" && currErr.time>0)
		{
			char *toprint=new char[currErr.msg.length()+1];
			for(unsigned int i=0; i<currErr.msg.length(); i++)
				toprint[i]=currErr.msg[i];
			toprint[currErr.msg.length()]=0;
			renderBitmapString(currErr.x, currErr.y+13, 0, GLUT_BITMAP_HELVETICA_12, toprint, currErr.color);
		}
	}
	if(currErr.msg!="" && currErr.time>0) //update time left for error message
	{
		currErr.time--;
		if(currErr.time<=0)
			redraw=1;
	}
    if((scrollDir&LEFT)==LEFT && topleft.x>0) //scroll
        topleft.x-=1;
    else if((scrollDir&RIGHT)==RIGHT && topleft.x<((map[0].size()*15)-WIDTH)/15)
        topleft.x+=1;
    if((scrollDir&DOWN)==DOWN && topleft.y>0)
        topleft.y-=.5;
    else if((scrollDir&UP)==UP && topleft.y<((map.size()*15)-HEIGHT)/15)
        topleft.y+=.5; //end scroll
	glScissor(0,100,WIDTH,HEIGHT-100);
	for(unsigned int g=0; g<creationqueueunits.size(); g++)//add units
	{
		for(unsigned int h=0; h<creationqueueunits[g].size(); h++)
		{
			for(unsigned int f=0; f<creationqueueunits[g][h].size(); f++)
			{
				creationqueueunits[g][h][f].timeremaining--;
				if(creationqueueunits[g][h][f].timeremaining<=0)
				{
					allbuildings[g][h].createunit(creationqueueunits[g][h][f].createwhat, creationqueueunits[g][h][f].pindex,f);
					creationqueueunits[g][h].erase(creationqueueunits[g][h].begin()+f);
				}
			}
		}
	}//end create units
	for(unsigned int i=0; i<allunits.size(); i++)//go through allunits - movement, training
	{
		for(unsigned int j=0; j<allunits[i].size(); j++)
		{
			// below is to make enemy move around
			/*if(allunits[1][0]->y>=29 && allunits[1][0]->y<=31)//delete all up until next 'delete'
				allunits[1][0]->movetoy=10;
			else if(allunits[1][0]->y>=9 && allunits[1][0]->y<=11)
				allunits[1][0]->movetoy=30;//delete*/
			allunits[i][j]->movement();
			if(allunits[i][j]->training==true)
				allunits[i][j]->train();
			if(allunits[i][j]->lieuttrain==true)
				allunits[i][j]->trainlieut();
			if(allunits[i][j]->exp>=(allunits[i][j]->veterancylvl+1)*(allunits[i][j]->veterancylvl+1)*1500*(allbuildableunits[allunits[i][j]->id].trainingcof/100.0f))
			{
				allunits[i][j]->exp=0;
				allunits[i][j]->veterancylvl++;
			}
			if((allunits[i][j]->lieutenant==0 && allunits[i][j]->lieutexp>=(360*FPS)/log(3*(allunits[i][j]->veterancylvl-3)-e*(allunits[i][j]->veterancylvl-4))) || (allunits[i][j]->lieutenant>0 && allunits[i][j]->lieutexp>=(allunits[i][j]->lieutenant+1)*(allunits[i][j]->lieutenant+1)*1500*1.5f))
			{
				allunits[i][j]->lieutenant++;
				allunits[i][j]->lieutexp=0;
			}
            if(allunits[i][j]->selfRecording)
            {
                vector<pointex> allseenunits;
                int addLOS=0;
                for(unsigned int h=0; h<alreadyResearched[i].size(); h++) //apply research bonuses
                {
                    if(allResearches[alreadyResearched[i][h]].checkResearch(allunits[i][j]->id))
                        addLOS+=allResearches[alreadyResearched[i][h]].los;
                }
                allunits[i][j]->checkrad((int)allunits[i][j]->los+addLOS-tilecameo[map[(unsigned int)allunits[i][j]->y][(unsigned int)allunits[i][j]->x].tilestyle],allunits[i][j]->x,allunits[i][j]->y,allseenunits);
                allunits[i][j]->rep.updateseenunits(allseenunits);
            }
		}
	}//end go through allunits
	for(unsigned int i=0; i<newlybuiltbuildings.size(); i++) //new buildings
	{
		for(unsigned int j=0; j<newlybuiltbuildings[i].size(); j++)
		{
			allbuildings[i][newlybuiltbuildings[i][j]].beingbuilt--;
			if(allbuildings[i][newlybuiltbuildings[i][j]].beingbuilt==-2)
			{
				if(allbuildings[i][newlybuiltbuildings[i][j]].id==18) //road
				{
					for(float k=allbuildings[i][newlybuiltbuildings[i][j]].y; k<allbuildings[i][newlybuiltbuildings[i][j]].y+allbuildings[i][newlybuiltbuildings[i][j]].height; k+=.25)
					{
						for(float h=allbuildings[i][newlybuiltbuildings[i][j]].x; h<allbuildings[i][newlybuiltbuildings[i][j]].x+allbuildings[i][newlybuiltbuildings[i][j]].width; h+=.25)
						{
							map[(int)k][(int)h].tilestyle=TS_ROAD;
						}
					}
					allbuildings[i].erase(allbuildings[i].begin()+newlybuiltbuildings[i][j]);
				}
				else if(allbuildings[i][newlybuiltbuildings[i][j]].id==19) //gate vert
				{
					for(float k=allbuildings[i][newlybuiltbuildings[i][j]].y+1; k<allbuildings[i][newlybuiltbuildings[i][j]].y+allbuildings[i][newlybuiltbuildings[i][j]].height-1; k+=.25)
					{
						for(float h=allbuildings[i][newlybuiltbuildings[i][j]].x; h<allbuildings[i][newlybuiltbuildings[i][j]].x+allbuildings[i][newlybuiltbuildings[i][j]].width; h+=.25)
						{
							map[(int)k][(int)h].tilestyle=TS_GATE;
						}
					}
				}
				else if(allbuildings[i][newlybuiltbuildings[i][j]].id==20) //gate horiz
				{
					for(float k=allbuildings[i][newlybuiltbuildings[i][j]].y; k<allbuildings[i][newlybuiltbuildings[i][j]].y+allbuildings[i][newlybuiltbuildings[i][j]].height; k+=.25)
					{
						for(float h=allbuildings[i][newlybuiltbuildings[i][j]].x+1; h<allbuildings[i][newlybuiltbuildings[i][j]].x+allbuildings[i][newlybuiltbuildings[i][j]].width-1; h+=.25)
						{
							map[(int)k][(int)h].tilestyle=TS_GATE;
						}
					}
				}
                else if(allbuildings[i][newlybuiltbuildings[i][j]].id==23) //mine
                {
                    for(float k=allbuildings[i][newlybuiltbuildings[i][j]].y; k<allbuildings[i][newlybuiltbuildings[i][j]].y+allbuildings[i][newlybuiltbuildings[i][j]].height; k+=.25)
					{
						for(float h=allbuildings[i][newlybuiltbuildings[i][j]].x; h<allbuildings[i][newlybuiltbuildings[i][j]].x+allbuildings[i][newlybuiltbuildings[i][j]].width; h+=.25)
						{
                            if((map[(int)k][(int)h].unitonMineon>>3)>0)
							{
                                map[(int)k][(int)h].unitonMineon|=2;
                                map[(int)k][(int)h].tilestyle=(map[(int)k][(int)h].unitonMineon>>3);
                                map[(int)k][(int)h].unitonMineon&=3; //sets everything except for the rightmost 2 bits to 0. IF I EVER USE THIS VARIABLE FOR ANYTHING ELSE, MODIFY THIS TO REFLECT THAT!!
                            }
						}
					}
                }
				newlybuiltbuildings[i].erase(newlybuiltbuildings[i].begin()+j);
			}
		}
	}//end new buildings
    
    //Update researching new things
    for(unsigned int i=0; i<allCurrResearch.size(); i++)
    {
        for(unsigned int j=0; j<allCurrResearch[i].size(); j++)
        {
            if(allbuildings[i][allCurrResearch[i][j].researchingWhere].health<=0) //building destroyed
            {
                allCurrResearch[i].erase(allCurrResearch[i].begin()+j);
                j--;
                continue;
            }
            allCurrResearch[i][j].timeleft--;
            if(allCurrResearch[i][j].timeleft<0) //research complete
            {
                alreadyResearched[i].push_back(allCurrResearch[i][j].researchingWhat);
                allCurrResearch[i].erase(allCurrResearch[i].begin()+j);
                currErr=allErr[indexResearchErrors+allCurrResearch[i][j].researchingWhat];
                j--;
                redraw=1;
            }
        }
    } //end researching
    
	if(frames%(FPS/5)==0) //Every fifth of a second. This just makes it update relatively infrequently, as more common ones are not needed
	{
		for(unsigned int i=0; i<allregiments.size(); i++) //updates reports of all regiments
		{
			for(unsigned int j=0; j<allregiments[i].size(); j++)
			{
				if(allregiments[i][j].recording==true) // if it is recording, and therefore needs updating
				{
					for(unsigned int k=0; k<allregiments[i][j].unitids.size(); k++)
					{
						double dist=pow(allunits[i][allregiments[i][j].lieutenant]->x-allunits[i][allregiments[i][j].unitids[k]]->x,2)+pow(allunits[i][allregiments[i][j].lieutenant]->y-allunits[i][allregiments[i][j].unitids[k]]->y,2);
						if(dist>DIST_TO_REG)
						{
							if(allunits[i][allregiments[i][j].unitids[k]]->recording==false)
								continue;
							else
							{
								allunits[i][allregiments[i][j].unitids[k]]->rep=allregiments[i][j].rep;
								allunits[i][allregiments[i][j].unitids[k]]->recording=false;
                                allunits[i][allregiments[i][j].unitids[k]]->selfRecording=true;                                
								continue;
							}
						}
						else
						{
                            if(!allunits[i][allregiments[i][j].unitids[k]]->recording)
                            {
                                allunits[i][allregiments[i][j].unitids[k]]->recording=true;
                                allunits[i][allregiments[i][j].unitids[k]]->selfRecording=false;
                                allregiments[i][j].rep.merge(allunits[i][allregiments[i][j].unitids[k]]->rep);
                            }
							vector<pointex> allseenunits;
                            int addLOS=0;
                            for(unsigned int h=0; h<alreadyResearched[i].size(); h++) //apply research bonuses
                            {
                                if(allResearches[alreadyResearched[i][h]].checkResearch(allunits[i][allregiments[i][j].unitids[k]]->id))
                                    addLOS+=allResearches[alreadyResearched[i][h]].los;
                            }
							allunits[i][allregiments[i][j].unitids[k]]->checkrad((int)allunits[i][allregiments[i][j].unitids[k]]->los+addLOS-tilecameo[map[allunits[i][allregiments[i][j].unitids[k]]->y][allunits[i][allregiments[i][j].unitids[k]]->x].tilestyle],allunits[i][allregiments[i][j].unitids[k]]->x,allunits[i][allregiments[i][j].unitids[k]]->y,allseenunits);
							allregiments[i][j].rep.updateseenunits(allseenunits);
						}
					}
				}
			}
		}
        for(unsigned int i=0; i<allAnimals.size(); i++) //move all of the animals
        {
            if(allAnimals[i].health<=0)
                continue;
            allAnimals[i].move();
        }
	}
    if(frames%(FPS/2)==0) //every half second - not super critical and could get rather slow
    {
        for(unsigned int i=0; i<allAdvancements.size(); i++) //loop through advancements
        {
            for(unsigned int j=0; j<advComplete.size(); j++) //loop through all of the players
            {
                if(!advComplete[j][i] && (advComplete[j][i]=allAdvancements[i].checkAdv(j)) && j==0) //if this advancement has not been accomplished by this player, and then this advancement has now been accomplished (record that) and then this is player 1, the human, display that. An "error" (abuse of error messaging system).
                {
                    redraw=1;
                    currErr=allErr[indexAdvErrors+i];
                }
            }
        }
    }
    //Below draws things
	for(unsigned int i=(int)topleft.y; i<topleft.y+HEIGHT/15; i++) //loop through map, print tiles
	{
		for(unsigned int j=(int)topleft.x; j<topleft.x+WIDTH/15; j++)
		{
			if(minimapseen[0][i][j]==false)//unknown part of the map
				continue; //don't bother printing the tile
			if(j>=MAPSIZE)
				break;
			if(i>=MAPSIZE)
				break;
            if(map[i][j].tilestyle==TS_GOLD && (map[i][j].unitonMineon&2)==2) //There's gold and a mine
                makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,yellow);
            else if(map[i][j].tilestyle==TS_STONE && (map[i][j].unitonMineon&2)==2) //There's stone and a mine
                makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,stone);
            else if((map[i][j].unitonMineon&2)==2) //theres a mine
                makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,mine);
			else if(map[i][j].tilestyle==TS_BERRIES) //print berries
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,red);
				//g.FillRectangle(&red,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f);
			else if(map[i][j].tilestyle==TS_WATER || map[i][j].tilestyle==TS_FISH || map[i][j].tilestyle==TS_WATERBUILDING) //print water and fish as the same thing
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,sea);
				//g.FillRectangle(&sea,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f);
			else if(map[i][j].tilestyle==TS_GRASS || map[i][j].tilestyle==TS_STONE || map[i][j].tilestyle==TS_GOLD || map[i][j].tilestyle==TS_BUILDING || map[i][j].tilestyle==TS_GATE)
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,grass);
				//g.FillRectangle(&grass,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f); //print grass, gold, stone, and buildings
			else if(map[i][j].tilestyle==TS_TREE)
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,tree);
				//g.FillRectangle(&tree,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f); //print trees
			else if(map[i][j].tilestyle==TS_BUSHES)
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,bushes);
				//g.FillRectangle(&bushes,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f); //print bushes
			else if(map[i][j].tilestyle==TS_ANIMAL)
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,animal);
				//g.FillRectangle(&animal,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f); //print animals
			else if(map[i][j].tilestyle==TS_ROAD)
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,road);
				//g.FillRectangle(&road,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f); //print roads
			else if(map[i][j].tilestyle==TS_USELESS)
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,black);
				//g.FillRectangle(&black,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f); //print void
			for(int k=0; k<map[i][j].elevation; k++)
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,elevation);
				//g.FillRectangle(&elevation,Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f); //print elevation
		}
	}
	for(int i=(int)topleft.y; i<topleft.y+HEIGHT/15; i++) //loop through map, to print, only print stuff for units and buildings
	{
		for(int j=(int)topleft.x; j<topleft.x+WIDTH/15; j++)
		{
			if(j>=MAPSIZE || i>=MAPSIZE || i<0 || j<0)
				break;
			if(minimapseen[0][i][j]==false)//color unknown parts of map
			{
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15.0,15.0,black);
				//g.FillRectangle(&black, Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f);
				continue; //Its unknown, so don't print stuff over this
			}//end color unknown parts of map
			float dist=sqrt(pow(allunits[0][generals[0]]->x-j,2)+pow(allunits[0][generals[0]]->y-i,2));
			if((map[i][j].tilestyle==TS_WATERBUILDING || map[i][j].tilestyle==TS_BUILDING) && minimapseen[0][i][j]==true)//print buildings, only if visible
			{	
				int index=map[i][j].buildingindex;
				int player=map[i][j].buildingplayer;
				if((int)allbuildings[player][index].x==j && (int)allbuildings[player][index].y==i)//print only once
				{
					if(allbuildings[player][index].health>0)//don't print dead things
					{
						char *text=new char[allbuildablebuildings[allbuildings[player][index].id].name.size()+1];
						for(unsigned int k=0; k<allbuildablebuildings[allbuildings[player][index].id].name.size(); k++)
						{
							if(allbuildablebuildings[allbuildings[player][index].id].name[k]=='_')
								text[k]=' ';
							else
								text[k]=allbuildablebuildings[allbuildings[player][index].id].name[k];
						}
						text[allbuildablebuildings[allbuildings[player][index].id].name.size()]=0;
						renderBitmapString((j-topleft.x)*15+2, (i-topleft.y)*15+9,0,GLUT_BITMAP_HELVETICA_12,text);
						//g.DrawString(text,allbuildablebuildings[allbuildings[player][index].id].name.size(),&bigfont,Gdiplus::PointF(Gdiplus::REAL((j-topleft.x)*15+2), Gdiplus::REAL((i-topleft.y)*15+2)),&black);
						if(allbuildings[player][index].beingbuilt<0)
						{
							makeEllipse((float)(j+allbuildings[player][index].width/2-topleft.x)*15, (float)(i+allbuildings[player][index].height/2-topleft.y)*15, ((allbuildings[player][index].width)/2+allbuildings[player][index].radiustodistribute)*15, ((allbuildings[player][index].height)/2+allbuildings[player][index].radiustodistribute)*15, softyellow);
							//g.FillEllipse(&softyellow, Gdiplus::REAL(j-allbuildings[player][index].radiustodistribute-topleft.x)*15, Gdiplus::REAL(i-allbuildings[player][index].radiustodistribute-topleft.y)*15, (allbuildings[player][index].width+(2*allbuildings[player][index].radiustodistribute))*15, (allbuildings[player][index].height+(2*allbuildings[player][index].radiustodistribute))*15);
							if(allbuildings[player][index].selected==false && player==0)
								drawEmptyRect((j-topleft.x)*15,(i-topleft.y)*15,allbuildings[player][index].width*15, allbuildings[player][index].height*15,magenta);
								//g.DrawRectangle(&(p[4]), (Gdiplus::REAL)(j-topleft.x)*15, (Gdiplus::REAL)(i-topleft.y)*15, (Gdiplus::REAL)allbuildings[player][index].width*15, (Gdiplus::REAL)allbuildings[player][index].height*15);
							else
								drawEmptyRect((j-topleft.x)*15,(i-topleft.y)*15,allbuildings[player][index].width*15, allbuildings[player][index].height*15,blue);
								//g.DrawRectangle(&(p[3]), (Gdiplus::REAL)(j-topleft.x)*15, (Gdiplus::REAL)(i-topleft.y)*15, (Gdiplus::REAL)allbuildings[player][index].width*15, (Gdiplus::REAL)allbuildings[player][index].height*15);
						}
						else
							drawEmptyRect((j-topleft.x)*15,(i-topleft.y)*15,allbuildings[player][index].width*15, allbuildings[player][index].height*15,yellow);
							//g.DrawRectangle(&(p[5]), (Gdiplus::REAL)(j-topleft.x)*15, (Gdiplus::REAL)(i-topleft.y)*15, (Gdiplus::REAL)allbuildings[player][index].width*15, (Gdiplus::REAL)allbuildings[player][index].height*15);
						delete[] text;
					}
				}
			}//end print buildings
			if(map[i][j].tilestyle!=TS_BUILDING && (map[i][j].unitonMineon&1)==1 && allunits[0][generals[0]]->los>dist)//print units
			{
				int index=map[i][j].unitindex;
				int player=map[i][j].unitplayer;
				if((int)allunits[player][index]->x==j && (int)allunits[player][index]->y==i)
				{
					if(allunits[player][index]->health>0)
					{
						char text=allunits[player][index]->veterancylvl+'0';
						glColor3f(0.0,0.0,0.0);
						glDisable(GL_TEXTURE_2D);
						glRasterPos3f(((allunits[player][index]->x-topleft.x)*15.0+2)*2.0/WIDTH,2.0-((allunits[player][index]->y-topleft.y)*15.0+10)*2.0/HEIGHT,0);
//						glRasterPos3f((float)x*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,(float)z);

						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,text);
						glEnable(GL_TEXTURE_2D);
						//g.DrawString(text, 1, &font, PointF((Gdiplus::REAL)(allunits[player][index]->x-topleft.x)*15, (Gdiplus::REAL)(allunits[player][index]->y-topleft.y)*15), &black);
						if(allunits[player][index]->selected==false)
							drawEmptyRect((allunits[player][index]->x-topleft.x)*15,(allunits[player][index]->y-topleft.y)*15,allunits[player][index]->width*15,allunits[player][index]->height*15,colors[player]);
							//g.DrawRectangle(&(p[player]), (Gdiplus::REAL)(allunits[player][index]->x-topleft.x)*15, (Gdiplus::REAL)(allunits[player][index]->y-topleft.y)*15, (Gdiplus::REAL)allunits[player][index]->width*15, (Gdiplus::REAL)allunits[player][index]->height*15);
						else
						{
							//Pen p[6]={Color(0,0,0), Color(255,0,0), Color(0,255,0), Color(0,0,255), Color(255,0,255), Color(255,255,0)};
							drawEmptyRect((allunits[player][index]->x-topleft.x)*15,(allunits[player][index]->y-topleft.y)*15,allunits[player][index]->width*15,allunits[player][index]->height*15,blue);
							drawEmptyRect((allunits[player][index]->x-topleft.x)*15,(allunits[player][index]->y-topleft.y)*15-3,15,3,black);
							makeRect((allunits[player][index]->x-topleft.x)*15,(allunits[player][index]->y-topleft.y)*15-3,allunits[player][index]->health/allbuildableunits[allunits[player][index]->id].health*15,3,green);
							//g.DrawRectangle(&(p[3]), (Gdiplus::REAL)(allunits[player][index]->x-topleft.x)*15, (Gdiplus::REAL)(allunits[player][index]->y-topleft.y)*15, (Gdiplus::REAL)allunits[player][index]->width*15, (Gdiplus::REAL)allunits[player][index]->height*15);
							//g.DrawRectangle(&(p[0]), (Gdiplus::REAL)(allunits[player][index]->x-topleft.x)*15, (Gdiplus::REAL)(allunits[player][index]->y-topleft.y)*15-3, (Gdiplus::REAL)15, (Gdiplus::REAL)3);
							//g.FillRectangle(&sb, (Gdiplus::REAL)(allunits[player][index]->x-topleft.x)*15, (Gdiplus::REAL)(allunits[player][index]->y-topleft.y)*15-3, (Gdiplus::REAL)(allunits[player][index]->health/allbuildableunits[allunits[player][index]->id].health*15), (Gdiplus::REAL)3);
						}
						if(allunits[player][index]->siegeindex!=-1 && allunits[player][index]->whatisit!=2)
							makeRect((allunits[player][index]->x-topleft.x)*15,(allunits[player][index]->y-topleft.y)*15,allunits[player][index]->width*15,allunits[player][index]->height*15,green);
							//g.FillRectangle(&sb, (Gdiplus::REAL)(allunits[player][index]->x-topleft.x)*15, (Gdiplus::REAL)(allunits[player][index]->y-topleft.y)*15, (Gdiplus::REAL)allunits[player][index]->width*15, (Gdiplus::REAL)allunits[player][index]->height*15);
					}
				}
			}//end print units
			if(minimapseen[0][i][j]==false)//color unknown parts of map
			{
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15,15,black);
				//g.FillRectangle(&black, Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f);
			}//end color unknown parts of map
			if(allunits[0][generals[0]]->los<dist) //far away
				makeRect((j-topleft.x)*15,(i-topleft.y)*15,15,15,lightgrey);
				//g.FillRectangle(&lightgrey, Gdiplus::REAL(j-topleft.x)*15, Gdiplus::REAL(i-topleft.y)*15, 15.0f, 15.0f);
		}
	}
	if(mousex2!=uptomousex2 && mousey2!=uptomousey2 && lbuttondown==true)//selection box
	{
		if(uptomousex2-mousex2>0 && uptomousey2-mousey2>0)
			drawEmptyRect(mousex2-topleft.x*15, mousey2-topleft.y*15, uptomousex2-mousex2, uptomousey2-mousey2, black);
			//g.DrawRectangle(&(p[0]), mousex-topleft.x*15, mousey-topleft.y*15, uptomousex-mousex, uptomousey-mousey);
		else if(uptomousex2-mousex2<0 && uptomousey2-mousey2>0)
			drawEmptyRect(uptomousex2-topleft.x*15, mousey2-topleft.y*15, mousex2-uptomousex2, uptomousey2-mousey2, black);
			//g.DrawRectangle((&p[0]), uptomousex-topleft.x*15, mousey-topleft.y*15, mousex-uptomousex, uptomousey-mousey);
		else if(uptomousex2-mousex2>0 && uptomousey2-mousey2<0)
			drawEmptyRect(mousex2-topleft.x*15, uptomousey2-topleft.y*15, uptomousex2-mousex2, mousey2-uptomousey2, black);
			//g.DrawRectangle((&p[0]), mousex-topleft.x*15, uptomousey-topleft.y*15, uptomousex-mousex, mousey-uptomousey);
		else if(uptomousex2-mousex2<0 && uptomousey2-mousey2<0)
			drawEmptyRect(uptomousex2-topleft.x*15, uptomousey2-topleft.y*15, mousex2-uptomousex2, mousey2-uptomousey2, black);
			//g.DrawRectangle(&(p[0]), uptomousex-topleft.x*15, uptomousey-topleft.y*15, mousex-uptomousex, mousey-uptomousey);
	}//end selection box
	if(buildinghover==true)
		makeRect((int)(currmousex2-((int)currmousex2%15)),(int)(currmousey2-((int)currmousey2%15)),buildingwidth*15,buildingheight*15, hoverColor);
		//g.FillRectangle(&hover,mouse.x-mouse.x%15,mouse.y-mouse.y%15,buildingwidth*15,buildingheight*15);
	//BitBlt(hdcBuf2,0,0,client.right,client.bottom,hdcBuf,0,0,SRCAND);
	//BitBlt(hdc,0,0,client.right,client.bottom,hdcBuf,0,0,SRCCOPY);

/*	glColor3f(0.0, 255.0/255.0, 0);
	glBegin(GL_QUADS);
		glVertex3f( 0,  1,0);
		glVertex3f( 2,  1,0);
		glVertex3f( 2,  2,0);
		glVertex3f( 0,  2,0);
	glEnd();*/
    for(unsigned int i=0; i<allbuttons.size(); i++) // Print mouse over text, if any
    {
        if(checkdisp(0,allbuttons[i].dispwhen))
        {
            if(((int)i>=indexGarrisonbutton && (int)i<indexGarrisonbuttonend) && alldisp[indexGarrisondisp+i-indexGarrisonbutton].func(indexGarrisondisp+i-indexGarrisonbutton)=="")
                continue;
            if(((int)i>indexBuildingsbutton && (int)i<indexBuildingsbuttonend) && !buildingAllowed[0][i-indexBuildingsbutton+1]) //not an allowed building
                continue;
            if((int)i>=indexUnitButtons && (int)i<indexUnitButtonsend && !unitAllowed[0][allbuttons[i].unitorbuilding]) //not an allowed unit
                continue;
            if((int)i>=indexResearchbutton && (int)i<indexResearchbuttonend && !researchAllowed[0][allbuttons[i].unitorbuilding]) //not an allowed unit
                continue;
            if((int)i>=indexResearchbutton && (int)i<indexResearchbuttonend) //don't print mouseovers for already researched/currently researching researches
            {
                bool good=true;
                for(unsigned int j=0; j<alreadyResearched[0].size(); j++)
                {
                    if(i-indexResearchbutton==(unsigned int)alreadyResearched[0][j])
                    {
                        good=false;
                        break;
                    }
                }
                if(!good)
                    continue;
                for(unsigned int j=0; j<allCurrResearch[0].size(); j++)
                {
                    if(i-indexResearchbutton==(unsigned int)allCurrResearch[0][j].researchingWhat)
                    {
                        good=false;
                        break;
                    }
                }
                if(!good)
                    continue;
            }
            if(mousex>allbuttons[i].x && mousex<allbuttons[i].x+allbuttons[i].width && mousey>allbuttons[i].y && mousey<allbuttons[i].y+allbuttons[i].height)
            {
                for(unsigned int j=0; j<allMouseOver.size(); j++)
                {
                    if(allMouseOver[j].dispwith==i)
                    {
                        string ret=allMouseOver[j].func(j);
                        char *toprint=new char[allMouseOver[j].text.length()+ret.length()+1];
                        for(unsigned int k=0; k<allMouseOver[j].text.length(); k++)
                            toprint[k]=allMouseOver[j].text[k];
                        for(unsigned int k=0; k<ret.length(); k++)
                            toprint[allMouseOver[j].text.length()+k]=ret[k];
                        toprint[allMouseOver[j].text.length()+ret.length()]=0;
                        renderBitmapString(allMouseOver[j].x,allMouseOver[j].y,0,GLUT_BITMAP_HELVETICA_12,toprint,white);
                        delete[] toprint;
                        break;
                    }
                }
            }
        }
    }
	glScissor(0,0,WIDTH,HEIGHT);
	glutSwapBuffers();
}
int processResources(string input, char search, int len)
{
	for(unsigned int i=0; i<input.length(); i++)
	{
		if(input[i]==search || input[i]==search+32) //search is uppercase, checks lowercase too
		{
			if(i+len<input.length() && (input[i+1]<'0' || input[i+1]>'9')) //word, i.e. food/wood/gold/stone, in range
			{
				i+=len; //go to where number should be.
				string num;
				for(unsigned int j=i; j<input.length(); j++)
				{
					if(input[j]>='0' && input[j]<='9') //number
						num+=input[j];
					else
						break;
				}
				if(num.length()==0)
					return 0;
				else
					return atoi(num.c_str());
			}
			else if(i+1<input.length() && (input[i+1]>='0' && input[i+1]<='9')) //letter followed by number
			{
				i+=1;
				string num;
				for(unsigned int j=i; j<input.length(); j++)
				{
					if(input[j]>='0' && input[j]<='9') //number
						num+=input[j];
					else
						break;
				}
				if(num.length()==0)
					return 0;
				else
					return atoi(num.c_str());
			}
		}
	}
	return 0;
}
bool transferResources(int player, string input, int bindex1, int bindex2)
{
	if(bindex1==bindex2) //transfering to self
	{
		if(player==0)
		{
			currErr=allErr[SELFTRANSFER];
			redraw=1;
		}
		return false;
	}
	if(input.length()==0) //null string
	{
		if(player==0)
		{
			currErr=allErr[INVALIDSTRING];
			redraw=1;
		}
		return false;
	}
	short totrans[4];
	int sumtrans=0;
	int sumheld=0;
	totrans[0]=processResources(input,'F',4);
	totrans[1]=processResources(input,'W',4);
	totrans[2]=processResources(input,'G',4);
	totrans[3]=processResources(input,'S',5);
	for(int i=0; i<4; i++)
	{
		if(allbuildings[player][bindex1].holding[i]>=totrans[i]) //enough stored to transfer that much
			sumtrans+=totrans[i];
		else
		{
			if(player==0)
			{
				currErr=allErr[FEWRESOURCES];
				redraw=1;
			}
			return false; //Cannot transfer that much: failed
		}	
		sumheld+=allbuildings[player][bindex2].holding[i];
	}
	if(sumtrans==0) //trying to transfer 0
	{
		if(player==0)
		{
			currErr=allErr[INVALIDSTRING];
			redraw=1;
		}
		return false;
	}
	if(sumheld+sumtrans>allbuildings[player][bindex2].maxhold) //The resources to be stored will not fit in the destination building
	{
		if(player==0)
		{
			currErr=allErr[TOOBIGTRANSFER];
			redraw=1;
		}
		return false; //failed
	}
	for(int i=0; i<4; i++)
		allbuildings[player][bindex1].transfer[i]=totrans[i];
	for(unsigned int i=0; i<selectedunits[player].size(); i++)
	{
		allunits[player][selectedunits[player][i]]->transferring=1;
		allunits[player][selectedunits[player][i]]->transferfrom=bindex1;
		allunits[player][selectedunits[player][i]]->transferto=bindex2;
		allunits[player][selectedunits[player][i]]->movetox=allbuildings[player][bindex1].x;
		allunits[player][selectedunits[player][i]]->movetoy=allbuildings[player][bindex1].y;
	}
	return true;
}
void dialogProc(int arg)
{
    if(makeReportDialog==false)
    {
        glutDestroyWindow(reportDialogWindow);
        glutSetWindow(mainWindow);
        return;
    }
    glutTimerFunc(100,dialogProc,0); //NOTE THAT THIS WILL DO IT ONLY ONCE, which is why it is called here. In effect, time delayed recursion.
    renderReportDialog();
}
