#ifndef siege_h
#define siege_h

#include "unit.h"
class siege : public unit
{
public://note: speed will be 0, in reality it will depend on the manning units. Ditto LOS. speed will be used in some variation of actualspeed=speed*manning.size(), as will attackspeed
	short minmanning; //the minimum number of units required to man the siege machine
	short maxmanning; //the maximum ditto
	vector <short> manning; // a vec of the units indexes in allunits that are manning this machine
	siege(unit u, short min=0, short max=0/*, float k=0*/);
};
#endif