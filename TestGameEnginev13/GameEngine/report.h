#ifndef REPORT_H
#define REPORT_H

#include <vector>
#include "data structures.h"
#include "base unit buildings.h"
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

	//! Completely empty default constructor
	report();

	//! This takes a vector of seen enemy units, and uses it to update enemyseenunits
	void updateseenunits(vector<pointex> &seenunits);
	//! This takes an enemy unit that was killed, and records that
	void updatedkilledunits(short player, short index);
	//! This gives a report
	void give();
	//! This updates the text in the report
	char* gentxtMyKilled();
	char* gentxtSeen();
	char* gentxtEnemyKilled();
};
#endif
