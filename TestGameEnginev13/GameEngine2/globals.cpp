#include "unit.h"
#include "dataStructures.h"
#include "building.h"
#include "button.h"
#include "regiment.h"
#include "siege.h"

short currplayer=0; //DELETE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TESTING ONLY!!!!!!!!!!!!!!!!!!!!!!!!! DELETE DELETE DELETE!!!!!!!!!!!!!!!!!!!!!!!!!!!

//declmarations of global variables
int countobstacle=0;//var to make sure that there are no infinite loops in threadgoaroundobstacle
vector <vector <unit*> > allunits;   //all the units, each new vector of units is for each player
vector <vector <building> > allbuildings;
vector <vector <unit> > actallunits; //the spot where the actual data for the units in allunits is stored. 
vector <vector<double> > resources; //player's resources, first layer being food/wood/gold/stone, next player being first player 1, then the other players //these should be rounded to nearest int when displaying to user
vector < vector<tile> > map;//the map, made of tiles
long double hours;
//vector < vector<obstaclethreadret> > allobstacles; //A obstaclethreadret class is made for each unit
vector < vector< vector<point> > > allobstacles; //a vector for each unit
vector <unsigned char> players; //this assigns a team number for each player. Its size will be == to allunits.size() which will be == to allbuildings.size()
vector <vector <short> > selectedunits; //layer one: player, layer 2: the units or buildings selected at any one time. One cannot select a building and a unit at the same time. point: x=player, y=index
vector <bool> whatsselected; //is the above vector storing units or buildings, for each player, unit==true, building==false
unsigned long int frames=0; // the number of frames that have passed. Its ok if this overflows
vector <vector <short> > overwriteunits; // all the indexes in allunits that can be overwritten since the unit died. first layer is player
vector <vector <short> > overwritebuildings; //same as above for allbuildings
vector <basicunit> allbuildableunits; //all of the units in the game, accessed with id, FIRST ONE WILL BE VILLAGER
vector <basicbuilding> allbuildablebuildings;
vector <vector <siege> > allsiegeunits;
vector <vector <regiment> > allregiments; //allregiments[player][regimentid]
vector <vector <vector <short> > > techtree; //layer one: 0==infantry, 1==archer, 2==cavalry, 3==siege, 4==ship //NOTE: order is techtree[layer1][layer2][layer3]
											 //layer two: each individual branch such as the branch that starts with militia, or simple bowmen, or scouts.
											 //layer three: the id's of the soldiers in the branches, ids being the ones for allbuildable units
											 //NOTE: any unit that is in techtree[0]/[1]/[2] will be able to have a dual specialty, being able to start from two different layer two branches, which do not necasserily have to be in the same layer one category
											 //NOTE: VILLAGERS (ID 0)  NOT IN HERE
vector <vector <vector <creationinfo> > > creationqueueunits;  //var[player][index][actual time, in frames, which it takes to make unit. If 0 then done, create the unit] third layer is necessary b/c can train mult units in one building simultaneously
vector <vector <vector <bool> > > minimapseen; //in form [player][y][x], if the specified player can see the tile, it is true, else false
vector <vector <short> > newlybuiltbuildings; //var[player][index of built thing]
bool lbuttondown=false;
float mousex=0; //Absolute position of mouse at any time, not in tiles, for mouse over text
float mousey=0; 
float mousex2=0; //where the mouse was when clicked
float mousey2=0;
float uptomousex2=0; //where the mouse was when last held down
float uptomousey2=0;
float currmousex2=0; //where the mouse is currently, for building hover
float currmousey2=0;
vector <short> designatedunit; //each player can designate one unit by ctrl-clicking. vector is of players. This is to select the lieutenant in a regiment
short numplayers=2; //this will later be made changeable, NOT FULLY IMPLEMENTED, CANT JUST CHANGE HERE
short tilecameo[14]={0,0,0,1,0,0,0,0,0,3,3,-2,0,0};
float tilespeed[14]={0,0,0,-.15f,0,0,0,0,0,-.1f,-.1f,.1f,0,0};
short *generals=new short[numplayers]; //the index in allunits of the general, for each player. generals[player]
vector <button> allbuttons;
point topleft(35,35);
bool dblclick=false;
vector <display> alldisp;
vector <vector<short> > allgarrisonedselectedunits;
bool buildinghover=false;
int buildingheight=0;
int buildingwidth=0;
int buildingwhat=-1;
int redraw=5;
/*HWND reportdialoghwnd;
HWND hTabControl; // tab control handle
HWND hCurrentTab; // tab dialog handle*/
char* (report::*reportfuncs[3])();
report *currRep=NULL;
int pagenum=0; //!< The page that the user is on, in the general menu of buttons
int currReportTab=0; //!< What tab in the report dialog the user is on
int transferResourcesPressed=0; //!< Whether the user has clicked the transfer resources button: If so, display the input screen.
string transferResourcesTyped="";
string transferInput="";
int transferBuilding1=-1;
ErrorMSG currErr;
vector<ErrorMSG> allErr;
vector<mouseOver> allMouseOver;

//Glut has ints instead of HWNDs. These are to store windows
int mainWindow=0;
int reportDialogWindow=0;

//These store indexes for various buttons and displays
int indexStancebutton;
int indexResourcedispunit;
int indexResourcedispbuilding;
int indexGarrisondisp;
int indexGarrisonbutton;
int indexGarrisonbuttonend;
int indexSailorsdisp;
int indexSailorsbutton;
int indexSailorsbuttonend;
int indexBuildingsbutton;
int indexTransferbutton;