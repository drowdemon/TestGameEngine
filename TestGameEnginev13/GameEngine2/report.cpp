#include "report.h"
#include "unit.h"
#include "globals.h"
#include "GUIAndInput.h"

#include <GL/glut.h>
using namespace std;

//!	\file report.cpp This contains class report

report::report()
{
}
void report::updateseenunits(vector<pointex> &seenunits)
{
	for(unsigned int i=0; i<seenunits.size(); i++)
	{
		bool good=true;
		for(unsigned int j=0; j<enemyseenunits.size(); j++)
		{
			if(seenunits[i].player==enemyseenunits[j].enemyplayer && seenunits[i].index==enemyseenunits[j].enemyindex)
			{
				good=false;
				break;
			}
		}
		if(good)
		{
			enemyseenunits.push_back(reportsimpleunitinfo(allunits[seenunits[i].player][seenunits[i].index]->id, point((float)seenunits[i].x,(float)seenunits[i].y), seenunits[i].player,seenunits[i].index));
			enemylivedids.push_back(enemyseenunits.size()-1);
		}
	}
}
void report::updateseenunits(vector<reportsimpleunitinfo> &seenunits)
{
	for(unsigned int i=0; i<seenunits.size(); i++)
	{
		bool good=true;
		for(unsigned int j=0; j<enemyseenunits.size(); j++)
		{
			if(seenunits[i].enemyplayer==enemyseenunits[j].enemyplayer && seenunits[i].enemyindex==enemyseenunits[j].enemyindex)
			{
				good=false;
				break;
			}
		}
		if(good)
		{
			enemyseenunits.push_back(seenunits[i]);
			enemylivedids.push_back(enemyseenunits.size()-1);
		}
	}
}
void report::updatedkilledunits(short player, short index)
{
	for(unsigned int i=0; i<enemylivedids.size(); i++)
	{
		if(enemyseenunits[enemylivedids[i]].enemyindex==index && enemyseenunits[enemylivedids[i]].enemyplayer==player)
		{
			enemykilledids.push_back(enemylivedids[i]);
			enemylivedids.erase(enemylivedids.begin()+i);
			break;
		}
	}
}
void report::give(short player)
{
    for(unsigned int i=0; i<seenTiles.size(); i++)
        minimapseen[player][(unsigned int)seenTiles[i].y][(unsigned int)seenTiles[i].x]=true;
	//reportdialoghwnd=CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_REPORTBOX), hWnd, reportdialogproc);
    makeReportDialog=true;
	reportDialogWindow=glutCreateSubWindow(mainWindow, 30, 30, 330, 400);
    glutMouseFunc(reportDialogMouse);
	glutSetWindow(reportDialogWindow);
    glutDisplayFunc(renderReportDialog);
    glutPostRedisplay();
    glutTimerFunc(100,dialogProc,0);
    glutSetWindow(mainWindow);
	currRep=this;
	reportfuncs[0]=&report::gentxtMyKilled;
	reportfuncs[1]=&report::gentxtEnemyKilled;
	reportfuncs[2]=&report::gentxtSeen;
	//ShowWindow(reportdialoghwnd,SW_SHOW);
}
char* report::gentxtMyKilled()
{
	string text;
	if(unitslost.size()==0)
		text.append("You haven't lost anyone!");
	else
		text.append("You have lost:\n");
	for(unsigned int i=0; i<unitslost.size(); i++)
	{
		text.append(" A level ");
		text.append(inttos(unitslost[i].vetlvl));
		text.append(" ");
		text.append(allbuildableunits[unitslost[i].id].name);
		if(unitslost[i].lieut==false)
			text.append(".\n");
		else
		{
			text.append(". He was a level ");
			text.append(inttos(unitslost[i].lieutlvl));
			text.append(" lieutenant.\n");
		}
	}
	char *ret=new char[text.size()+1];
	for(unsigned int i=0; i<text.size(); i++)
	{
		ret[i]=text[i];
	}
	ret[text.size()]=0;
	return ret;
}
char* report::gentxtSeen()
{
	string text;
	if(enemylivedids.size()==0)
		text.append("You haven't seen any enemies and let them live!");
	else
		text.append("You have seen:\n");
	for(unsigned int i=0; i<enemylivedids.size(); i++)
	{
		text.append("Player ");
		text.append(inttos(enemyseenunits[enemylivedids[i]].enemyplayer));
		text.append("'s ");
		text.append(allbuildableunits[enemyseenunits[enemylivedids[i]].enemyseenid].name);
		text.append(" on (");
		text.append(inttos((int)enemyseenunits[enemylivedids[i]].enemyseenloc.x));
		text.append(",");
		text.append(inttos((int)enemyseenunits[enemylivedids[i]].enemyseenloc.y));
		text.append(").\n");
	}
	char *ret=new char[text.size()+1];
	for(unsigned int i=0; i<text.size(); i++)
	{
		ret[i]=text[i];
	}
	ret[text.size()]=0;
	return ret;
}
char* report::gentxtEnemyKilled()
{
	string text;
	if(enemykilledids.size()==0)
		text.append("You haven't killed any enemies");
	else
		text.append("You have killed:\n");
	for(unsigned int i=0; i<enemykilledids.size(); i++)
	{
		text.append("Player ");
		text.append(inttos(enemyseenunits[enemykilledids[i]].enemyplayer));
		text.append("'s ");
		text.append(allbuildableunits[enemyseenunits[enemykilledids[i]].enemyseenid].name);
		text.append(" on (");
		text.append(inttos((int)enemyseenunits[enemykilledids[i]].enemyseenloc.x));
		text.append(",");
		text.append(inttos((int)enemyseenunits[enemykilledids[i]].enemyseenloc.y));
		text.append(").\n");
	}
	char *ret=new char[text.size()+1];
	for(unsigned int i=0; i<text.size(); i++)
	{
		ret[i]=text[i];
	}
	ret[text.size()]=0;
	return ret;
}
void report::merge(report r)
{
    updateseenunits(r.enemyseenunits);
    vector<short> temp;
    for(unsigned int i=0; i<r.enemykilledids.size(); i++)
    {
        bool good=true;
        for(unsigned int j=0; j<enemykilledids.size(); j++)
        {
            if(r.enemykilledids[i]==enemykilledids[j])
            {
                good=false;
                break;
            }
        }
        if(good)
        {
            temp.push_back(r.enemykilledids[i]);
        }
    }
    for(unsigned int i=0; i<temp.size(); i++)
        updatedkilledunits(r.enemyseenunits[temp[i]].enemyplayer,r.enemyseenunits[temp[i]].enemyindex);
    
    temp.clear();
    for(unsigned int i=0; i<r.unitslost.size(); i++)
    {
        bool good=true;
        for(unsigned int j=0; j<unitslost.size(); j++)
        {
            if(r.unitslost[i].id==unitslost[j].id)
            {
                good=false;
                break;
            }
        }
        if(good)
        {
            temp.push_back(i);
        }
    }
    for(unsigned int i=0; i<temp.size(); i++)
        unitslost.push_back(r.unitslost[temp[i]]);
    
    temp.clear();
    for(unsigned int i=0; i<r.seenTiles.size(); i++)
    {
        bool good=true;
        for(unsigned int j=0; j<seenTiles.size(); j++)
        {
            if((int)r.seenTiles[i].x==(int)seenTiles[j].x && (int)r.seenTiles[i].y==(int)seenTiles[j].y)
            {
                good=false;
                break;
            }
        }
        if(good)
        {
            temp.push_back(i);
        }
    }
    for(unsigned int i=0; i<temp.size(); i++)
        seenTiles.push_back(r.seenTiles[temp[i]]);
}