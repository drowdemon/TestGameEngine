#include "stdafx.h"
#include "building.h"
#include "globals.h"
#include "button functions.h"

building::building(short h, short ma, short a, short g, short hh, short garmu, short ch, short gra, short gma, short r, short pid, float px, float py, short p, float bbb, float bbt, float bbl, float bbr, short i, short mxhld, short food, short wood, short gold, short stone, float w, float pheight, short radd, short nts, short beingb):basebuilding(h, a, g, hh, garmu, ch, gra, gma, r, pid, mxhld, pheight, radd)
{
	x=px;
	y=py;
	selected=false;
	player=p;
	boundingbox.bottom=bbb;
	boundingbox.left=bbl;
	boundingbox.right=bbr;
	boundingbox.top=bbt;
	trainer=-1;
	index=i;
	holding[0]=food;
	holding[1]=wood;
	holding[2]=gold;
	holding[3]=stone;
	createatx=createaty=-1;
	numtrainsimultaneously=nts;
	beingbuilt=beingb;
}
building::building(basebuilding b, float px, float py, short p, short i, float bbb, float bbt, float bbl, float bbr,  short food, short wood, short gold, short stone, short beingb) : basebuilding(b.health, b.armor, b.garrison, b.healhealth, b.garrisonedarmorup, b.chanceofbeinghit, b.garrisonedrangedattack, b.garrisonedmeleeattack, b.range, b.id, b.maxhold, b.width, b.height, b.radiustodistribute, b.numtrainsimultaneously,b.landorwater)
{
	x=px;
	y=py;
	selected=false;
	player=p;
	boundingbox.bottom=bbb;
	boundingbox.left=bbl;
	boundingbox.right=bbr;
	boundingbox.top=bbt;
	trainer=-1;
	index=i;
	holding[0]=food;
	holding[1]=wood;
	holding[2]=gold;
	holding[3]=stone;
	createatx=createaty=-1;
	beingbuilt=beingb;
}
void building::createunit(short createwhat, short pindex, short creating) //createwhat is the index of the unit to be created in unitsmade, creating is the index in creationqueueunits if it is time to actually create the unit
{
	if(creating==-1)//NOTE: what is assumed is that only the available createwhat's are displayed to the user, so its impossible for createwhat to be an invalid value. That means that the second unit in branch 2 will not have the option to become unit 1 in branch 2 already. It also means that if its too expensive to update a unit, that option will be greyed out. This also means that if this particular building cannot upgrade this unit, no options to upgrade will be shown
	{
		resources[player][0]-=allbuildableunits[createwhat].foodontraining;
		resources[player][1]-=allbuildableunits[createwhat].woodontraining;
		resources[player][2]-=allbuildableunits[createwhat].goldontraining;
		resources[player][3]-=allbuildableunits[createwhat].stoneontraining;
		holding[0]-=allbuildableunits[createwhat].foodontraining;
		holding[1]-=allbuildableunits[createwhat].woodontraining;
		holding[2]-=allbuildableunits[createwhat].goldontraining;
		holding[3]-=allbuildableunits[createwhat].stoneontraining;
		creationqueueunits[player][index].push_back(creationinfo(pindex, allbuildableunits[createwhat].timeontraining, createwhat));
	}
	else
	{
		if(pindex==-1) //its a villager, ship, or siege unit (created not trained from existing)
		{
			if(overwriteunits[player].size()!=0)
			{
				point p=checksurroundingarea(allbuildableunits[createwhat].whatisit, allbuildableunits[createwhat].width, allbuildableunits[createwhat].height);
				if(p.x!=-1 && p.y!=-1)
					actallunits[player][overwriteunits[player][overwriteunits[player].size()-1]]=unit(allbuildableunits[createwhat], p.x, p.y, player, overwriteunits[player].size()-1);
				else
					actallunits[player][overwriteunits[player][overwriteunits[player].size()-1]]=unit(allbuildableunits[createwhat], x+2, y+height, player, overwriteunits[player].size()-1);
				//allobstacles[player].resize(allobstacles[player].size()+2);
				if(createatx!=-1 && createaty!=-1)
				{
					allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->movetox=createatx;
					allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->movetoy=createaty;
				}
				else
				{
					if(p.x!=-1 && p.y!=-1)
					{
						allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->movetox=p.x;
						allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->movetoy=p.y;
					}
					else
						allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->movetoy=y+height+1;
				}
				map[(int)allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->y][(int)allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->x].player=player;
				map[(int)allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->y][(int)allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->x].index=overwriteunits[player].size()-1;
				map[(int)allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->y][(int)allunits[player][overwriteunits[player][overwriteunits[player].size()-1]]->x].uniton=true;
				overwriteunits[player].pop_back();
			}
			else
			{//if I can't overwrite anything
				point p=checksurroundingarea(allbuildableunits[createwhat].whatisit, allbuildableunits[createwhat].width, allbuildableunits[createwhat].height);
				if(p.x!=-1 && p.y!=-1)
					actallunits[player].push_back(unit(allbuildableunits[createwhat], p.x, p.y, player, actallunits[player].size()));
				else
					actallunits[player].push_back(unit(allbuildableunits[createwhat], x+2, y+height, player, actallunits[player].size()));
				allunits[player].push_back(&actallunits[player][actallunits[player].size()-1]);
				allobstacles[player].resize(allobstacles[player].size()+2);
				if(createatx!=-1 && createaty!=-1)
				{
					allunits[player][allunits[player].size()-1]->movetox=createatx;
					allunits[player][allunits[player].size()-1]->movetoy=createaty;
				}
				else
				{
					if(p.x!=-1 && p.y!=-1)
					{
						allunits[player][allunits[player].size()-1]->movetox=p.x;
						allunits[player][allunits[player].size()-1]->movetoy=p.y;
					}
					else
						allunits[player][allunits[player].size()-1]->movetoy=y+height+1;
				}
				for(int i=0; i<allbuildableunits[createwhat].width; i++)
				{
					for(int j=0; j<allbuildableunits[createwhat].height; j++)
					{
						map[(int)allunits[player][allunits[player].size()-1]->y+j][(int)allunits[player][allunits[player].size()-1]->x+i].player=player;
						map[(int)allunits[player][allunits[player].size()-1]->y+j][(int)allunits[player][allunits[player].size()-1]->x+i].index=allunits[player].size()-1;
						map[(int)allunits[player][allunits[player].size()-1]->y+j][(int)allunits[player][allunits[player].size()-1]->x+i].uniton=true;
					}
				}
			}
		}
		else //not a villager so this is an 'upgrade' not a .push_back
		{
			short tempg=allunits[player][pindex]->garrisoned;
			actallunits[player][pindex]=unit(allbuildableunits[createwhat], allunits[player][pindex]->x, allunits[player][pindex]->y, player, pindex, allunits[player][pindex]->moral);
			allunits[player][pindex]->garrisoned=tempg;
			for(unsigned int i=0; i<garrisoned.size(); i++)
			{
				if(allunits[player][garrisoned[i]]->index==pindex)
				{
					ungarrison(buttonparam(player,i+indexGarrisonbutton));
					break;
				}
			}
		}
	}
}
point building::checksurroundingarea(short utype, short uwidth, short uheight)
{
	int checkwidth=1;
	while(true)
	{
		int i=(int)x-checkwidth; //left of building
		int j=0;
		int numbad=0;
		if(i>=0 && i<MAPSIZE)
		{
			for(j=(int)y-checkwidth; j<(int)y+(int)height+checkwidth; j++)
			{
				if(j>=MAPSIZE || j<0 || i>=MAPSIZE || i<0)
					break;
				bool metagood=true;
				bool touchlandgood=false;
				for(int k=0; k<uwidth; k++)
				{
					for(int h=0; h<uheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=chkmvp1(map[j+h][i+k],player,-1,utype);//-1 is a hack, but a functional one
						if(good==false)
						{
							metagood=false;
							break;
						}
						if(touchlandgood!=true && checktouchingland(i+k,j+h)==true) //the first condition means that the function does not run if its unnecessary. Its a bit of an efficiency hack
						{
							touchlandgood=true;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true && touchlandgood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		j=(int)y-checkwidth;
		if(j>=0 && j<MAPSIZE)
		{
			for(i=(int)x; i<(int)x+(int)width; i++)
			{
				bool metagood=true;
				bool touchlandgood=false;
				for(int k=0; k<uwidth; k++)
				{
					for(int h=0; h<uheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=chkmvp1(map[j+h][i+k],player,-1,utype);//-1 is a hack, but a functional one
						if(good==false)
						{
							metagood=false;
							break;
						}
						if(touchlandgood!=true && checktouchingland(i+k,j+h)==true) //the first condition means that the function does not run if its unnecessary. Its a bit of an efficiency hack
						{
							touchlandgood=true;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true && touchlandgood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		i=(int)x+(int)width+checkwidth;
		if(i>=0 && i<MAPSIZE)
		{
			for(j=(int)y-checkwidth; j<(int)y+(int)height+checkwidth; j++)
			{
				bool metagood=true;
				bool touchlandgood=false;
				for(int k=0; k<uwidth; k++)
				{
					for(int h=0; h<uheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=chkmvp1(map[j+h][i+k],player,-1,utype);//-1 is a hack, but a functional one
						if(good==false)
						{
							metagood=false;
							break;
						}
						if(touchlandgood!=true && checktouchingland(i+k,j+h)==true) //the first condition means that the function does not run if its unnecessary. Its a bit of an efficiency hack
						{
							touchlandgood=true;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true && touchlandgood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		j=(int)y+(int)height+checkwidth;
		if(j>=0 && j<MAPSIZE)
		{
			for(i=(int)x; i<(int)x+(int)width; i++)
			{
				bool metagood=true;
				bool touchlandgood=false;
				for(int k=0; k<uwidth; k++)
				{
					for(int h=0; h<uheight; h++)
					{
						if(h+j>MAPSIZE || k+i>MAPSIZE)
						{
							metagood=false;
							break;
						}
						bool good=chkmvp1(map[j+h][i+k],player,-1,utype);//-1 is a hack, but a functional one
						if(good==false)
						{
							metagood=false;
							break;
						}
						if(touchlandgood!=true && checktouchingland(i+k,j+h)==true) //the first condition means that the function does not run if its unnecessary. Its a bit of an efficiency hack
						{
							touchlandgood=true;
						}
					}
					if(metagood==false)
						break;
				}
				if(metagood==true && touchlandgood==true)
					return point((float)i,(float)j);
			}
		}
		else
			numbad++;
		if(numbad==4)
			return (-1.0f,-1.0f); //failed
		checkwidth++;
	}
}