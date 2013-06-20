#include "astarsearch.h"
extern vector < vector<tile> > map;
problem::problem(point gs, bool dx, bool dy)
{
	goalstate.x=gs.x;
	goalstate.y=gs.y;
	dirx=dx;
	diry=dy;
}
bool problem::goal(point state)
{
	bool goalx=false;
	bool goaly=false;
	if((state.x-goalstate.x>=0 && dirx==true) || (state.x-goalstate.x<=0 && dirx==false))
		goalx=true;
	if((state.y-goalstate.y>=0 && diry==true) || (state.y-goalstate.y<=0 && diry==false))
		goaly=true;
	return (goaly && goalx); //This disgusts me. A lot.
}
float problem::heuristic(point &state)
{
	float dx=state.x-goalstate.x;
	if((dx>=0 && dirx==true) || (dx<=0 && dirx==false))
		dx=0;
	float dy=state.y-goalstate.y;
	if((dy>=0 && diry==true) || (dy<=0 && diry==false))
		dy=0;
	return sqrt(dx*dx+dy*dy)/2; //arbitrary weighting used.
	//return sqrt(pow(state.x-goalstate.x,2)+pow(state.y-goalstate.y,2));
}
void problem::getsuccessors(point &state, vector<point> &add, vector<point> &moves, float speed, unit *u)
{
	point checkthese[4]={point(-speed,0),point(0,-speed),point(0,speed),point(speed,0)};
	//point checkthese[4]={point(-0.9f,0),point(0,-0.9f),point(0,0.9f),point(0.9f,0)};
	for(int k=0; k<4; k++)
	{
        int maxsofar=0;
        for(int h=1; h<=10; h++)
		{
            float i=checkthese[k].y*h;
            float j=checkthese[k].x*h;
            int good[4]={-1,-1,-1,-1};
            good[0]=u->chkmvp1(map[(int)(state.y+i)][(int)(state.x+j)]);
            if(good[0]==0)
                good[1]=u->chkmvp1(map[(int)(state.y+i+u->height-0.001f)][(int)(state.x+j)]);
            if(good[1]==0)
                good[2]=u->chkmvp1(map[(int)(state.y+i)][(int)(state.x+j+u->width-0.001f)]);
            if(good[2]==0)
                good[3]=u->chkmvp1(map[(int)(state.y+i+u->height-0.001f)][(int)(state.x+j+u->width-0.001f)]);
            if(good[3]==0)
            {
                maxsofar=h;
                //add.push_back(point(state.x+j,state.y+i));
                //moves.push_back(point(j,i));
            }
            else
                break;
        }
        if(maxsofar!=0)
        {
            add.push_back(point(state.x+checkthese[k].x*maxsofar,state.y+checkthese[k].y*maxsofar));
            moves.push_back(point(checkthese[k].x*maxsofar,checkthese[k].y*maxsofar));
        }
        if(maxsofar>2)
        {
            add.push_back(point(state.x+checkthese[k].x*maxsofar/2.0,state.y+checkthese[k].y*maxsofar/2.0));
            moves.push_back(point(checkthese[k].x*maxsofar/2.0,checkthese[k].y*maxsofar/2.0));
        }
	}
}

searchfringe::searchfringe(point pState, float wtd, vector<point> &mtd)
{
	state=pState;
	weighttodate=wtd;
	movestodate=mtd;
}
