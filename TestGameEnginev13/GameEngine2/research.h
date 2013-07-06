#ifndef RESEARCH_H
#define	RESEARCH_H

#include <string>

using namespace std;

//! Apply to changewhat in Research
#define NONE              0
#define INFANTRY          1
#define ARCHERS           2
#define ARCHER_CAVALRY    4
#define HEAVY_CAVALRY     8
#define SHIPS             16
#define SIEGE             32
#define LAND_BUILDINGS    64
#define WATER_BUILDINGS   128
#define SCOUTS			  256
#define SPIES			  512
#define ASSASSINS	      1024
#define TRANSPORT		  2048
class Research
{
public:
	short changewhat; //what type of unit/building it affects
	short price[4];
	short time;
	short researchwhere; //what building can research this
	short armor;  //added
	short attack; //added
	short range; //added
	short accuracy; //added
	short gather[4];      //added   //speed up for each resource
	short buildingattack; //added
	short buildspeed; //added
	short maxgarrison; //added
	short los; //added line of sight
	string name;
	Research(short cw, short food, short wood, short gold, short stone, short t, short rw, short arm, short att, short r, short acc, short fs, short ws, short gs, short ss, short ba, short bs, short mg, short los, string n);
	bool checkResearch(int unitid, bool building=false);
};

#endif	/* RESEARCH_H */

