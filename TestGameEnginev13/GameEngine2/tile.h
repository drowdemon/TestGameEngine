#ifndef tile_h
#define tile_h
#include "dataStructures.h"

//TS==tile style
#define TS_GRASS 1 //clear (grass)
#define TS_USELESS 2 //occupied by useless stuff (rock, mountain)
//#define TS_UNIT 3 //unit on grass
#define TS_BUILDING 3 //building on grass
#define TS_TREE 4 //tree on grass (camoflauge bonus and wood, can be cut down)
#define TS_GOLD 5 //gold under grass (must dig+discover)
#define TS_STONE 6 //useful stone under grass (must dig and discover)
#define TS_WATER 7 //water, only ships can move on it, can build walls, gate, and docks on it
#define TS_ANIMAL 8 // animal on grass (hunt)
#define TS_FISH 9 //fish under water, must fish for a while and find
#define TS_BERRIES 10 //wild berries and like on bush on grass (food/camo, can be emptied of berries, but not auto destroy. Can destroy if you want to)
#define TS_BUSHES 11 //bushes for camo bonus (can be optionally destroyed)
#define TS_ROAD 12 //must be built, expiediates travel for supply line, trade caravan, troops, basically everything
//#define TS_MINE 14 //there is a mine(i.e. for stone/gold) here, cannot walk over, but can walk into to work. A mine will be a single tile building.
#define TS_WATERBUILDING 13 //wall or gate on the water (also functions like a bridge), can be walked on by land units
#define TS_GATE 14 //Can be crossed by units of the same player/allied player, but not enemy. 

class tile//any and all map tiles, so this will be X by X pixels (probably a square), and approximately the size of one soldier
{
public://poss delete num 9 in following list for which tile (animal on grass)
	unsigned char tilestyle; //use tile style #define
	unsigned char locoftunnel; //1 is nothing, 2 is entrance/exit, 3 is tunnel, can't be underwater(will collapse and kill everyone inside after 2-3 tiles) ditto if its more than a certain number of tiles long, which can be extended via research
	short resources[4]; //[0] is food, 1=wood, 2=gold, 3=stone
	short unitindex; //the index and player of the unit standing on it if uniton==true, or that of the building if its TS_BUILDING/TS_WATEERBUILDING, -1 if empty
	short unitplayer;
	short buildingindex;
	short buildingplayer;
	unsigned char unitonMineon; //is there a unit on the tile: false=no, 1=unit. If there's a mine i.e. digging mine there: 2=yes. Bit shifting.             //IF I EVER USE THIS FOR EVERYTHING ELSE, BE SURE TO CHANGE LINE IN MAINTIMERPROC() THAT CHANGE THE TILESTYLE TO REFLECT THAT, WITH A SPARE & STATEMENT, SAME IN UNIT.CPP WHEN ITS BUILT.
	short inbuildingrad; //if not in building radius, its -1. whether or not a tile is in the radius of a building which can supply resources, and if it is it will be that buildings index in allbuildings, else it will be -1
	mychar whichplayer; //which player (in allunits) the tile can supply with resources(false), and which player (index in players[]) a building that can attack that square belongs to(true)
	char elevation;
	tile(short iunit=0, short punit=0, short ibuilding=0, short pbuilding=0, int ts=1, int lt=1, int f=0, int w=0, int g=0, int s=0, /*float c=0,*/ unsigned char uo=false, short ibr=-1, unsigned char wpf=0, unsigned char wpt=0, char elev=0);
};
#endif