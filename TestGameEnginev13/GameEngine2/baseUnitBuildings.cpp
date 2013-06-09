#include "baseUnitBuildings.h"
baseunit::baseunit(float h, short ma, short ra, short a, short ba, float LOS, float s, short as, short fc, short sn, float ar, short pid, float c, short high, short w, short bs, short mxhld, unsigned char wisit, unsigned char ming, unsigned char maxg)
{
	health=h;
	meleeattack=ma;
	rangedattack=ra;
	armor=a;
	buildingattack=ba;
	los=LOS;
	speed=s;
	attackspeed=as;
	foodconsumed=fc;
	sleepneeded=sn;
	attackrange=ar;
	id=pid;
	camouflage=c;
	height=high;
	width=w;
	buildspeed=bs;
	maxhold=mxhld;
	whatisit=wisit;
	mingarrison=ming;
	maxgarrison=maxg;
}
basicunit::basicunit(float h, short ma, short ra, short a, short ba, float LOS, float s, short as, short fc, short sn, float ar, short pid, float c, short high, short w, short bs, short mxhld, unsigned char wisit, unsigned char ming, unsigned char maxg,  char tf, short ft, short wt, short gt, short st, short tt, string n) : baseunit(h, ma, ra, a, ba, LOS, s, as, fc, sn, ar, pid, c, high, w, bs, mxhld, wisit, ming, maxg)
{ 
	/*health=h;
	meleeattack=ma;
	rangedattack=ra;
	armor=a;
	buildingattack=ba;
	los=LOS;
	attackspeed=as;
	foodconsumed=fc;
	sleepneeded=sn;
	attackrange=ar;
	camouflage=c;
	width=w;
	height=ht;*/
	trainingcof=tf;
	foodontraining=ft;
	woodontraining=wt;
	goldontraining=gt;
	stoneontraining=st;
	timeontraining=tt;
	name=n;
}
basebuilding::basebuilding(short h, short a, short g, float hh, short garmu, short ch, short gra, short gma, short r, short pid, short mxhld, float w, float pheight, short radd, short nts, bool low)
{
	health=h;
	armor=a;
	garrison=g;
	healhealth=hh;
	garrisonedarmorup=garmu;
	chanceofbeinghit=ch;
	garrisonedrangedattack=gra;
	garrisonedmeleeattack=gma;
	range=r;
	id=pid;
	maxhold=mxhld;
	width=w;
	height=pheight;
	radiustodistribute=radd;
	numtrainsimultaneously=nts;
	landorwater=low;
}
basicbuilding::basicbuilding(short h, short a, short g, float hh, short garmu, short ch, short gra, short gma, short r, short pid, short mxhld, float w, float pheight, short radd, short nts, bool low, short fb, short wb, short gb, short sb, short tt, string n)
{
	health=h;
	armor=a;
	garrison=g;
	healhealth=hh;
	garrisonedarmorup=garmu;
	chanceofbeinghit=ch;
	garrisonedrangedattack=gra;
	garrisonedmeleeattack=gma;
	range=r;
	id=pid;
	maxhold=mxhld;
	width=w;
	height=pheight;
	radiustodistribute=radd;
	numtrainsimultaneously=nts;
	landorwater=low;
	foodtobuild=fb;
	woodtobuild=wb;
	goldtobuild=gb;
	stonetobuild=sb;
	timetobuild=tt;
	name=n;
}
basicbuilding::basicbuilding(basebuilding bb, short fb, short wb, short gb, short sb, short tt, string n) : basebuilding(bb.health, bb.armor, bb.garrison, bb.healhealth, bb.garrisonedarmorup, bb.chanceofbeinghit, bb.garrisonedrangedattack, bb.garrisonedmeleeattack, bb.range, bb.id, bb.maxhold, bb.width, bb.height, bb.radiustodistribute, bb.numtrainsimultaneously, bb.landorwater)
{
	foodtobuild=fb;
	woodtobuild=wb;
	goldtobuild=gb;
	stonetobuild=sb;
	timetobuild=tt;
	name=n;
}
