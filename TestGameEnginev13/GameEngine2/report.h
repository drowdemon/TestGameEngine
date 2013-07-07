#ifndef REPORT_H
#define REPORT_H

#include <vector>
#include "dataStructures.h"
#include "baseUnitBuildings2.h"
using namespace std;

//!	\file report.h This contains class report

//! This class is used to store and display a report to the user
class unit;
class report;

string inttos(int num);

extern vector <vector <unit*> > allunits;
extern vector<basicunit> allbuildableunits;
extern char* (report::*reportfuncs[3])();
extern report *currRep;

//! The report contains the events that happened in a regiment since the 'start report recording' button was pressed.
class report
{
public:
	vector <reportunitinfo> unitslost; //!< This is a vector of information about the unit(s) lost

	vector <short> enemykilledids; //!< This contains the ids of the killed enemy units, id in enemyseenunits
	
	//! This contains ids of surviving enemy units
	vector <short> enemylivedids; //can be derived. This is for easy access. id in enemyseenunits
	vector <reportsimpleunitinfo> enemyseenunits; //!< this contains basic info about all of the enemy units that were seen
	vector <point> seenTiles; //!< This contains tiles the unit has seen.  //MAKE IT AN OPTIMIZED HASHTABLE/LIST, or rather a linked hashset and list. So for accessing to see if its something new, use a hashtable, for listing/looping through use the list, add/erase from both. 

	//! Completely empty default constructor
	report();

	//! This takes a vector of seen enemy units, and uses it to update enemyseenunits
	void updateseenunits(vector<pointex> &seenunits);
	//! The same, for merge()
	void updateseenunits(vector<reportsimpleunitinfo> &seenunits);
	//! This takes an enemy unit that was killed, and records that
	void updatedkilledunits(short player, short index);
	//! This gives a report
	void give(short player);
	//! Merges two reports.
	void merge(report r); 
	//! This updates the text in the report
	char* gentxtMyKilled();
	char* gentxtSeen();
	char* gentxtEnemyKilled();
};
#endif
