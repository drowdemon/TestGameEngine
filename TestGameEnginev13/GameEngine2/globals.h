#ifndef globals_h
#define globals_h

#include "unit.h"
#include "building.h"
#include "siege.h"
#include "regiment.h"
#include "button.h"
#include "astarsearch.h"
#include "dataStructures.h"

//NOTE TO SELF: THIS FILE MAY ONLY HAVE extern VARIABLES, AND EXTERN VARIABLE MAY NOT BE DEFINED HERE i.e NO EQUALS SIGNS. THOSE GO IN globals.cpp!

extern short currplayer; //DELETE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TESTING ONLY!!!!!!!!!!!!!!!!!!!!!!!!! DELETE DELETE DELETE!!!!!!!!!!!!!!!!!!!!!!!!!!!

//#defines/constants (just so as not to use actual constant values, since they are liable to change)
#define MAX_LOADSTRING 100
//! for the timer, game hours passed in one real second
#define hourspersecond 0.2
//! pi
#define PI 3.1415926535897932384626433832795
//! e
#define e 2.71828182846
//! Frames Per Second
#define FPS 25
//! the distance a unit has to be from a unit to use the units meleeattack, if greater will be ranged
#define MELEE 3 
//! the maximum distance a unit can be from a siege unit to man it
#define DIST_TO_MAN 4
//! the minimum units per regiment
#define MINREG 3//20  FIX THIS IT SHOULD BE 20!!!!!!!!!!!!!!
//! the maximum units per regiment
#define MAXREG 50
//! You can gather once every second
#define GATHERING_RATE FPS
//! Dist that you can see an ally from, to steal food from
#define SEE_ALLIES 5
//! The size of the map on one side
#define MAPSIZE 200 //will be 1000
//! Distance that a unit can be from the lieutenant of a regiment before it no longer recieves/updates report
#define DIST_TO_REG 20 

//forward declarations of functions
void hourpassed(int arg);
int threadgoaroundobstacle(obstaclethreadparam *param, short walkable[6], bool sotr); //used to be thread proc that returned DWORD WINAPI and took an LPVOID lpParameter as an argument
bool pointonlinesegment(double x, double movetox, double newx);
void attackmovement(int player, int index, bool unit);
//pointex getcandidate(int x, int y, int player, int index, double dist);
void quicksort(vector <pointex2> &num, int top, int bottom);
int partition(vector <pointex2> &num, int top, int bottom);
bool checkdimensions(float x, float y);
bool checkdimensions(point p);
vector<point> astarsearch(problem p, float speed, unit *u);
bool checkdisp(int player, unsigned long long dispwhen);
void build(short player, short buildwhat, int x, int y);
bool chkmvp1(tile &checkwhat, short player, short index, short utype);
bool checktouchingland(int x, int y);
string inttos(int num);
bool transferResources(int player, string input, int bindex1, int bindex2);

