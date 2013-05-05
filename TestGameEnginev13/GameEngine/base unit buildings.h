#ifndef base_unit_buildings_h
#define base_unit_buildings_h
#include <vector>
#include <string.h>
using namespace std;
//! Data that every unit must have, and is not unique for each unit. 
class baseunit
{
public:
	float health; //!< (Initially) The maximum health a unit can have.
	short meleeattack; //!< Self explanatory. for a ranged unit this will either be lower than ranged att, or their melee defense will be much lower, or both
	short rangedattack; //!< Self explanatory. for melee this will be 0
	short armor; //!< this will be how much damage will be absorbed, if its something like plate armor, should absorb almost all. [Include rnd chance of missile/wep going into crease/hole and doing extra damage or a 1-hit kill] - not done
	//!< damage done will be attack(whichever is applicable)-armor
	short buildingattack; //!< this will be near 0 for archers/melee, and 0 for most ships. Exceptions - cannons, fire arrows, etc.
	float los; //!< line of sight, measured in tiles
	float speed; //!< musn't be 0, unless unit is immobile, should = how many tiles (decimal) unit moves in one frame
	short attackspeed; //!< musn't be 0, and its inverse #of frames=attackspeed / attack -> bigger=slower
	short foodconsumed; //!< how much food do they need daily... more or less unchanging from unit to unit, [however make a random +/- value] - not done yet
	short sleepneeded; //!< how much time do they spend asleep per day. moral will go down if amount of sleep is too low
	float attackrange; //!< how far can it attack; 3 for melee units (thats the MELEE #define)
	short id;		   //!< each unit will have a unique id, unique from building id too. (unique per unit type, not per unit). This is the position in allbuildableunits
	float camouflage;  //!< how many tiles will be subtracted from an enemy units LOS while it is looking. This subtraction won't be applicable to seeing map tiles, only to seeing units on them
	short width;	   //!< how much space they actually take up in the x/y plane
	short height;	   //!< how much space they actually take up in the x/y plane
	short buildspeed;  //!< how fast they can build things (This is a villager-like characteristic, but applicable to all)
	short maxhold;     //!< the maximum amount of resources a unit can hold, what everything in holding can sum up to
	unsigned char whatisit; //!< 0 is regular unit (cav/archer/infantry) (removed: 1 is villager), 2 is siege, 3 is ship
	unsigned char maxgarrison; //!< this variable is for ships only, the maximum amount of units that can garrison in it, else (not ship) it will be -1
	unsigned char mingarrison; //!< this variable is for ships only, the minimum number of units that must be garrisoned for the ship to be usable
	baseunit(float h=0, short ma=0, short ra=0, short a=0, short ba=0, float LOS=0, float s=0, short as=0, short fc=0, short sn=0, float ar=0, short pid=0, float c=0, short high=0, short w=0, short bs=0, short mxhld=0, unsigned char wisit=1, unsigned char ming=-1, unsigned char maxg=-1); //!< Constructor/default constructor. Accepts all of the above variables
};
//! the stats in allbuildableunits
class basicunit : public baseunit 
{
public:
	//float health;
	//short meleeattack;//for a ranged unit this will either be lower than ranged att, or their melee defense will be much lower, or both
	//short rangedattack;//for melee this will be 0
	//short armor; //this will be how much damage will be absorbed, if its something like plate armor, should absorb almost all. Include rnd chance of missile/wep going into crease/hole and doing extra damage or a 1-hit kill
	//short buildingattack;
	//float los;
	//short attackspeed;
	//short foodconsumed;
	//short sleepneeded;
	//float attackrange;
	//float camouflage;
	//short width;
	//short height;
	char trainingcof; //!< DIVIDE BY 100 FOR USE. training coeficient, see excel document, sheet 2 for details. This is a. 
	short foodontraining; //!< food required to train it. Rest are similar
	short woodontraining;
	short goldontraining;
	short stoneontraining;
	short timeontraining;
	string name;		//!< What is this unit called
	basicunit(float h=0, short ma=0, short ra=0, short a=0, short ba=0, float LOS=0, float s=0, short as=0, short fc=0, short sn=0, float ar=0, short pid=0, float c=0, short high=0, short w=0, short bs=0, short mxhld=0, unsigned char wisit=1, unsigned char ming=-1, unsigned char maxg=-1,  char tf=0, short ft=0, short wt=0, short gt=0, short st=0, short tt=0, string n=""); //!< [default] constructor
};
//! A building, stores all info that does not change from building to building and is required for each building
class basebuilding
{
public:
	short health; //!< Total max health
	short armor; //!< this will be how much damage will be absorbed
	short garrison; //!< how many people can enter the building
	float healhealth; //!< hospital only, self explanatory
	short garrisonedarmorup; //!< how much the armor of all garrisoned units will go up... it should be a large number 
	//!< anyone attacking a building w/ garrisoned units will do some damage to the building and some to the defenders... archers won't damage building and will have a chance of damaging people inside
	short chanceofbeinghit;//!< chance of being hit when garrisoned in a building.
	short garrisonedrangedattack; //!< how much attack will go up for all garrisoned ranged units
	short garrisonedmeleeattack; //!< how much attack will go up for all garrisoned melee units
	short range; //!< the range of all ranged units in the building, range of melee units is the #define
	short id;    //!< each building will have a unique id (index in allbuildablebuildings
	vector <short> unitsmade; //!< holds ids of the units that this building can build, which will be used to access an external array of all units (allbuildableunits)
	vector <short> techsresearchable; //!< [not implemented] same as units made, but for techs that it can research.
	float width; //!< Space it takes up on map on the xy plane
	float height; //!< Space it takes up on map on the xy plane
	short maxhold; //!< the max resources it can hold, total on all resources
	short radiustodistribute; //!< the radius within which it can distribute resources to units.
	short numtrainsimultaneously; //!< The number of units it can train simultaneously. [not implemented]
	bool landorwater; //!< true==land (water buildings exist. i.e. walls, port, etc.
	basebuilding(short h=0, short a=0, short g=0, float hh=0, short garmu=0, short ch=0, short gra=0, short gma=0, short r=0, short pid=0, short mxhld=0, float w=1, float pheight=1, short radd=0, short nts=1, bool low=true); //!< [default] constructor
};
//! stores stats of buildings in allbuildablebuildings
class basicbuilding : public basebuilding 
{
public:
	short foodtobuild; //!< Resources required to build this
	short woodtobuild;
	short stonetobuild;
	short goldtobuild;
	short timetobuild;
	string name; //!< What the building is called
	basicbuilding(short h=0, short a=0, short g=0, float hh=0, short garmu=0, short ch=0, short gra=0, short gma=0, short r=0, short pid=0, short mxhld=0, float w=1, float pheight=1, short radd=0, short nts=1, bool los=true, short fb=0, short wb=0, short gb=0, short sb=0, short tt=0, string n=""); //!< [default] constructor
	basicbuilding(basebuilding bb, short fb=0, short wb=0, short gb=0, short sb=0, short tt=0,string n=""); //!< [default] constructor
};
#endif
