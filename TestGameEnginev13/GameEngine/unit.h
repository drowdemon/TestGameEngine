#ifndef unit_h
#define unit_h
#include "base unit buildings.h"
#include "data structures.h"
#include "tile.h"
#include "report.h"

//US==Unit Stance //similar to age of emps
#define US_AGGRESIVE 1 //chases while in LOS
#define US_DEFENSIVE 2 // chases until enemy leaves the units original LOS view (or a user specified number of tiles), then goes back to starting pos
#define US_DONTMOVE 3 //doesn't move, but can still attack
#define US_DONOTHING 4 //doesn't move, doesn't attack

//! \file unit.h This contains class unit

//! This class contains volatile data about a unit.

//! Static data, like maxhealth, is stored in baseunit, and used in basic unit, which is for allbuildableunits
//! there is a global vector of vector of pointers to units called allunits. 
class unit : public baseunit //This will be any infantry, archer, ship, cavalry, siege, etc.
{
public:
	float x;//coordinates on map (used to be int... will a decimal of a tile location work???) It basically works. Involves a lot of typecasting in the map, and makes some .99999999 issues
	float y;//ditto 
	short player; //its first index in allunits
	short index; //its next index in allunits
	myrect boundingbox; //the 2d bounding box of the image that is the unit

	float moral;//if 0, unit will desert/rebel goes from 0 to 100
	float timeslept;//reset daily, stores amount of time asleep
	//should I force the player to gather water... and not salt water???
	short veterancylvl; //lvl of the unit, based on battles experienced
	float exp;  //experience, garnered during battles, when a unit gets enough they gain a veterency lvl
	float movetox; //which tile the unit is moving to (if standing this == x)
	float movetoy; //used to be int as well
	short siegeindex; //its index in allsiegeunits if its a siege unit, or its the index of the siege unit its manning if it is not a siege unit, in which case its -1 if its not manning anything
	bool selected; //whether or not the player that owns it selected it
	char unitstance;//the unit stance, see US #define
	bool stayoutoftowerrange; //whether or not this unit must stay out of range of all towers and castles
	short attackingunitplayer; //for allunits OR ALLBUILDINGS, the first index of the unit this unit is attacking 
	short attackingunitindex; //same as above, second unit
	float dstancecoox;//the following 3 vars can be used as vars for things other than the described purpose if the stance is not defensive
	float dstancecooy;//the xy coordinates when the unit went off to attack something in its radius 
	short dstancerad; //the radius (default == LOS), can be modified by player
	bool userordered; //whether the current attack was ordered by the player. If it was, the unit is basically in aggresive stance
	bool attackingwhat; //do attackingunitplayer and attackingunitindex refer to a unit(true) or a building (false)
	short prevsiegeindex; //in a regular unit, the index of the siege unit it was manning before it went off to fight something that was in melee range, and thus dangerous
	short garrisoned; //0 is no, >=1 is garrisoning, in which case it is also the 1+index of the building in allbuildings that its garrisoning, <=-1 is yes, in which case its the index-1 in allbuildings
	//note: allunits can function as villagers (except siege and ships), so the following 2 vars are for villager-like functionality. There will be units that are specifically villagers, however there is no advantage to having those over any other unit, thats just the root of all of them, and can become any other unit (obviously excluding ships and siege units again)
	short specialty; //each villager gains a specialty over time, gives slight improvement to gathering rate, which is normally const. 0 is none, farming 1, lumber 2, gold 3, stone 4, fish 5, build 6, research 7, hunt 8
	float specexperience[8];  //there experience in each of the above specialties
	float gatheringx; //The coordinates of the square from which the player is gathering
	float gatheringy; //ditto
	short holding[4]; // how many resources a unit is holding, 0=food, 1=wood, 2=gold, 3=stone
	short nearesthold; //the index in allbuildings that is nearest to the unit
	int regimentid; //which regiment is it in, for 2d array allregiments[player][regimentid]
	vector <short> unitsinside; //For SHIPS contains the index in allunits of the units garrisoned inside, will be empty if not a ship
	bool training;
	//short takeresourcesfrom; //building index that unit will take resources from by default
	char goingobstaclecounter;
	bool firstobstacleattempt; //Is this the first attempt to avoid an obstacle (true), or have we failed already(false)
	short lieutenant;
	float lieutexp;
	bool lieuttrain;
	bool recording; //!< This is true if the regiment the unit is part of is recording. 
	report rep; //!< This stores the report of the regiment the unit is in
	
