#ifndef ANIMAL_H
#define	ANIMAL_H

#include "tile.h"

class animal
{
public:
	short health;
	short maxhealth;
	float speed;
	short x;
	short y;
	short dir;
	short reproctr;
	short reprowhen;
	short runtime;
	short prevTileStyle;
	animal(short h, float s, short px, short py, short d, short rw, short pts=TS_GRASS);
	void move();
};

#endif	/* ANIMAL_H */

