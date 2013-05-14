#ifndef astarsearch_h
#define astarsearch_h

#include "data structures.h"
#include "unit.h"
#include <cmath>
//! This documents a problem that will be used by astar search. Used for obstacle avoidance.
class problem
{
public:
	point goalstate; //!< This is the point that one wants to go to
	bool dirx;       //!< In x, do I need to be left of goalstate.x (false) or right of it (true)
	bool diry;		 //!< Same as dirx but for for y, -1(false), 1(true)
	problem(point gs, bool dx, bool dy); //!< A constructor. Takes a point, and dirx, diry
	void getsuccessors(point &state, vector<point> &add, vector<point> &moves, float speed, unit *u); //!< This retrieves the valid moves from a point. It takes the state (position) a unit is in, its speed, and a pointer to the unit. It outputs the moves to make, and what to add to the search fringe
	bool goal(point state); //!< This checks if a given point is the goal state
	float heuristic(point &state); //!< This returns a heuristic for how close a given point is to the goal state
};
//! Stores the data needed to construct the search fringe in astarsearch
class searchfringe
{
public:
	point state; //!< The current state
	float weighttodate; //!< How expensive the current sequence is
	vector<point> movestodate; //!< What has been done in this search so far
	searchfringe(point pState, float wtd, vector<point> *mtd); //!< A constructor, that takes all of the variables listed
};
#endif
