#ifndef ADVANCEMENT_H
#define	ADVANCEMENT_H

#include "dataStructures.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

#define UNITUNLOCK 0
#define BUILDINGUNLOCK 1
#define TECHUNLOCK 2

class advancement
{
public:
	vector<advReq> unitsreq; //!< The units one must have for advancement, with quantity
	vector<advReq> buildingsreq; //!< The buildings one must have for advancement, with quantity
	
	int unlockIndex; //!< What the index of the unlock will be: i.e. if unlockWhat is a unit, this is the index in unitAllowed of that unit
	int unlockWhat; //!< What will be unlocked: unit, building, technology, etc. See #define
	
	advancement(vector<advReq> ur, vector<advReq> buildingsreq, int ui, int uw);
	bool checkAdv(short player);
	string unlockMessage();
};

#endif	/* ADVANCEMENT_H */

