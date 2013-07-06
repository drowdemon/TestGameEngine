#ifndef button_h
#define button_h

#include <string>
#include "dataStructures.h"

//for dispwhen, bitshifting involved. Currently, it is impossible to select a unit that is not your own. However, to account for possible future expansion, the symbol is named YOUR_WHATEVER
#define NOTHING 1
#define YOUR_UNIT 2
#define YOUR_MULT_UNITS 4
#define YOUR_BUILDING 8
#define HOUSE_BUILDING 16 //The type of building that it must be in order to be displayed. Use with YOUR_BUILDING. In this case its id=1 - house
#define BARRACKS_BUILDING 32 //id=2
#define DOCK_BUILDING     64 //id=3
#define YOUR_SHIP		  128 //You selected a ship
//! This displays only if the selected unit(s) are members of a regiment
#define YOUR_REGIMENT_MEMBER 256 
//! Your on page 1 (building list)
#define PAGE_1			  512 
//!Your on page 2 (building list)
#define PAGE_2			  1024 
#define ARCHERY_BUILDING  2048 //id=4
#define STABLE_BUILDING   4096 //id=5
#define CASTLE_BUILDING   8192 //id=13
#define RESEARCH_BUILDING 16384 //id=7 (research facility)

using namespace std;
class button
{
public:
	int x;
	int y;
	int width;
	int height;
	string text;
	void (*func)(buttonparam);
	unsigned long long dispwhen;
	int unitorbuilding;
	button(int px, int py, int w, int h, string txt, void (*f)(buttonparam), unsigned long long dw, int uorb=-1);
};
#endif
