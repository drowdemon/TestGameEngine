#ifndef building_h
#define building_h
#include "base unit buildings.h"
#include "data structures.h"
class building : public basebuilding //all buildings, including towers, walls, castles, houses etc.
{
public:
	float x;
	float y;
	bool selected; //whether or not the player that owns it has selected it. 
	//short player; //which player owns it, for the first index in allbuildings
	myrect boundingbox; //the 2d bounding box of the image that is the building
	short trainer; //who is the trainer in the building. index in allunits. only applicable to buildings that train units. if the vetlvl of this unit is < the lvl of a garrisoned unit, that unit will train on his own
	vector <short> garrisoned; //the ids in allunits of the units in this building
	short player; //the player that owns it
	short index; //the buildings index in allbuildings
	short holding[4]; //the resources its holding 0=food, 1=wood, 2=gold, 3=stone
	float createatx; //where the created units should go
	float createaty;
	short beingbuilt;
	building(short h=0, short ma=0, short a=0, short g=0, short hh=0, short garmu=0, short ch=0, short gra=0, short gma=0, short r=0, short pid=0, float px=0, float py=0, short p=0, float bbb=0, float bbt=0, float bbl=0, float bbr=0, short i=0, short mxhld=0, short food=0, short wood=0, short gold=0, short stone=0, float w=1, float pheight=1, short radd=0, short nts=1, short beingb=-1);
	building(basebuilding b, float px=0, float py=0, short p=0, short i=0, float bbb=0, float bbt=0, float bbl=0, float bbr=0,  short food=0, short wood=0, short gold=0, short stone=0, short beingb=-1);
	void createunit(short createwhat, short unitindex, short creating=-1);
	point checksurroundingarea(short utype, short width, short height); //unit type: land or water : var whatisit, returns a valid point to ungarrison to
};
#endif