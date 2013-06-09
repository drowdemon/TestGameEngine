#ifndef regiment_h
#define regiment_h
#include <vector>
#include "report.h"
using namespace std;

//! \file regiment.h This is the file that contains the regiment class

//! class for regiments: collections of unit under a lieutenant
class regiment
{
public:
	vector<short> unitids; //!< ids of all of the units inside. Not in constructor
	short reglvl; //!< level of the regiment. Not in constructor
	short lieutenant; //!< id of lieutenant in allunits
	short player; //!< player that owns reg
	short index; //!< index in allregiments of the regiment
	bool recording; //!< Whether this regiment is recording all that it does for the report. Not in constructor - always false. 
	report rep; //!< A report. Non-empty if the above is true
	
	//! Constructor
	//! \param lieu lieutenant argument
	//! \param p player argument
	//! \param i index argument
	regiment(short lieu=-1, short p=0, short i=0);

	//! This calculates the regiment level of the regiment
	
	//! It is based on the veterancy levels of the contained units
	void calcreglvl();

	//! This adds one unit to the regiment
	bool addunits(short id);

	//! This adds multiple units to the regiment
	bool addunits(vector <short> &ids);

	//! This removes a unit from the regiment, due to death. 
	
	//! id is index in allunits. Updates report if recording.
	void removeunit(short id);
};
#endif