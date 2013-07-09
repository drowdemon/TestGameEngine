#include <cstdlib>

#include "animal.h"
#include "globals.h"
#include "tile.h"

animal::animal(short h, float s, short px, short py, short d, short rw, short pts)
{
    maxhealth=health=h;
    speed=s;
    x=px;
    y=py;
    dir=d;
    reprowhen=rw;
    reproctr=0;
    runtime=0;
    prevTileStyle=pts;
}
void animal::move()
{
    int change=dir;
    if(change==0)
    {
        if(health<maxhealth)
        {
            dir=(rand()%4)+1;
            change=dir;
        }
        else
        {
            change=rand()%5;
            reproctr+=FPS/5; //how often this happens
        }
    }
    else
        runtime++;
    if(runtime>=17)
    {
        dir=0;
        runtime=0;
        maxhealth=health; //so that it doesn't immediately run again
    }
    if(reproctr>=reprowhen)
    {
        if(chkmvp1(map[y][x+1], 0, -1, 0))
        {
            allAnimals.push_back(animal(health, speed, x+1, y, 0, reprowhen,map[y][x+1].tilestyle));
            map[y][x+1].tilestyle=TS_ANIMAL;
            map[y][x+1].resources[0]=200;
        }
        else if(chkmvp1(map[y][x-1], 0, -1, 0))
        {
            allAnimals.push_back(animal(health, speed, x-1, y, 0, reprowhen,map[y][x-1].tilestyle));
            map[y][x-1].tilestyle=TS_ANIMAL;
            map[y][x-1].resources[0]=200;
        }
        else if(chkmvp1(map[y+1][x], 0, -1, 0))
        {
            allAnimals.push_back(animal(health, speed, x, y+1, 0, reprowhen,map[y+1][x].tilestyle));
            map[y+1][x].tilestyle=TS_ANIMAL;
            map[y+1][x].resources[0]=200;
        }
        else if(chkmvp1(map[y-1][x], 0, -1, 0))
        {
            allAnimals.push_back(animal(health, speed, x, y-1, 0, reprowhen,map[y-1][x].tilestyle));
            map[y-1][x].tilestyle=TS_ANIMAL;
            map[y-1][x].resources[0]=200;
        }
        reproctr=0;
    }
    bool moved=false;
    if(change==1)
    {
        if(chkmvp1(map[y][x+1], 0, -1, 0))
        {
            map[y][x].tilestyle=prevTileStyle;
            map[y][x].resources[0]=0;
            prevTileStyle=map[y][x+1].tilestyle;
            map[y][x+1].tilestyle=TS_ANIMAL;
            map[y][x+1].resources[0]=200;
            x++;
            moved=true;
        }
    }
    else if(change==2)
    {
        if(chkmvp1(map[y][x-1], 0, -1, 0))
        {
            map[y][x].tilestyle=prevTileStyle;
            map[y][x].resources[0]=0;
            prevTileStyle=map[y][x-1].tilestyle;
            map[y][x-1].tilestyle=TS_ANIMAL;
            map[y][x-1].resources[0]=200;
            x--;
            moved=true;
        }
    }
    else if(change==3)
    {
        if(chkmvp1(map[y+1][x], 0, -1, 0))
        {
            map[y][x].tilestyle=prevTileStyle;
            map[y][x].resources[0]=0;
            prevTileStyle=map[y+1][x].tilestyle;
            map[y+1][x].tilestyle=TS_ANIMAL;
            map[y+1][x].resources[0]=200;
            y++;
            moved=true;
        }
    }
    else if(change==4)
    {
        if(chkmvp1(map[y-1][x], 0, -1, 0))
        {
            map[y][x].tilestyle=prevTileStyle;
            map[y][x].resources[0]=0;
            prevTileStyle=map[y-1][x].tilestyle;
            map[y-1][x].tilestyle=TS_ANIMAL;
            map[y-1][x].resources[0]=200;
            y--;
            moved=true;
        }
    }
    if(!moved && dir!=0)
    {
        dir=(rand()%4)+1;
    }
}