//declarations of global variables
extern int countobstacle;//var to make sure that there are no infinite loops in threadgoaroundobstacle
extern vector <vector <unit*> > allunits;   //all the units, each new vector of units is for each player
extern vector <vector <building> > allbuildings;
extern vector <vector <unit> > actallunits; //the spot where the actual data for the units in allunits is stored. 
extern vector <vector<double> > resources; //player's resources, first layer being food/wood/gold/stone, next player being first player 1, then the other players //these should be rounded to nearest int when displaying to user
extern vector < vector<tile> > map;//the map, made of tiles
extern long double hours;
//extern vector < vector<obstaclethreadret> > allobstacles; //A obstaclethreadret class is made for each unit
extern vector < vector< vector<point> > > allobstacles; //a vector for each unit
extern vector <unsigned char> players; //this assigns a team number for each player. Its size will be == to allunits.size() which will be == to allbuildings.size()
extern vector <vector <short> > selectedunits; //layer one: player, layer 2: the units or buildings selected at any one time. One cannot select a building and a unit at the same time. point: x=player, y=index
extern vector <bool> whatsselected; //is the above vector storing units or buildings, for each player, unit==true, building==false
extern unsigned long int frames; // the number of frames that have passed. Its ok if this overflows
extern vector <vector <short> > overwriteunits; // all the indexes in allunits that can be overwritten since the unit died. first layer is player
extern vector <vector <short> > overwritebuildings; //same as above for allbuildings
extern vector <basicunit> allbuildableunits; //all of the units in the game, accessed with id, FIRST ONE WILL BE VILLAGER
extern vector <basicbuilding> allbuildablebuildings;
extern vector <vector <siege> > allsiegeunits;
extern vector <vector <regiment> > allregiments; //allregiments[player][regimentid]
extern vector <vector <vector <short> > > techtree; //layer one: 0==infantry, 1==archer, 2==cavalry, 3==siege, 4==ship //NOTE: order is techtree[layer1][layer2][layer3]
											 //layer two: each individual branch such as the branch that starts with militia, or simple bowmen, or scouts.
											 //layer three: the id's of the soldiers in the branches, ids being the ones for allbuildable units
											 //NOTE: any unit that is in techtree[0]/[1]/[2] will be able to have a dual specialty, being able to start from two different layer two branches, which do not necasserily have to be in the same layer one category
											 //NOTE: VILLAGERS (ID 0)  NOT IN HERE
extern vector <vector <vector <creationinfo> > > creationqueueunits;  //var[player][index][actual time, in frames, which it takes to make unit. If 0 then done, create the unit] third layer is necessary b/c can train mult units in one building simultaneously
extern vector <vector <vector <bool> > > minimapseen; //in form [player][y][x], if the specified player can see the tile, it is true, else false
extern vector <vector <short> > newlybuiltbuildings; //var[player][index of built thing]
extern bool lbuttondown;
extern float mousex;//absolute position of mouse at any time
extern float mousey;
extern float mousex2; //where the mouse was when clicked
extern float mousey2;
extern float uptomousex2; //where the mouse was when last held down
extern float uptomousey2;
extern float currmousex2; //where the mouse is currently
extern float currmousey2;
extern vector <short> designatedunit; //!< To select the lieutenant in a regiment.

//!< select the each player can designate one unit by ctrl-clicking. vector is of players. Stores the index in allunits
//extern HWND reportdialoghwnd;
//extern HWND hTabControl; // tab control handle
//extern HWND hCurrentTab; // tab dialog handle

extern short numplayers; //this will later be made changeable, NOT FULLY IMPLEMENTED, CANT JUST CHANGE HERE
extern short tilecameo[14];
extern float tilespeed[14];
extern short *generals; //the index in allunits of the general, for each player. generals[player]
extern vector <button> allbuttons;
extern point topleft;
extern bool dblclick;
extern vector <display> alldisp;
extern vector <vector<short> > allgarrisonedselectedunits;
extern bool buildinghover;
extern int buildingwidth;
extern int buildingheight;
extern int buildingwhat;
extern int redraw;
extern char* (report::*reportfuncs[3])();
extern report* currRep;
extern int pagenum;
extern int mainWindow;
extern int reportDialogWindow;
extern int currReportTab;
extern int transferResourcesPressed;
extern string transferResourcesTyped;
extern string transferInput;
extern int transferBuilding1;
extern ErrorMSG currErr;
extern vector<ErrorMSG> allErr;
extern vector<mouseOver> allMouseOver;

extern int indexStancebutton;
extern int indexResourcedispunit;
extern int indexResourcedispbuilding;
extern int indexGarrisondisp;
extern int indexGarrisonbutton;
extern int indexGarrisonbuttonend;
extern int indexSailorsdisp;
extern int indexSailorsbutton;
extern int indexSailorsbuttonend;
extern int indexBuildingsbutton;
extern int indexTransferbutton;

#endif