	//!<  It is empty if the unit is near the regiment. Else, it stores the report as it was when it left the vicinity of the regiment. At that point, no more updating takes place.
	unit(float h=0, short ma=0, short ra=0, short a=0, short ba=0, float LOS=0, float s=0, short as=0, short fc=0, /*short ft=0, short wt=0, short gt=0, short st=0, short tt=0, */short sn=0, float px=0, float py=0, float ar=0, short pid=0, float c=0, short p=-1, short i=-1, short high=0, short w=0, short bs=0, short mxhld=0, unsigned char wisit=1, unsigned char ming=-1, unsigned char maxg=-1, float m=100, float ts=0, short vetlvl=0, float xp=0, float mx=-1, float my=-1, short si=-1, char us=US_AGGRESIVE, bool sotr=false, float dx=0, float dy=0, short drad=10, short spec=0,/* short tr=-1,*/ short regid=-1, bool lieut=false);
	unit(unsigned char hlding[4], float se[8], myrect bb, float h=0, short ma=0, short ra=0, short a=0, short ba=0, float LOS=0, float s=0, short as=0, short fc=0, /*short ft=0, short wt=0, short gt=0, short st=0, short tt=0, */short sn=0, float px=0, float py=0, float ar=0, short pid=0, float c=0, short p=-1, short i=-1, short high=0, short w=0, short bs=0, short mxhld=0, unsigned char wisit=1, unsigned char ming=-1, unsigned char maxg=-1, float m=100, float ts=0, short vetlvl=0, float xp=0, float mx=-1, float my=-1, short si=-1, char us=US_AGGRESIVE, bool sotr=false, float dx=0, float dy=0, short drad=10, short spec=0, /*short tr=-1,*/ short regid=-1, bool lieut=false);
	unit(basicunit u, float px=0, float py=0, short p=-1, short i=-1, float m=100, float ts=0, short vetlvl=0, float xp=0, float mx=-1, float my=-1, short si=-1, char us=US_AGGRESIVE, bool sotr=false, float dx=0, float dy=0, short drad=10, short spec=0,/* short tr=-1,*/ short regid=-1, bool lieut=false);
	void movement(bool siegesent=false);
	short checkmove(point checkwhat, float actspeed);

	//! This functions finds the nearest enemy unit that this unit can see and tells the unit to attack it.
	bool attackunitstance();
	void attackmovement();
	void fight(); //!< This is called when one unit attacks another. It updates reports if a unit is killed
	vector <pointex2> findallies(int foodneeded);
	void attackgoingtobstacle(char canmoveto[6]);
	void goingtobstacle0(char canmoveto[6]);
	void goingtobstacle();
	void train();
	void trainlieut();
	bool chkmvp1(tile &checkwhat, bool checkelev=true);
	short build(short buildwhat, int x, int y);
	void revealmapcreation();
	point checksurroundingarea(short cwidth, short cheight); //for ships only
	
	//! This checks a specified radius around the unit, and outputs all of the enemy units in it
	
	//! \param cyclethrough [in]
	//! \param startx [in]
	//! \param starty [in]
	//! \param allseenunits [out]
	void unit::checkrad(int cyclethrough, float startx, float starty, vector<pointex>& allseenunits);
protected:
	pointex getcandidate(int x, int y, double dist);
	bool pointonlinesegment(double x, double movetox, double newx);
	//void revealmapmvmt(double newx, double newy, double actspeed); //this reveals the new land that the player can see on the mini map and actual map
	void gather(char gatheringwhat);
	bool checknomove(bool siegesent);
};
#endif