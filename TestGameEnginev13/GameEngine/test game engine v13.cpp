#include "globals.h"
#include "data structures.h"
#include "tile.h"
#include "base unit buildings.h"
#include "building.h"
#include "unit.h"
#include "siege.h"
#include "regiment.h"
#include "button.h"
#include "astarsearch.h"
#include "button functions.h"
#include "display functions.h"

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

#define WIDTH 700 //temporary, I hope
#define HEIGHT 700 //temporary, I hope
#define DBL_CLICK 3
// Global Variables:
int dblclickglut=-1;
/*HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HDC hdc;
HDC hdcBuf;
 
HBITMAP hbmBackBM;
HBITMAP hbmOldBackBM;
HWND buttons[2];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
VOID CALLBACK mainTimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);*/
void mainTimerProc(int arg);
void renderScene();
void initializeGameEngine();
void changeSize(int w, int h);
void processMouse(int button, int state, int x, int y);
void processMouseMove(int x, int y);
void processPassiveMouseMove(int x, int y);

struct POINT
{
	int x;
	int y;
};

vector<point> astarsearch(problem p, float speed, unit *u)
{
	vector<searchfringe> fringe; //poss make this a heap
	vector<point> empty;
	fringe.push_back(searchfringe(point(u->x,u->y),0,&empty));
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
			fringe.push_back(searchfringe(add[i],fringe[minindex].weighttodate+weights[i],&temp));
		}
		fringe.erase(fringe.begin()+minindex);
	}
}
//VOID CALLBACK hourpassed(HWND hWnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
void hourpassed(int arg)
{
	hours+=hourspersecond;//goes off every 1 sec.
	if((int)hours%24==0 && abs((long)(hours-int(hours+0.00001)))<=0.0001) //one day passed
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
									map[(int)k][(int)h].uniton=false;
									map[(int)k][(int)h].index=0;
									map[(int)k][(int)h].player=0;
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
/*DWORD WINAPI ThreadTimerProc(LPVOID lpParameter) // thread data
{
	UINT uiTimer = SetTimer(NULL, 1234, 1000, hourpassed);		
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}*/
void selectone(int player, point &clicked) //select one unit, no shift key pressed, if its a siege unit, select everyone manning it as well
{
	redraw=true;//mod buttons likely
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
	else if(map[(int)clicked.y][(int)clicked.x].uniton==true)
	{
		if(clicked.y<allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->boundingbox.bottom && clicked.y>allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->boundingbox.top && clicked.x>allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->boundingbox.left && clicked.x<allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->boundingbox.right) //if the user clicked on the unit (within the 2d bouding box)
		{
			if(player==map[(int)clicked.y][(int)clicked.x].player)
			{
				if(allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->selected==true)
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
					if(allunits[player][map[(int)clicked.y][(int)clicked.x].index]->health<=0)
						return;
					allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->selected=true;
					for(unsigned int i=0; i<selectedunits[player].size(); i++)
					{
						if(whatsselected[player]==true)
							allunits[player][selectedunits[player][i]]->selected=false;
						else
							allbuildings[player][selectedunits[player][i]].selected=false;
					}
					selectedunits[player].clear();
					whatsselected[player]=true;
					selectedunits[player].push_back(map[(int)clicked.y][(int)clicked.x].index);
					if(allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->whatisit==2) //if its a siege unit
					{
						for(unsigned int i=0; i<allsiegeunits[player][allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->siegeindex].manning.size(); i++)
						{
							allunits[player][allsiegeunits[player][allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->siegeindex].manning[i]]->selected=true;
							selectedunits[player].push_back(allsiegeunits[player][allunits[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index]->siegeindex].manning[i]);
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
				//some sort of show info thing.
			}
		}
	}
	else if(map[(int)clicked.y][(int)clicked.x].tilestyle==TS_BUILDING || map[(int)clicked.y][(int)clicked.x].tilestyle==TS_WATERBUILDING)
	{
		if(clicked.y<allbuildings[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index].boundingbox.bottom && clicked.y>allbuildings[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index].boundingbox.top && clicked.x>allbuildings[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index].boundingbox.left && clicked.x<allbuildings[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index].boundingbox.right) //if the user clicked on the building (within the 2d bouding box)
		{
			if(player==map[(int)clicked.y][(int)clicked.x].player)
			{
				if(allbuildings[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index].selected==true)
				{
					allbuildings[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index].selected=false;
					selectedunits[player].clear();
				}
				else
				{
					if(allbuildings[player][map[(int)clicked.y][(int)clicked.x].index].health<0)
						return;
					allbuildings[map[(int)clicked.y][(int)clicked.x].player][map[(int)clicked.y][(int)clicked.x].index].selected=true;
					for(unsigned int i=0; i<selectedunits[player].size(); i++)
					{
						if(whatsselected[player]==true)
							allunits[player][selectedunits[player][i]]->selected=false;
						else
							allbuildings[player][selectedunits[player][i]].selected=false;
					}
					selectedunits[player].clear();
					whatsselected[player]=false;
					selectedunits[player].push_back(map[(int)clicked.y][(int)clicked.x].index);
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
				//some sort of show info thing.
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
	redraw=true; //mod buttons likely
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
void move(int player, POINT &gowhere)
{
	if(whatsselected[player]==true)
	{
		for(unsigned int i=0; i<selectedunits[player].size(); i++)//loop through all of the players selected units
		{
			if(allunits[player][selectedunits[player][i]]->whatisit!=2 && allunits[player][selectedunits[player][i]]->siegeindex!=-1)
				continue;
			if(allunits[player][selectedunits[player][i]]->health<=0) //if its dead, it shouldn't be allowed to move
				continue;
			allunits[player][selectedunits[player][i]]->movetox=(float)gowhere.x; //tell it to go to the passed point
			allunits[player][selectedunits[player][i]]->movetoy=(float)gowhere.y;
			allobstacles[player][selectedunits[player][i]].clear();
			//allunits[player][selectedunits[player][i]]->actualtomovex=-1;
			//allunits[player][selectedunits[player][i]]->actualtomovey=-1;
			allunits[player][selectedunits[player][i]]->userordered=true;
			allunits[player][selectedunits[player][i]]->dstancecoox=-1;
			allunits[player][selectedunits[player][i]]->dstancecooy=-1;
			allunits[player][selectedunits[player][i]]->gatheringx=-1;
			allunits[player][selectedunits[player][i]]->gatheringy=-1;
			allunits[player][selectedunits[player][i]]->firstobstacleattempt=true; // at this point, definitely haven't tried to avoid an obstacle
			if(map[(int)gowhere.y][(int)gowhere.x].uniton==true) //if there is a unit on the square where we are going
			{
				if(gowhere.x>allunits[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index]->boundingbox.left && gowhere.x<allunits[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index]->boundingbox.right && gowhere.y>allunits[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index]->boundingbox.top && gowhere.y<allunits[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index]->boundingbox.bottom) //if the current unit is going to be attacked
				{
					if(map[(int)gowhere.y][(int)gowhere.x].player!=players[player])
					{
						if(allunits[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index]->whatisit==2)//its a siege unit
						{
							if(allsiegeunits[map[(int)gowhere.y][(int)gowhere.x].player][allunits[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size()==0)
							{ //its unmanned
								//make this push back a new unit into whatever stores the actual units for the current player, making sure the new unit has the same exact stats as this one. xp, loc etc. then 'kill this unit'
								//in effect, once this is done, the siege unit has been 'captured' by the attacking player.
								//then say that they are manning the siege unit.
							}
							else
							{
								for(unsigned int j=0; j<allsiegeunits[map[(int)gowhere.y][(int)gowhere.x].player][allunits[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size(); j++)
								{
									//same as above, BUT says if they are out of LOS range, then capture, else can't, must attack
								}
							}
						}
						allunits[player][selectedunits[player][i]]->attackingunitplayer=map[(int)gowhere.y][(int)gowhere.x].player; //record which unit is being attacked
						allunits[player][selectedunits[player][i]]->attackingunitindex=map[(int)gowhere.y][(int)gowhere.x].index;
						allunits[player][selectedunits[player][i]]->attackingwhat=true;
						allunits[player][selectedunits[player][i]]->attackmovement(); //figure out how far to go to be in range
						continue;
					}
					else
					{
						if(player==map[(int)gowhere.y][(int)gowhere.x].player)
						{
							if(allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->whatisit==2) //going to my siege unit, so man it if neccesary
							{
								if(allunits[player][selectedunits[player][i]]->siegeindex==-1 && allunits[player][selectedunits[player][i]]->whatisit!=2 && allunits[player][selectedunits[player][i]]->whatisit!=3)//not already manning anything, and its not a siege unit, and its not a ship
								{
									if(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size()<(unsigned int)allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].maxmanning) //needs to be manned by more units
									{
										switch((allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size()%4)+1)
										{
										case 1: //first unit to man
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->x+(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->y-allunits[player][selectedunits[player][i]]->height;
											break;
										case 2:
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->x+allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->width-allunits[player][selectedunits[player][i]]->width-(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->y-allunits[player][selectedunits[player][i]]->height;
											break;
										case 3:
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->x+(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->y+allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->height;
											break;
										case 4:
											allunits[player][selectedunits[player][i]]->movetox=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->x+allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->width-allunits[player][selectedunits[player][i]]->width-(int)(allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.size()/4);
											allunits[player][selectedunits[player][i]]->movetoy=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->y+allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->height;
											break;
										};
										allsiegeunits[player][allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex].manning.push_back(selectedunits[player][i]);
										allunits[player][selectedunits[player][i]]->siegeindex=allunits[player][map[(int)gowhere.y][(int)gowhere.x].index]->siegeindex;
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
				if(gowhere.x>allbuildings[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index].boundingbox.left && gowhere.x<allbuildings[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index].boundingbox.right && gowhere.y>allbuildings[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index].boundingbox.top && gowhere.y<allbuildings[map[(int)gowhere.y][(int)gowhere.x].player][map[(int)gowhere.y][(int)gowhere.x].index].boundingbox.bottom) //if the current unit is going to be attacked
				{
					if(map[(int)gowhere.y][(int)gowhere.x].player!=players[player])
					{
						if(allunits[player][selectedunits[player][i]]->buildingattack!=0)
						{
							allunits[player][selectedunits[player][i]]->attackingunitindex=map[(int)gowhere.y][(int)gowhere.x].index;
							allunits[player][selectedunits[player][i]]->attackingunitplayer=map[(int)gowhere.y][(int)gowhere.y].player;
							allunits[player][selectedunits[player][i]]->attackingwhat=false;
							allunits[player][selectedunits[player][i]]->attackmovement();
							continue;
						}
					}
					else
					{
						if(allbuildings[player][map[(int)gowhere.y][(int)gowhere.x].index].garrison-allbuildings[player][map[(int)gowhere.y][(int)gowhere.x].index].garrisoned.size()>0 || allbuildings[player][map[(int)gowhere.y][(int)gowhere.x].index].beingbuilt>0)
							allunits[player][selectedunits[player][i]]->garrisoned=1+map[(int)gowhere.y][(int)gowhere.x].index;//garrisoning/building
					}
				}
			}
			else if(map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_TREE || map[(int)gowhere.y][(int)gowhere.x].tilestyle==TS_BERRIES)
			{
				allunits[player][selectedunits[player][i]]->gatheringx=(short)gowhere.x;
				allunits[player][selectedunits[player][i]]->gatheringy=(short)gowhere.y;
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
		if(allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].player][map[(unsigned int)clicked.y][(unsigned int)clicked.x].index].garrisoned.size()==0)
		{
			if(sqrt(pow(allunits[player][selectedunits[player][0]]->x-(unsigned int)clicked.x,2)+pow(allunits[player][selectedunits[player][0]]->y-(unsigned int)clicked.y, 2))<allunits[player][selectedunits[player][0]]->los-tilecameo[map[(unsigned int)clicked.y][(unsigned int)clicked.x].tilestyle])
			{
				short bindex=0;
				if(overwritebuildings[player].size()>0)
				{
					bindex=overwritebuildings[player][overwritebuildings[player].size()-1];
					overwritebuildings[player].pop_back();
				}
				else
				{
					allbuildings[player].push_back(allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].player][map[(unsigned int)clicked.y][(unsigned int)clicked.x].index]);
					bindex=allbuildings[player].size()-1;
				}
				allbuildings[player][bindex]=allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].player][map[(unsigned int)clicked.y][(unsigned int)clicked.x].index];//building(allbuildablebuildings[buildwhat], x+1, y+1, player, overwritebuildings[player][overwritebuildings[player].size()-1], y+1+allbuildablebuildings[buildwhat].height, y+1, x+1, x+1+allbuildablebuildings[buildwhat].width,0,0,0,0,allbuildablebuildings[buildwhat].timetobuild); //CHANGE y+1 and the like to something from GUI, so that you can specify where you want it
				allbuildings[player][bindex].index=bindex;
				allbuildings[player][bindex].player=player;
				allbuildings[player][bindex].selected=false;
				allbuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].player][map[(unsigned int)clicked.y][(unsigned int)clicked.x].index].health=0;
				overwritebuildings[map[(unsigned int)clicked.y][(unsigned int)clicked.x].player].push_back(map[(unsigned int)clicked.y][(unsigned int)clicked.x].index);
				for(float k=allbuildings[player][bindex].y-allbuildings[player][bindex].radiustodistribute; k<allbuildings[player][bindex].y+allbuildings[player][bindex].height+allbuildings[player][bindex].radiustodistribute; k+=.25)
				{
					for(float h=allbuildings[player][bindex].x-allbuildings[player][bindex].radiustodistribute; h<allbuildings[player][bindex].x+allbuildings[player][bindex].width+allbuildings[player][bindex].radiustodistribute; h+=.25)
					{
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
						map[(int)k][(int)h].player=player;
						map[(int)k][(int)h].index=bindex;
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
		else if((dispwhen&HOUSE_BUILDING)>=1 || (dispwhen&BARRACKS_BUILDING)>=1 || (dispwhen&DOCK_BUILDING)>=1) //It has a flag but the other condition was false
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
					return;
				touchland=true;
			}
			else //water
			{
				if((map[i][j].tilestyle!=TS_WATER && map[i][j].tilestyle!=TS_FISH) || map[i][j].elevation!=buildelev)
					return;
				if(checktouchingland(j,i))
					touchland=true;
			}
		}
	}
	if(touchland==false)
		return;
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
	if(checkwhat.tilestyle==TS_GATE && checkwhat.player==players[player])
		good=true;
	if(checkwhat.player==player && allunits[checkwhat.player][checkwhat.index]->whatisit==3 && checkwhat.uniton==true)
		return true;
	if(good==true && checkwhat.uniton==true && (checkwhat.index!=index || checkwhat.player!=player))
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
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WIDTH,HEIGHT);

	mainWindow=glutCreateWindow("Game Engine");

	glutDisplayFunc(renderScene);
	glutMouseFunc(processMouse);
	glutReshapeFunc(changeSize);
	glutMotionFunc(processMouseMove); //this is while mouse button pressed
	glutPassiveMotionFunc(processPassiveMouseMove); //this is while mouse button is not pressed

	initializeGameEngine();

	glutMainLoop();

	return 0;
}
void processPassiveMouseMove(int x, int y)
{
	if(x<=20 && topleft.x>0)
		topleft.x-=1;
	else if(x>=(WIDTH-20) && topleft.x<((map[0].size()*15)-WIDTH)/15)
		topleft.x+=1;
	if(y<=40 && topleft.y>0)
		topleft.y-=.5;
	else if(y>=(HEIGHT-20) && y<=HEIGHT+20 && topleft.y<((map.size()*15)-HEIGHT)/15)
		topleft.y+=.5;//end scroll
	currmousex2=x;
	currmousey2=y;
	currmousex2+=(topleft.x*15);
	currmousey2+=(topleft.y*15);
}
void processMouseMove(int x, int y)
{
	if(mousex2!=x && mousey2!=y)
	{
		uptomousex2=(float)x;
		uptomousey2=(float)y;
		uptomousex2+=(topleft.x*15);
		uptomousey2+=(topleft.y*15);
	}
}

void dblClickTimer(int arg)
{
	dblclick=-1;
}

void processMouse(int button, int state, int x, int y)
{
	if(button==GLUT_LEFT_BUTTON)
	{
		if(dblclickglut==-1)
		{
			glutTimerFunc(250,dblClickTimer,0);
			dblclickglut=0;
		}
		else if(dblclickglut==0)
			state=DBL_CLICK;
		if(state==GLUT_DOWN)
		{
			//mousex=(float)GET_X_LPARAM(lParam);
			//mousey=(float)GET_Y_LPARAM(lParam);
			mousex2=x;
			mousey2=y;
			mousex2+=(topleft.x*15);
			mousey2+=(topleft.y*15);
			if(buildinghover==true)
			{
				build(0,buildingwhat,(int)mousex2/15,(int)mousey2/15);
				buildinghover=false;
				buildingwhat=-1;
				buildingwidth=0;
				buildingheight=0;
				return;
			}
			lbuttondown=true;
			uptomousex2=mousex2;
			uptomousey2=mousey2;
		}
		else if(state==GLUT_UP)
		{
			if(dblclick==true)
			{
				dblclick=false; //makes sure this isn't called after a double click. It will be:  left down, left up, left down, double click, left up (ignored due to this) end
				return;
			}
			//RECT client;
			//GetClientRect(hWnd, &client);
			//int currx=GET_X_LPARAM(lParam);
			//int curry=GET_Y_LPARAM(lParam);
			lbuttondown=false;
			if(y>=HEIGHT-100)//buttons
			{
				for(unsigned int i=0; i<allbuttons.size(); i++)
				{
					if(checkdisp(0,allbuttons[i].dispwhen))
					{
						if(x>allbuttons[i].x && x<allbuttons[i].x+allbuttons[i].width && y>allbuttons[i].y && y<allbuttons[i].y+allbuttons[i].height)
						{
							allbuttons[i].func(buttonparam(0,i,allbuttons[i].unitorbuilding)); //player 0, index i
						}
					}
				}
				return; //not selecting anything in the button area, so finished
			}
			else if(mousex2==uptomousex2 && mousey2==uptomousey2)
			{
				point p(mousex2/15,mousey2/15);
				selectone(0, p);//point(mousex/15, mousey/15));
			}
			else
			{
				myrect clicked;
				if(uptomousex2-mousex2>0 && uptomousey2-mousey2>0)
				{
					clicked.left=mousex2;
					clicked.top=mousey2;
					//clicked.right=uptomousex-mousex;
					//clicked.bottom=uptomousey-mousey;
					clicked.right=uptomousex2;
					clicked.bottom=uptomousey2;
				}
				else if(uptomousex2-mousex2<0 && uptomousey2-mousey2>0)
				{
					clicked.left=uptomousex2;
					clicked.top=mousey2;
					//clicked.right=mousex-uptomousex;
					//clicked.bottom=uptomousey-mousey;
					clicked.right=mousex2;
					clicked.bottom=uptomousey2;
				}
				else if(uptomousex2-mousex2>0 && uptomousey2-mousey2<0)
				{
					clicked.left=mousex2;
					clicked.top=uptomousey2;
					clicked.right=uptomousex2;
					clicked.bottom=mousey2;
					//clicked.right=uptomousex-mousex;
					//clicked.bottom=mousey-uptomousey;
				}
				else if(uptomousex2-mousex2<0 && uptomousey2-mousey2<0)
				{
					clicked.left=uptomousex2;
					clicked.top=uptomousey2;
					//clicked.right=mousex-uptomousex;
					//clicked.bottom=mousey-uptomousey;
					clicked.right=mousex2;
					clicked.bottom=mousey2;
				}
				clicked.top/=15;
				clicked.bottom/=15;
				clicked.left/=15;
				clicked.right/=15;
				selectmany(currplayer,clicked);
			}
			return;
		}
		else if(state==DBL_CLICK)
		{
			dblclick=true;
			dblclickglut=-1;
			//float mx=x;
			//float my=y;
			int reg=-1;
			if(map[(int)(mousey2/15)][(int)(mousex2/15)].uniton==true)
				reg=allunits[0][map[(int)(mousey2/15)][(int)(mousex2/15)].index]->regimentid;
			if(reg!=-1)
			{
				for(unsigned int i=0; i<selectedunits[0].size(); i++)
					allunits[0][i]->selected=false;
				selectedunits[0].clear();
				for(unsigned int i=0; i<allregiments[0][reg].unitids.size(); i++)
				{
					allunits[0][allregiments[0][reg].unitids[i]]->selected=true;
					selectedunits[0].push_back(allregiments[0][reg].unitids[i]);
				}
			}
			return;
		}
	}
	else if(button==GLUT_RIGHT_BUTTON)
	{
		if(state==GLUT_DOWN)
		{
			POINT p;
			p.x=x;
			p.y=y;
			if(p.y>HEIGHT-100)
				return; //clicked in the 'button' area
			p.x+=(topleft.x*15);
			p.y+=(topleft.y*15);
			p.x/=15;
			p.y/=15;
			move(currplayer, p);
			return;
		}
		else if(state==GLUT_UP)
		{
			POINT p;
			p.x=x;
			p.y=y;
			if(p.y>=HEIGHT-100)//buttons
			{
				for(int i=indexGarrisonbutton; i<indexGarrisonbuttonend; i++)
				{
					if(checkdisp(0,allbuttons[i].dispwhen))
					{
						if(p.x>allbuttons[i].x && p.x<allbuttons[i].x+allbuttons[i].width && p.y>allbuttons[i].y && p.y<allbuttons[i].y+allbuttons[i].height)
						{
							ungarrison(buttonparam(0,i,-1));
						}
					}
				}
			}
		}
	}
	else if(button==GLUT_MIDDLE_BUTTON)
	{
		point p(x/15.0f, y/15.0f);
		capture(currplayer, p);
	}
}

void initializeGameEngine()
{
	srand((unsigned int)time(0)); //From here: absolutely necessary stuff.
	map.resize(MAPSIZE);
	minimapseen.resize(numplayers);
	overwriteunits.resize(numplayers);
	overwritebuildings.resize(numplayers);
	selectedunits.resize(numplayers);
	whatsselected.resize(numplayers);
	minimapseen[0].resize(MAPSIZE);
	minimapseen[1].resize(MAPSIZE);
	allsiegeunits.resize(numplayers);
	resources.resize(numplayers);
	resources[0].resize(4);
	resources[1].resize(4);
	players.push_back(0);
	players.push_back(1);
	newlybuiltbuildings.resize(numplayers);
	designatedunit.resize(numplayers);
	allregiments.resize(numplayers);
	allgarrisonedselectedunits.resize(numplayers);
	for(int i=0; i<MAPSIZE; i++)
	{
		map[i].resize(MAPSIZE);
		minimapseen[0][i].resize(MAPSIZE);
		minimapseen[1][i].resize(MAPSIZE);
	}
	allunits.resize(numplayers);
	actallunits.resize(numplayers);
	actallunits[0].reserve(100);
	actallunits[1].reserve(100);
	allbuildings.resize(numplayers);
	allobstacles.resize(numplayers);
	players.push_back(0);
	players.push_back(1);
	creationqueueunits.resize(numplayers);

	ifstream inf("unit.specs"); //Get allbuildableunits
	if(!inf)
	{
		printf("Unable to open unit.specs");
		exit(-235);
	}
	float args[26]={0};
	string name;
	while(!inf.eof())
	{
		for(int i=0; i<26; i++) //26=number of args to basic unit. Make sure that this is so.
		{
			inf >> args[i];
			inf.get();
		}
		getline(inf, name);
		allbuildableunits.push_back(basicunit(args[0],(short)args[1],(short)args[2],(short)args[3],(short)args[4],args[5],args[6],(short)args[7],(short)args[8],(short)args[9],args[10],(short)args[11],args[12],(short)args[13],(short)args[14],(short)args[15],(short)args[16],(unsigned char)args[17],(unsigned char)args[18],(unsigned char)args[19],(char)args[20],(short)args[21],(short)args[22],(short)args[23],(short)args[24],(short)args[25],name));
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
	}
	ifstream inf3("map2.specs");
	if(!inf3)
	{
		printf("Unable to open map.specs");
		exit(-235);
	}

	//buttons
	/*int width  = GetSystemMetrics(SM_CXFULLSCREEN);//screen dimensions
	int height = GetSystemMetrics(SM_CYFULLSCREEN);*/

	allbuttons.push_back(button(WIDTH*2/3+5+300+20,615,92,18,"Make Regiment",makereg,YOUR_MULT_UNITS));
	indexStancebutton=allbuttons.size();
	allbuttons.push_back(button(WIDTH*2/3+5,615,70,18,"Aggressive",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5+75,615,70,18,"Defensive",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5+150,615,70,18,"Don't Move",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5+225,615,70,18,"Do Nothing",setstance,YOUR_UNIT | YOUR_MULT_UNITS));
	allbuttons.push_back(button(WIDTH*2/3+5, 615+22, 70, 18, "Designate",designate,YOUR_UNIT));
	allbuttons.push_back(button(WIDTH*2/3+5, 615+22*3, 70, 18, "Record", beginrecordreport, YOUR_UNIT | YOUR_MULT_UNITS | YOUR_REGIMENT_MEMBER));
	allbuttons.push_back(button(WIDTH*2/3+5+75, 615+22*3, 70, 18, "Report", givereport, YOUR_UNIT | YOUR_MULT_UNITS | YOUR_REGIMENT_MEMBER));

	for(unsigned int i=1; i<allbuildablebuildings.size(); i++) //starting with 1 skips the pile
	{
		if(i<=12)
			allbuttons.push_back(button(5+103*((i-1)/3), 615+22*((i-1)%3), 100, 18, allbuildablebuildings[i].name, build, YOUR_UNIT | YOUR_MULT_UNITS | PAGE_1,i));
		else
			allbuttons.push_back(button(5+103*((i-13)/3), 615+22*((i-13)%3), 100, 18, allbuildablebuildings[i].name, build, YOUR_UNIT | YOUR_MULT_UNITS | PAGE_2,i));
	}
	allbuttons.push_back(button(5+309, 615+66, 100, 18, "Switch Page", nextpage, YOUR_UNIT | YOUR_MULT_UNITS));
	//allbuttons.push_back(button(5, 615, 60, 18, "House", build, YOUR_UNIT | YOUR_MULT_UNITS,0));
	//allbuttons.push_back(button(5, 615+22, 60, 18, "Barracks", build, YOUR_UNIT | YOUR_MULT_UNITS,2));
	//allbuttons.push_back(button(5, 615+44, 60, 18, "Dock", build, YOUR_UNIT | YOUR_MULT_UNITS,3));

	allbuttons.push_back(button(5, 615, 60, 18, "Villager", createnewunit, YOUR_BUILDING | HOUSE_BUILDING, 1));
	allbuttons.push_back(button(5, 615, 60, 18, "Militia", createunit, YOUR_BUILDING | BARRACKS_BUILDING, 4));
	allbuttons.push_back(button(5, 615, 60, 18, "Ship", createnewunit, YOUR_BUILDING | DOCK_BUILDING, 3));

	indexGarrisonbutton=allbuttons.size();
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<3; j++)
		{
			allbuttons.push_back(button(WIDTH/3+5+j*80, 615+22*i, 75, 18, "", selectgarrison, YOUR_BUILDING));
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

	//display
	indexResourcedispunit=alldisp.size();
	//alldisp.push_back(display("Food", width/3+5, 615, YOUR_UNIT, &allunits[0][selectedunits[0][0]]->holding[0]));
	alldisp.push_back(display("Food: ", WIDTH/3+5, 615+0*18, YOUR_UNIT, dispresourcesunit));
	alldisp.push_back(display("Wood: ", WIDTH/3+5, 615+1*18, YOUR_UNIT, dispresourcesunit));
	alldisp.push_back(display("Gold: ", WIDTH/3+5, 615+2*18, YOUR_UNIT, dispresourcesunit));
	alldisp.push_back(display("Stone: ", WIDTH/3+5, 615+3*18, YOUR_UNIT, dispresourcesunit));
	indexResourcedispbuilding=alldisp.size();
	alldisp.push_back(display("Food: ", WIDTH/3+5, 615+0*18, YOUR_BUILDING, dispresourcesbuilding));
	alldisp.push_back(display("Wood: ", WIDTH/3+5, 615+1*18, YOUR_BUILDING, dispresourcesbuilding));
	alldisp.push_back(display("Gold: ", WIDTH/3+5, 615+2*18, YOUR_BUILDING, dispresourcesbuilding));
	alldisp.push_back(display("Stone: ", WIDTH/3+5, 615+3*18, YOUR_BUILDING, dispresourcesbuilding));

	alldisp.push_back(display("Health: ", WIDTH/3+5+370, 615, YOUR_UNIT, disphealth));
	alldisp.push_back(display("", WIDTH/3+5+275, 615, YOUR_UNIT, dispname));
	alldisp.push_back(display("Moral: ", WIDTH/3+5+373, 635, YOUR_UNIT, dispmoral));
	alldisp.push_back(display("Food Required: ", WIDTH/3+5+320, 655, YOUR_UNIT, dispfoodrequired));
	alldisp.push_back(display("Health: ", WIDTH/3+5+370, 615, YOUR_BUILDING, dispbuildinghealth));
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
	//elevation change
	/*for(int i=20; i<30; i++)
	{
		for(int j=20; j<30; j++)
		{
			if(i<26 || i>28)
				map[i][j].elevation=1;
			else
				map[i][j].elevation=2;
		}
	}*/
	/*for(int i=50; i<60; i++)//create water
	{
		for(int j=15; j<35; j++)
		{
			map[j][i].tilestyle=TS_WATER;
		}
	}//end create water*/
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
			map[(int)k][(int)h].player=0;
			map[(int)k][(int)h].index=0;
			map[(int)k][(int)h].whichplayer.set(false, (unsigned char)0);
			map[(int)k][(int)h].whichplayer.set(true, players[0]);
		}
	}
	for(float k=allbuildings[0][0].y-allbuildings[0][0].radiustodistribute; k<allbuildings[0][0].y+allbuildings[0][0].height+allbuildings[0][0].radiustodistribute; k+=.25)
	{
		for(float h=allbuildings[0][0].x-allbuildings[0][0].radiustodistribute; h<allbuildings[0][0].x+allbuildings[0][0].width+allbuildings[0][0].radiustodistribute; h+=.25)
		{
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
					map[(int)k][(int)h].uniton=true;
					map[(int)k][(int)h].index=actallunits[i][j].index;
					map[(int)k][(int)h].player=actallunits[i][j].player;
				}
			}
		}
	}
	//map[10][10].tilestyle=TS_BERRIES;
	//map[10][10].resources[0]=100;
	//allunits[1][0]->movetoy=5; //makes unit move up

	//SetTimer(hWnd, 50, 1000/FPS, mainTimerProc);
	glutTimerFunc(50,mainTimerProc,0); //NOTE THAT THIS WILL DO IT ONLY ONCE
	//CreateThread(NULL, 0, ThreadTimerProc, NULL, 0, NULL);
	glutTimerFunc(1000,hourpassed,0);
}

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
		// Set the camera
	gluLookAt(	0.5f, 0.5f, 1.0f,
			    0.5f, 0.5f, 0.0f,
		     	0.0f, 1.0f, 0.0f);

	glutSwapBuffers();
}

void makeRect(float x, float y, float width, float height, RGB color)
{
	glColor3f((float)color.r/255.0, (float)color.g/255.0, (float)color.b/255.0);
	glBegin(GL_QUADS);
		glVertex3f((float)x/WIDTH,(float)y/HEIGHT,0);
		glVertex3f((float)(x+width)/WIDTH,(float)y/HEIGHT,0);
		glVertex3f((float)(x+width)/WIDTH,(float)(y+height)/HEIGHT,0);
		glVertex3f((float)x/WIDTH,(float)(y+height)/HEIGHT,0);
	glEnd();
}

void makeRect(float x, float y, float width, float height, ARGB color)
{
	glColor4f(color.r/255.0, color.g/255.0, color.b/255.0, color.a/255.0); //rgba
	glBegin(GL_QUADS);
		glVertex3f((float)x/WIDTH,(float)y/HEIGHT,0);
		glVertex3f((float)(x+width)/WIDTH,(float)y/HEIGHT,0);
		glVertex3f((float)(x+width)/WIDTH,(float)(y+height)/HEIGHT,0);
		glVertex3f((float)x/WIDTH,(float)(y+height)/HEIGHT,0);
	glEnd();
}

void drawLine(float x, float y, float fx, float fy, RGB color)
{
	glColor3f((float)color.r/255.0, (float)color.g/255.0, (float)color.b/255.0);
	glBegin(GL_LINES);
		glVertex3f((float)x/WIDTH,(float)y/HEIGHT,0);
		glVertex3f((float)fx/WIDTH,(float)fy/HEIGHT,0);
	glEnd();
}

void drawEmptyRect(float x, float y, float width, float height, RGB color)
{
	glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f((float)x/WIDTH,(float)y/HEIGHT,0);
		glVertex3f((float)(x+width)/WIDTH,(float)y/HEIGHT,0);
		glVertex3f((float)(x+width)/WIDTH,(float)(y+height)/HEIGHT,0);
		glVertex3f((float)x/WIDTH,(float)(y+height)/HEIGHT,0);
	glEnd();
}

void renderBitmapString(float x,float y,float z,void *font,char *string)
{
	char *c;
	glRasterPos3f((float)x/WIDTH,(float)y/HEIGHT,(float)z/1000.0);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void mainTimerProc(int arg)
{
	glutTimerFunc(50,mainTimerProc,0); //NOTE THAT THIS WILL DO IT ONLY ONCE, which is why it is called here. In effect, time delayed recursion.
	frames++;
	//RECT client;
	//GetClientRect(hWnd, &client);
	//client.bottom-=100; //IMPORTANT
	/*Pen p[6]={Color(0,0,0), Color(255,0,0), Color(0,255,0), Color(0,0,255), Color(255,0,255), Color(255,255,0)};
	SolidBrush sb(Color(0,255,0));
	SolidBrush green(Color(50, 0, 255, 0));
	SolidBrush black(Color(0, 0, 0)); 
	SolidBrush red(Color(255,0,0));
	SolidBrush sea(Color(55,160,225));
	SolidBrush grass(Color(35,228,50));
	SolidBrush tree(Color(106,52,43));
	SolidBrush bushes(Color(0,81,0));
	SolidBrush animal(Color(128,128,0));
	SolidBrush road(Color(0,255,255));
	SolidBrush elevation(Color(100,100,100,100));
	SolidBrush hover(Color(100,200,200,200));
	SolidBrush yellow(Color(100,255,255,0));
	SolidBrush lightgrey(Color(50,230,230,230));
	FontFamily  fontFamily(L"Times New Roman"); //To print unit lvl
	Font        font(&fontFamily, 12, FontStyleRegular, UnitPixel);
	Font        bigfont(&fontFamily, 14, FontStyleRegular, UnitPixel);*/
	//SolidBrush white(Color(255,255,255));
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
	//Setting up openGL
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//clears the whole screen

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(0.5f, 0.5f, 1.0f,
			  0.5f, 0.5f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	//end setup
	if(redraw==true) //TODO note that currently the whole screen is cleared each time. Fix this so it only clears the upper part.
	{
		redraw=false;
		
		makeRect(0,HEIGHT-70,WIDTH,70,yelloworange);

		drawLine(WIDTH/3,HEIGHT-70,WIDTH/3,HEIGHT,black);
		drawLine(WIDTH*2/3,HEIGHT-70,WIDTH*2/3,HEIGHT,black);
		for(unsigned int i=0; i<allbuttons.size(); i++) // Print buttons
		{
			if(checkdisp(0,allbuttons[i].dispwhen))
			{
				if(((int)i>=indexGarrisonbutton && (int)i<indexGarrisonbuttonend) && alldisp[indexGarrisondisp+i-indexGarrisonbutton].func(indexGarrisondisp+i-indexGarrisonbutton)=="")
					continue;
				char *toprint=new char[allbuttons[i].text.size()];
				for(unsigned int j=0; j<allbuttons[i].text.size(); j++)
				{
					if(allbuttons[i].text[j]=='_')
						toprint[j]=' ';
					else
						toprint[j]=allbuttons[i].text[j];
				}
				renderBitmapString(allbuttons[i].x,allbuttons[i].y,0,GLUT_BITMAP_TIMES_ROMAN_10,toprint);
				//temp.DrawString(toprint,allbuttons[i].text.size(),&bigfont,Gdiplus::PointF(Gdiplus::REAL(allbuttons[i].x), Gdiplus::REAL(allbuttons[i].y)),&black);
				//temp.DrawRectangle(&(p[0]),allbuttons[i].x,allbuttons[i].y,allbuttons[i].width,allbuttons[i].height);
				drawEmptyRect(allbuttons[i].x,allbuttons[i].y,allbuttons[i].width,allbuttons[i].height,black);
				delete[] toprint;
			}
		}
		for(unsigned int i=0; i<alldisp.size(); i++) //Print things in disp
		{
			if(checkdisp(0,alldisp[i].dispwhen))
			{
				string var=alldisp[i].func(i);
				char *toprint=new char[alldisp[i].text.size()+var.size()];
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
				//temp.DrawString(toprint,alldisp[i].text.size()+var.size(), &bigfont, Gdiplus::PointF(Gdiplus::REAL(alldisp[i].x),Gdiplus::REAL(alldisp[i].y)),&black);
				renderBitmapString(alldisp[i].x,alldisp[i].y,0,GLUT_BITMAP_TIMES_ROMAN_10,toprint);
				delete[] toprint;
			}
		}
		/*RECT topaint;
		topaint.left=0;
		topaint.top=client.bottom;
		topaint.right=client.right;
		topaint.top=client.bottom+100;*/
		//HBRUSH b=CreateSolidBrush(RGB(250,225,65));
		//FillRect(hdcBuf,&topaint,b);
		//DeleteObject(b);
		//ShowWindow(buttons[0], SW_SHOWNORMAL);
		//buttons[0]=CreateWindowEx(NULL, "button","makereg", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 100, 630, 60, 20, hWnd, HMENU(IDB_MAKEREG), NULL, NULL);
		//buttons[0]=CreateWindow("button","makereg", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 100, 630, 60, 20, hWnd, HMENU(IDB_MAKEREG), NULL, NULL);
		//buttons[1]=CreateWindow("button","destroyreg", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 100, 670, 80, 20, hWnd, HMENU(IDB_MAKEREG), NULL, NULL);
	}
	//Graphics g(hdcBuf);
	//Graphics g2(hdcBuf2);
	//FillRect(hdcBuf, &client, (HBRUSH)GetStockObject(WHITE_BRUSH)); //Add optimization code: if most of screen must be black, fill with black and color rest white. Else do this.
	//g.FillRectangle(&white,0,0,client.right,client.bottom);
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
				newlybuiltbuildings[i].erase(newlybuiltbuildings[i].begin()+j);
			}
		}
	}//end new buildings
	//elevation change
	/*for(int i=20; i<30; i++)
	{
		for(int j=20; j<30; j++)
		{
			g.FillRectangle(&SolidBrush(Color(255-(map[i][j].elevation*30),255-(map[i][j].elevation*30),255-(map[i][j].elevation*30))), j*15, i*15, 15, 15);
		}
	}*/
	/*POINT mouse;//scroll
	GetCursorPos(&mouse);
	if(mouse.x<=20 && topleft.x>0)
		topleft.x-=1;
	else if(mouse.x>=(client.right-20) && topleft.x<((map[0].size()*15)-client.right)/15)
		topleft.x+=1;
	if(mouse.y<=40 && topleft.y>0)
		topleft.y-=.5;
	else if(mouse.y>=(client.bottom-20) && mouse.y<=client.bottom+20 && topleft.y<((map.size()*15)-client.bottom)/15)
		topleft.y+=.5;//end scroll*/
	//Below draws things

	//mouse.y-=20; //IMPORTANT
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
						double dist=sqrt(pow(allunits[i][allregiments[i][j].lieutenant]->x-allunits[i][allregiments[i][j].unitids[k]]->x,2)+pow(allunits[i][allregiments[i][j].lieutenant]->y-allunits[i][allregiments[i][j].unitids[k]]->y,2));
						if(dist>DIST_TO_REG)
						{
							if(allunits[i][allregiments[i][j].unitids[k]]->recording==false)
								continue;
							else
							{
								allunits[i][allregiments[i][j].unitids[k]]->rep=allregiments[i][j].rep;
								allunits[i][allregiments[i][j].unitids[k]]->recording=false;
								continue;
							}
						}
						else
						{
							allunits[i][allregiments[i][j].unitids[k]]->recording=true;
							vector<pointex> allseenunits;
							allunits[i][allregiments[i][j].unitids[k]]->checkrad((int)allunits[i][allregiments[i][j].unitids[k]]->los,allunits[i][allregiments[i][j].unitids[k]]->x,allunits[i][allregiments[i][j].unitids[k]]->y,allseenunits);
							allregiments[i][j].rep.updateseenunits(allseenunits);
						}
					}
				}
			}
		}
	}
	for(unsigned int i=(int)topleft.y; i<topleft.y+HEIGHT/15; i++) //loop through map, print tiles
	{
		for(unsigned int j=(int)topleft.x; j<topleft.x+HEIGHT/15; j++)
		{
			if(j>=MAPSIZE)
				break;
			if(i>=MAPSIZE)
				break;
			if(map[i][j].tilestyle==TS_BERRIES) //print berries
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
		for(int j=(int)topleft.x; j<topleft.x+HEIGHT/15; j++)
		{
			if(j>=MAPSIZE || i>=MAPSIZE || i<0 || j<0)
				break;
			float dist=sqrt(pow(allunits[0][generals[0]]->x-j,2)+pow(allunits[0][generals[0]]->y-i,2));
			if((map[i][j].tilestyle==TS_WATERBUILDING || map[i][j].tilestyle==TS_BUILDING) && minimapseen[0][i][j]==true)//print buildings, only if visible
			{	
				int index=map[i][j].index;
				int player=map[i][j].player;
				if((int)allbuildings[player][index].x==j && (int)allbuildings[player][index].y==i)//print only once
				{
					if(allbuildings[player][index].health>0)//don't print dead things
					{
						char *text=new char[allbuildablebuildings[allbuildings[player][index].id].name.size()];
						for(unsigned int k=0; k<allbuildablebuildings[allbuildings[player][index].id].name.size(); k++)
						{
							if(allbuildablebuildings[allbuildings[player][index].id].name[k]=='_')
								text[k]=' ';
							else
								text[k]=allbuildablebuildings[allbuildings[player][index].id].name[k];
						}
						renderBitmapString((j-topleft.x)*15+2, (i-topleft.y)*15+2,0,GLUT_BITMAP_TIMES_ROMAN_10,text);
						//g.DrawString(text,allbuildablebuildings[allbuildings[player][index].id].name.size(),&bigfont,Gdiplus::PointF(Gdiplus::REAL((j-topleft.x)*15+2), Gdiplus::REAL((i-topleft.y)*15+2)),&black);
						if(allbuildings[player][index].beingbuilt<0)
						{
							//TODO Figure out the below ellipse
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
					}
				}
			}//end print buildings
			else if(map[i][j].uniton==true && allunits[0][generals[0]]->los>dist)//print units
			{
				int index=map[i][j].index;
				int player=map[i][j].player;
				if((int)allunits[player][index]->x==j && (int)allunits[player][index]->y==i)
				{
					if(allunits[player][index]->health>0)
					{
						char text=allunits[player][index]->veterancylvl;
						glRasterPos3f((allunits[player][index]->x-topleft.x)*15/WIDTH,(allunits[player][index]->y-topleft.y)*15/HEIGHT,0);
						glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10,text);
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

	glutSwapBuffers();
}

void renderReportDialog()
{ //TODO Make this change the text when you click a 'button'
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
		// Set the camera
	gluLookAt(	0.5f, 0.5f, 1.0f,
				0.5f, 0.5f,  0.0f,
				0.0f, 1.0f,  0.0f);

	RGB black(0,0,0);
	RGB blue(0,0,255);
	RGB currColor=black;
	if(currReportTab==0)
		currColor=blue;
	else
		currColor=black;
	drawEmptyRect(3,3,15,5,currColor); //buttons
	renderBitmapString(5,5,0,GLUT_BITMAP_TIMES_ROMAN_10,"Units Lost");
	if(currReportTab==1)
		currColor=blue;
	else
		currColor=black;
	drawEmptyRect(20,3,15,5,currColor);
	renderBitmapString(22,5,0,GLUT_BITMAP_TIMES_ROMAN_10,"Enemies Killed");
	if(currReportTab==2)
		currColor=blue;
	else
		currColor=black;
	drawEmptyRect(28,3,15,5,currColor);
	renderBitmapString(30,5,0,GLUT_BITMAP_TIMES_ROMAN_10,"Surviving Enemies Seen");

	renderBitmapString(25,25,0,GLUT_BITMAP_TIMES_ROMAN_10,(currRep->*reportfuncs[currReportTab])());
	glutSwapBuffers();
}

/*BOOL CALLBACK reportdialogproc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
    {
		case WM_INITDIALOG:
		{
			INITCOMMONCONTROLSEX ix;
			ix.dwSize = sizeof(INITCOMMONCONTROLSEX);
		    ix.dwICC = ICC_TAB_CLASSES;
		    InitCommonControlsEx(&ix); // have to run this to use tab control
		    hTabControl = GetDlgItem(hWnd,IDC_TAB);
			TCITEM ti;
			ti.mask = TCIF_TEXT; // I'm only having text on the tab
			ti.pszText = "Units lost";
			TabCtrl_InsertItem(hTabControl,0,&ti);
			ti.pszText = "Enemies killed";
			TabCtrl_InsertItem(hTabControl,1,&ti);
			ti.pszText = "Surviving enemies seen";
			TabCtrl_InsertItem(hTabControl,2,&ti);
			TabCtrl_SetCurSel(hTabControl,0);

			hCurrentTab = CreateDialog(hInst,MAKEINTRESOURCE(IDC_TAB1),hTabControl,0); // Setting dialog to tab one by default
			return TRUE;
		}
		case WM_NOTIFY:
		{
			switch(((LPNMHDR)lParam)->code)
			{
				case TCN_SELCHANGE: // message sent because someone changed the tab selection (clicked on another tab)
				{
					EndDialog(hCurrentTab,0); // we don't want the current tab, kill it
					hCurrentTab=CreateDialog(hInst,MAKEINTRESOURCE(IDC_TAB1+TabCtrl_GetCurSel(hTabControl)),hTabControl,0);
					updatetext(hWnd,MSG_ONE+TabCtrl_GetCurSel(hTabControl),(currRep->*reportfuncs[TabCtrl_GetCurSel(hTabControl)])());
					return TRUE;
				}
			}
			return TRUE;
		}
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
					EndDialog(hCurrentTab,0);
					DestroyWindow(reportdialoghwnd);
            }
			break;
		case WM_DESTROY:
			EndDialog(hCurrentTab,0);
			DestroyWindow(reportdialoghwnd);
			break;
		case WM_CLOSE:
			EndDialog(hCurrentTab,0);
			DestroyWindow(reportdialoghwnd);
        default:
            return FALSE;
    }
    return TRUE;
}*/
