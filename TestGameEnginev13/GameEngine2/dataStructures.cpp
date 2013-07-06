#include "dataStructures.h"

//! \file "data structures.cpp"
//! \brief This file contains various classes used solely for data storage

//! \file "data structures.cpp" Mostly, member functions consist of only constructors.

creationinfo::creationinfo(short pi, short tr, short cw)
{
	pindex=pi;
	timeremaining=tr;
	createwhat=cw;
}
obstaclethreadparam::obstaclethreadparam(float xc, float yc, float ux, float uy, float p_upx, float p_upy, int p, int i, int i2, float h, float w, float gxc, float gyc)
{
	xchange=xc;
	ychange=yc;
	unitx=ux;
	unity=uy;
	upx=p_upx;
	upy=p_upy;
	player=p;
	index=i;
	index2=i2;
	height=h;
	width=w;
	goodxchange=gxc;
	goodychange=gyc;
}
point::point(float px, float py)
{
	x=px;
	y=py;
}
bool operator==(point &p1, point &p2)
{
	return (p1.x==p2.x && p1.y==p2.y);
}
pointex::pointex(double px, double py, double d, bool u, short p, short i)
{
	x=px;
	y=py;
	dist=d;
	unit=u;
	player=p;
	index=i;
}
pointex2::pointex2(short i, double d, short f)
{
	index=i;
	dist=d;
	food=f;
}
myrect::myrect(float t, float b, float r, float le)
{
	top=t;
	bottom=b;
	right=r;
	left=le;
}
obstaclethreadret::obstaclethreadret(float gxc, float gyc)
{
	//dist=d;
	goodxchange=gxc;
	goodychange=gyc;
}
mychar::mychar()
{
	data=0;
}
unsigned char mychar::get(bool whichone) //false is bits 0-3, true is bits 4-7, 0-3 being from the right
{
	if(whichone==true)
		return (data & (15 << 4)) >> 4;
	else
		return data & 15;
}
void mychar::set(bool whichone, unsigned char towhat) //same for whichone
{
	if(whichone==false)
	{
		data&=(15<<4);
		data|=towhat;
	}
	else
	{
		data&=15;
		data|=(towhat<<4);
	}
}
buttonparam::buttonparam(short p, short i, short uorb)
{
	player=p;
	index=i;
	unitorbuilding=uorb;
}
display::display(string txt, int p_x, int p_y, unsigned long long dw, string (*f)(int))
{
	text=txt;
	x=p_x;
	y=p_y;
	dispwhen=dw;
	func=f;
}
reportunitinfo::reportunitinfo(short p_id, short vlvl, bool l, short llvl)
{
	id=p_id;
	vetlvl=vlvl;
	lieut=l;
	lieutlvl=llvl;
}
reportsimpleunitinfo::reportsimpleunitinfo(short esid, point esl, short ep, short ei)
{
	enemyseenid=esid;
	enemyseenloc=esl;
	enemyplayer=ep;
	enemyindex=ei;
}
RGB::RGB(int pr, int pg, int pb)
{
	r=pr;
	g=pg;
	b=pb;
}
ARGB::ARGB(int pa, int pr, int pg, int pb)
{
	a=pa;
	r=pr;
	g=pg;
	b=pb;
}
ErrorMSG::ErrorMSG(string m, int px, int py, int t) : color(150,0,0)
{
	msg=m;
	x=px;
	y=py;
	time=t;
}
ErrorMSG::ErrorMSG(string m, int px, int py, int t, RGB c) : color(c)
{
	msg=m;
	x=px;
	y=py;
	time=t;
}
ErrorMSG::ErrorMSG() : color(150,0,0)
{
	msg="";
	x=-1;
	y=-1;
	time=-1;
}
mouseOver::mouseOver(string txt, int p_x, int p_y, unsigned long long dw, string (*f)(int))
{
	text=txt;
	x=p_x;
	y=p_y;
	dispwith=dw;
	func=f;
}
progressBar::progressBar(int px, int py, int w, string txt, int disp, double(*f)(int))
{
    x=px;
    y=py;
    width=w;
    text=txt;
    dispwhen=disp;
    func=f;
}
advReq::advReq(short wr, short ar)
{
    whatsreq=wr;
    amntreq=ar;
}
powerBonus::powerBonus(vector<short> aw, double ab, double ap, double db, double dp)
{
    againstWhat=aw;
    attackBonus=ab;
    attackPercentBonus=ap;
    defenseBonus=db;
    defensePercentBonus=dp;
}
unitPriceMod::unitPriceMod(short fw, short f, short w, short g, short s, short t)
{
    forWhat=fw;
    prices[0]=f;
    prices[1]=w;
    prices[2]=g;
    prices[3]=s;
    time=t;
}
CurrentlyResearching::CurrentlyResearching(short tl, short what, short where)
{
    timeleft=tl;
    researchingWhat=what;
    researchingWhere=where;
}