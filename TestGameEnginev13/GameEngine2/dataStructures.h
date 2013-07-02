#ifndef data_structures_h
#define data_structures_h

#include <vector>
#include <string>
using namespace std;

//! \file "data structures.h"
//! \brief This file contains various classes used solely for data storage

//! \file "data structures.h" Mostly, member functions consist of only constructors.

class creationinfo
{
public:
	short pindex; //index in the buildings garrison vector
	short timeremaining; //time left to build it
	short createwhat; //id in buildableunits of what is being created
	creationinfo(short pi, short tr, short cw);
};
class obstaclethreadparam
{
public:
	float xchange; //the change in x tiles that a unit is trying (and failing) to make
	float ychange; //ditto for y
	float unitx; //where is the unit
	float unity; //ditto
	float upx; //is this thread in charge of calculating whether the path ABOVE(left) is shorter, or the path BELOW(right). -1 is up/left, 1 is down/right 
	float upy;
	int player; //whose unit is this; the first index for allobstacles
	int index; //the second index in allobstacles   These to will dictate what the thread will return to
	int index2; //whether this is going up or down, in terms of the index to allobstacles: do I add 0 or 1 to index
	float height;
	float width;
	float goodxchange;//the amount we have moved productively, in the correct direction, without being on an obstacle
	float goodychange;
	obstaclethreadparam(float xc, float yc, float ux, float uy, float p_upx, float p_upy, int p, int i, int i2, float h, float w, float gxc=0.0f, float gyc=0.0f);
};
class point
{
public:
	float x;
	float y;
	point(float px=0, float py=0);
};
bool operator==(point &p1, point &p2);
class pointex //this is like the above point, but also stores whether the point in question on the map has a unit or a building. For attack()
{
public:
	double x;
	double y;
	bool unit;
	double dist;
	short player;
	short index;
	pointex(double px=0, double py=0, double d=-1, bool u=true, short p=-1, short i=-1);
};
class pointex2 //this is like the above point, but also stores whether the point in question on the map has a unit or a building. For attack()
{
public:
	short index;
	double dist;
	short food;
	pointex2(short i=-1, double d=-1, short f=0);
};
class myrect
{
public:
	float top;
	float bottom;
	float right;
	float left;
	myrect(float t=0, float b=0, float r=0, float le=0);
};
class obstaclethreadret
{
public://stores values produced by the go around obstacle threads globally
	vector <point> movesmade; //the actual moves made by the thread
	//int dist;//the distance traveled by the thread
	float goodxchange; //the productive changes in x (neg is unproductive)
	float goodychange; //productive changes in y
	obstaclethreadret(float gxc=0, float gyc=0);
};
class mychar
{
public:
	unsigned char data;
	mychar();
	unsigned char get(bool whichone);
	void set(bool whichone, unsigned char towhat);
};
class buttonparam
{
public:
	short player;
	short index; //this buttons index in allbuttons. Used for when there are multiple consecutive similar buttons
	short unitorbuilding; //index in buildableunits/buildable buildings of relevant building/unit: for constructing a building or training a unit
	buttonparam(short p=0, short i=0, short unitorbuilding=-1);
};
class display
{
public:
	string text;
	int x;
	int y;
	unsigned long long dispwhen; //also uses YOUR_UNIT, etc
	//short *dispvar; //The variable to display. If -1, it will not print.
	string (*func)(int);
	display(string txt, int p_x, int p_y, unsigned long long dw, string (*f)(int));
};
//! This class stores data for the report class. It contains data only about units, not events.
class reportunitinfo
{
public:
	short id; //!< The id of the unit in allbuildableunits
	short vetlvl; //!< The veterancy level of the unit
	bool lieut; //!< Whether or not the unit was a lieutenant
	short lieutlvl; //!< If so, the lieutenant level of the unit. Else -1

	//! The only constructor available.

	/*!
	\param p_id What the id member will be initiallized to.
	\param vlvl What the vetlvl member will be initiallized to.
	\param l What the lieut member will be initiallized to, default false
	\param llvl What the lieutlvl  member will be initiallized to, default -1
	*/
	reportunitinfo(short p_id, short vlvl, bool l=false, short llvl=-1); 
};
//! This class stores data for the report class. It contains simple data about enemy units
class reportsimpleunitinfo
{
public:
	short enemyseenid; //!< This contains ids in allbuildable units of seen enemy units
	point enemyseenloc; //!< This contains the points at which enemy units were seen
	short enemyplayer; //!< This stores the player of each unit in enemy seen units.
	short enemyindex; //!< This stores the index of each unit in enemy seen units.
	
	//!<Constructor
	reportsimpleunitinfo(short esid, point esl, short ep, short ei);
};
class RGB
{
public:
	int r;
	int g;
	int b;
	RGB(int pr, int pg, int pb);
};
class ARGB
{
public:
	int a;
	int r;
	int g;
	int b;
	ARGB(int pa, int pr, int pg, int pb);
};

#define FEWRESOURCES     0
#define INVALIDSTRING	 1
#define ILLEGALLOC       2
#define FEWMENREG        3
#define MANYMENREG	     4
#define SELFTRANSFER	 5
#define TOOBIGTRANSFER   6
#define NORESOURCELOC    7
#define EARLYBUILD		 8
#define EARLYTRAIN		 9
#define NOUNITSSELECTED  10

class ErrorMSG
{
public:
	string msg;
	int x;
	int y;
	int time; //In frames. When creating do sec*FPS
	RGB color;
	ErrorMSG(string m, int px, int py, int t);
	ErrorMSG(string m, int px, int py, int t, RGB c);
	ErrorMSG();
};
class mouseOver
{
public:
	string text;
	int x; //where it will show up.
	int y; 
	unsigned long long dispwith; //!< What button it is assosiated with
	string (*func)(int); //!< To print variable text
	mouseOver(string txt, int p_x, int p_y, unsigned long long dw, string (*f)(int));
};
struct POINT
{
	int x;
	int y;
};
class progressBar
{
public:
	int x;
	int y;
	int width;
	string text;
	int dispwhen;
	double (*func)(int);
	progressBar(int px, int py, int w, string txt, int disp, double (*f)(int));
};
class advReq 
{
public:
	short whatsreq; //!< The stuff one must have for advancement
	short amntreq;  //!< How much of it one must have
	advReq(short wr, short ar); //!< simple constructor
};
class powerBonus
{
public:
	vector<short> againstWhat;
	double attackBonus;
	double attackPercentBonus;
	double defenseBonus;
	double defensePercentBonus;
	powerBonus(vector<short> aw, double ab, double ap, double db, double dp);
};
class unitPriceMod
{
public:
	short forWhat;
	short prices[4];
	short time;
	unitPriceMod(short fw, short f, short w, short g, short s, short t);
};
#endif