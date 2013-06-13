#include "tile.h"
tile::tile(short iunit, short punit, short ibuilding, short pbuilding, int ts, int lt, int f, int w, int g, int s, /*float c,*/ bool uo, short ibr, unsigned char wpf, unsigned char wpt, char elev)
{
	unitindex=iunit;
	unitplayer=punit;
	buildingindex=ibuilding;
	buildingplayer=pbuilding;
	tilestyle=ts;
	locoftunnel=lt;
	resources[0]=f;
	resources[1]=w;
	resources[2]=g;
	resources[3]=s;
	//camouflage=c;
	uniton=uo;
	inbuildingrad=ibr;
	whichplayer.set(false, wpf);
	whichplayer.set(true, wpt);
	elevation=elev;
}