#include <GL/glut.h>
#include "tile.h"
#include "globals.h"
#include "dataStructures.h"
#include "buttonFunctions.h"
#define DBL_CLICK 3

int dblclickglut=-1;

void processPassiveMouseMove(int x, int y)
{
    scrollDir=0;
    if(x<=20 && topleft.x>0)
        scrollDir|=LEFT;
    else if(x>=(WIDTH-20) && topleft.x<((map[0].size()*15)-WIDTH)/15)
        scrollDir|=RIGHT;
    if(y<=40 && topleft.y>0)
        scrollDir|=DOWN;
    else if(y>=(HEIGHT-140) && y<=HEIGHT-100 && topleft.y<((map.size()*15)-HEIGHT)/15)
        scrollDir|=UP;
	/*if(x<=20 && topleft.x>0)
		topleft.x-=1;
	else if(x>=(WIDTH-20) && topleft.x<((map[0].size()*15)-WIDTH)/15)
		topleft.x+=1;
	if(y<=40 && topleft.y>0)
		topleft.y-=.5;
	else if(y>=(HEIGHT-140) && y<=HEIGHT-100 && topleft.y<((map.size()*15)-HEIGHT)/15)
		topleft.y+=.5;//end scroll*/
	currmousex2=x;
	currmousey2=y;
	mousex=x;
	mousey=y;
}
void processMouseMove(int x, int y)
{
    scrollDir=0;
    if(x<=20 && topleft.x>0)
        scrollDir|=LEFT;
    else if(x>=(WIDTH-20) && topleft.x<((map[0].size()*15)-WIDTH)/15)
        scrollDir|=RIGHT;
    if(y<=40 && topleft.y>0)
        scrollDir|=DOWN;
    else if(y>=(HEIGHT-140) && y<=HEIGHT-100 && topleft.y<((map.size()*15)-HEIGHT)/15)
        scrollDir|=UP;
	if(mousex2!=x && mousey2!=y && lbuttondown)
	{
		uptomousex2=(float)x;
		uptomousey2=(float)y;
		uptomousex2+=(topleft.x*15);
		uptomousey2+=(topleft.y*15);
	}
	mousex=x;
	mousey=y;
}
void dblClickTimer(int arg)
{
	dblclickglut=-1;
}
void processMouse(int button, int state, int x, int y)
{
	if(button==GLUT_LEFT_BUTTON)
	{
		if(state==GLUT_DOWN)
		{
			if(dblclickglut==-1)
			{
				glutTimerFunc(250,dblClickTimer,0);
				dblclickglut=0;
			}
			else if(dblclickglut==0)
				state=DBL_CLICK;
		}
		if(state==GLUT_DOWN)
		{
			//printf("L button down\n");
			//mousex=(float)GET_X_LPARAM(lParam);
			//mousey=(float)GET_Y_LPARAM(lParam);
			mousex2=x;
			mousey2=y;
			mousex2+=(topleft.x*15);
			mousey2+=(topleft.y*15);
			if(buildinghover==true)
			{
				build(0,buildingwhat,(int)mousex2/15,(int)mousey2/15);
				buildinghover=false;
				buildingwhat=-1;
				buildingwidth=0;
				buildingheight=0;
				//return; //Fixed bug where units would be selected if building placed in invalid square
			}
			lbuttondown=true;
			uptomousex2=mousex2;
			uptomousey2=mousey2;
		}
		else if(state==GLUT_UP)
		{
			lbuttondown=false;
			if(dblclick==true)
			{
				dblclick=false; //makes sure this isn't called after a double click. It will be:  left down, left up, left down, double click, left up (ignored due to this) end
				return;
			}
			if(transferResourcesPressed==1)
			{
				transferResourcesPressed=0;
				transferResourcesTyped.clear();
			}
			else if(transferResourcesPressed>1)
			{
				if(mousex2==uptomousex2 && mousey2==uptomousey2) //A click not a box
				{
					point p(mousex2/15,mousey2/15);
					if((map[(int)p.y][(int)p.x].tilestyle==TS_BUILDING || map[(int)p.y][(int)p.x].tilestyle==TS_WATERBUILDING) && map[(int)p.y][(int)p.x].buildingindex!=-1 && map[(int)p.y][(int)p.x].buildingplayer==0)
					{
						if(transferResourcesPressed==2)
						{
							transferBuilding1=map[(int)p.y][(int)p.x].buildingindex;
							transferResourcesPressed=3;
							transferResourcesTyped="Now click on the building to transfer to";
						}
						else if(transferResourcesPressed==3)
						{
							transferResources(0, transferInput, transferBuilding1, map[(int)p.y][(int)p.x].buildingindex);
							transferResourcesTyped="";
							transferResourcesPressed=0;
						}
						redraw=1;
					}
				}
				return; //Processed - Done!
			}
			//printf("L button up\n");
			//RECT client;
			//GetClientRect(hWnd, &client);
			//int currx=GET_X_LPARAM(lParam);
			//int curry=GET_Y_LPARAM(lParam);
			if(y>=HEIGHT-100)//buttons
			{
				for(unsigned int i=0; i<allbuttons.size(); i++)
				{
					if(checkdisp(0,allbuttons[i].dispwhen))
					{
						if(x>allbuttons[i].x && x<allbuttons[i].x+allbuttons[i].width && y>allbuttons[i].y && y<allbuttons[i].y+allbuttons[i].height)
						{
							allbuttons[i].func(buttonparam(0,i,allbuttons[i].unitorbuilding)); //player 0, index i
						}
					}
				}
				return; //not selecting anything in the button area, so finished
			}
			else if(mousex2==uptomousex2 && mousey2==uptomousey2)
			{
				point p(mousex2/15,mousey2/15);
				selectone(0, p);//point(mousex/15, mousey/15));
			}
			else
			{
				myrect clicked;
				if(uptomousex2-mousex2>0 && uptomousey2-mousey2>0)
				{
					clicked.left=mousex2;
					clicked.top=mousey2;
					//clicked.right=uptomousex-mousex;
					//clicked.bottom=uptomousey-mousey;
					clicked.right=uptomousex2;
					clicked.bottom=uptomousey2;
				}
				else if(uptomousex2-mousex2<0 && uptomousey2-mousey2>0)
				{
					clicked.left=uptomousex2;
					clicked.top=mousey2;
					//clicked.right=mousex-uptomousex;
					//clicked.bottom=uptomousey-mousey;
					clicked.right=mousex2;
					clicked.bottom=uptomousey2;
				}
				else if(uptomousex2-mousex2>0 && uptomousey2-mousey2<0)
				{
					clicked.left=mousex2;
					clicked.top=uptomousey2;
					clicked.right=uptomousex2;
					clicked.bottom=mousey2;
					//clicked.right=uptomousex-mousex;
					//clicked.bottom=mousey-uptomousey;
				}
				else if(uptomousex2-mousex2<0 && uptomousey2-mousey2<0)
				{
					clicked.left=uptomousex2;
					clicked.top=uptomousey2;
					//clicked.right=mousex-uptomousex;
					//clicked.bottom=mousey-uptomousey;
					clicked.right=mousex2;
					clicked.bottom=mousey2;
				}
				clicked.top/=15;
				clicked.bottom/=15;
				clicked.left/=15;
				clicked.right/=15;
				selectmany(currplayer,clicked);
			}
			return;
		}
		else if(state==DBL_CLICK)
		{
			//printf("DBL click\n");
			if(transferResourcesPressed!=0)
			{
				transferResourcesPressed=0;
				transferResourcesTyped.clear();
			}
			dblclick=true;
			dblclickglut=-1;
			//float mx=x;
			//float my=y;
			int reg=-1;
			if(map[(int)(mousey2/15)][(int)(mousex2/15)].uniton==true)
				reg=allunits[0][map[(int)(mousey2/15)][(int)(mousex2/15)].unitindex]->regimentid;
			if(reg!=-1)
			{
				for(unsigned int i=0; i<selectedunits[0].size(); i++)
					allunits[0][i]->selected=false;
				selectedunits[0].clear();
				for(unsigned int i=0; i<allregiments[0][reg].unitids.size(); i++)
				{
					allunits[0][allregiments[0][reg].unitids[i]]->selected=true;
					selectedunits[0].push_back(allregiments[0][reg].unitids[i]);
				}
			}
			return;
		}
	}
	else if(button==GLUT_RIGHT_BUTTON)
	{
		if(state==GLUT_DOWN)
		{
			POINT p;
			p.x=x;
			p.y=y;
			if(p.y>HEIGHT-100)
				return; //clicked in the 'button' area
			p.x+=(topleft.x*15);
			p.y+=(topleft.y*15);
			p.x/=15;
			p.y/=15;
			if((glutGetModifiers()&GLUT_ACTIVE_SHIFT)>0) //shift pressed
				move(0,p,true);
			else
				move(currplayer, p);
			return;
		}
		else if(state==GLUT_UP)
		{
			POINT p;
			p.x=x;
			p.y=y;
			if(p.y>=HEIGHT-100)//buttons
			{
				for(int i=indexGarrisonbutton; i<indexGarrisonbuttonend; i++)
				{
					if(checkdisp(0,allbuttons[i].dispwhen))
					{
						if(p.x>allbuttons[i].x && p.x<allbuttons[i].x+allbuttons[i].width && p.y>allbuttons[i].y && p.y<allbuttons[i].y+allbuttons[i].height)
						{
							ungarrison(buttonparam(0,i,-1));
						}
					}
				}
			}
		}
	}
	else if(button==GLUT_MIDDLE_BUTTON)
	{
		point p(x/15.0f, y/15.0f);
		capture(currplayer, p);
	}
}
void changeSize(int w, int h)
{
	HEIGHT=glutGet(GLUT_WINDOW_HEIGHT);
	WIDTH=glutGet(GLUT_WINDOW_WIDTH);
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	/*if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);*/
}
void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
		// Set the camera
	gluLookAt(1.0f, 1.0f, 1.0f,
				  1.0f, 1.0f, 0.0f,
				  0.0f, 1.0f, 0.0f);

	glutSwapBuffers();
}
void makeRect(float x, float y, float width, float height, RGB color)
{
	glColor3f((float)color.r/255.0, (float)color.g/255.0, (float)color.b/255.0);
	glBegin(GL_QUADS);
		glVertex3f((float)x*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,0);
		glVertex3f((float)(x+width)*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,0);
		glVertex3f((float)(x+width)*2.0/WIDTH,2.0-(float)(y+height)*2.0/HEIGHT,0);
		glVertex3f((float)x*2.0/WIDTH,2.0-(float)(y+height)*2.0/HEIGHT,0);
	glEnd();
}
void makeEllipse(float x, float y, float radiusx, float radiusy, RGB color)
{
	glColor3f((float)color.r/255.0, (float)color.g/255.0, (float)color.b/255.0);
	glBegin(GL_POLYGON);
		for(double i = 0; i < 2 * PI; i += PI / 24) //The 6 is the smoothness, basically. Higher number makes it smoother.
			glVertex3f((float)(cos(i)*radiusx+x)*2.0/WIDTH, 2.0-(float)(sin(i)*radiusy+y)*2.0/HEIGHT, 0.0);
	glEnd();
}
void makeRect(float x, float y, float width, float height, ARGB color)
{
	glColor4f(color.r/255.0, color.g/255.0, color.b/255.0, color.a/255.0); //rgba
	glBegin(GL_QUADS);
		glVertex3f((float)x*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,0);
		glVertex3f((float)(x+width)*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,0);
		glVertex3f((float)(x+width)*2.0/WIDTH,2.0-(float)(y+height)*2.0/HEIGHT,0);
		glVertex3f((float)x*2.0/WIDTH,2.0-(float)(y+height)*2.0/HEIGHT,0);
	glEnd();
}
void makeEllipse(float x, float y, float radiusx, float radiusy, ARGB color)
{
	glColor4f((float)color.r/255.0, (float)color.g/255.0, (float)color.b/255.0, color.a/255.0);
	glBegin(GL_POLYGON);
		for(double i = 0; i < 2 * PI; i += PI / 24) //The 6 is the smoothness, basically. Higher number makes it smoother.
			glVertex3f((float)(cos(i)*radiusx+x)*2.0/WIDTH, 2.0-(float)(sin(i)*radiusy+y)*2.0/HEIGHT, 0.0);
	glEnd();
}
void makeRect(float x, float y, float width, float height, RGB color, int winwidth, int winheight)
{
	glColor3f((float)color.r/255.0, (float)color.g/255.0, (float)color.b/255.0);
	glBegin(GL_QUADS);
		glVertex3f((float)x*2.0/winwidth,2.0-(float)y*2.0/winheight,0);
		glVertex3f((float)(x+width)*2.0/winwidth,2.0-(float)y*2.0/winheight,0);
		glVertex3f((float)(x+width)*2.0/winwidth,2.0-(float)(y+height)*2.0/winheight,0);
		glVertex3f((float)x*2.0/winwidth,2.0-(float)(y+height)*2.0/winheight,0);
	glEnd();
}
void drawLine(float x, float y, float fx, float fy, RGB color)
{
	glColor3f((float)color.r/255.0, (float)color.g/255.0, (float)color.b/255.0);
	glBegin(GL_LINES);
		glVertex3f((float)x*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,0);
		glVertex3f((float)fx*2.0/WIDTH,2.0-(float)fy*2.0/HEIGHT,0);
	glEnd();
}
void drawEmptyRect(float x, float y, float width, float height, RGB color)
{
	glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f((float)x*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,0);
		glVertex3f((float)(x+width)*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,0);
		glVertex3f((float)(x+width)*2.0/WIDTH,2.0-(float)(y+height)*2.0/HEIGHT,0);
		glVertex3f((float)x*2.0/WIDTH,2.0-(float)(y+height)*2.0/HEIGHT,0);
	glEnd();
}
void drawEmptyRect(float x, float y, float width, float height, RGB color, int winwidth, int winheight)
{
	glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f((float)x*2.0/winwidth,2.0-(float)y*2.0/winheight,0);
		glVertex3f((float)(x+width)*2.0/winwidth,2.0-(float)y*2.0/winheight,0);
		glVertex3f((float)(x+width)*2.0/winwidth,2.0-(float)(y+height)*2.0/winheight,0);
		glVertex3f((float)x*2.0/winwidth,2.0-(float)(y+height)*2.0/winheight,0);
	glEnd();
}

void renderBitmapString(float x,float y,float z,void *font,char *string)
{
	char *c;
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0,0.0,0.0);
	glRasterPos3f((float)x*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,(float)z);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
	glEnable(GL_TEXTURE_2D);
}
void renderBitmapString(float x,float y,float z,void *font,char *string,RGB color)
{
	char *c;
	glDisable(GL_TEXTURE_2D);
	glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
	glRasterPos3f((float)x*2.0/WIDTH,2.0-(float)y*2.0/HEIGHT,(float)z);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
	glEnable(GL_TEXTURE_2D);
}
void renderBitmapString(float x,float y,float z,void *font,char *string, int winwidth, int winheight)
{
	char *c;
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0,0.0,0.0);
	glRasterPos3f((float)x*2.0/winwidth,2.0-(float)y*2.0/winheight,(float)z);
    int numnew=0;
	for (c=string; *c != '\0'; c++)
	{
        if(*c=='\n')
        {
            numnew++;
            glRasterPos3f((float)x*2.0/winwidth,2.0-(float)(y+numnew*20)*2/winheight,(float)z);
        }
		glutBitmapCharacter(font, *c);
	}
	glEnable(GL_TEXTURE_2D);
}
void processKeys(unsigned char key, int x, int y)
{
	if(transferResourcesPressed>0)
	{
		if(key==8 || key==127) //delete or backspace
		{	
			if(transferResourcesTyped.length()!=0)
				transferResourcesTyped.erase(transferResourcesTyped.end()-1);
		}
		else if(key==27) //escape
		{
			transferResourcesTyped.clear();
			transferResourcesPressed=0;
		}
		else if(key==10 || key==13) //enter?
		{
			transferInput=transferResourcesTyped;
			transferResourcesTyped="Now, click on the building to transfer from";
			transferResourcesPressed=2;
		}
		else
			transferResourcesTyped+=key;
		redraw=1;
	}
}
void renderReportDialog()
{ //TODO Make this change the text when you click a 'button'
    glutSetWindow(reportDialogWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
		// Set the camera
	gluLookAt(1.0f, 1.0f, 1.0f,
			  1.0f, 1.0f, 0.0f,
			  0.0f, 1.0f, 0.0f);

	RGB black(0,0,0);
	RGB blue(0,0,255);
    RGB grey(200,200,200);
	RGB currColor=black;
    RGB reddish(205,65,23);
	if(currReportTab==0)
		currColor=blue;
	else
		currColor=black;
    int dwidth=glutGet(GLUT_WINDOW_WIDTH);
	int dheight=glutGet(GLUT_WINDOW_HEIGHT);
    makeRect(0,0,dwidth,dheight,grey,dwidth,dheight);
	drawEmptyRect(15,20,70,18,currColor,dwidth,dheight); //buttons
	renderBitmapString(17,32,0,GLUT_BITMAP_HELVETICA_12,(char *)"Units Lost\0", dwidth, dheight);
	if(currReportTab==1)
		currColor=blue;
	else
		currColor=black;
	drawEmptyRect(88,20,87,18,currColor,dwidth,dheight);
	renderBitmapString(90,32,0,GLUT_BITMAP_HELVETICA_12,(char *)"Enemies Killed\0",dwidth, dheight);
	if(currReportTab==2)
		currColor=blue;
	else
		currColor=black;
	drawEmptyRect(178,20,142,18,currColor,dwidth,dheight);
	renderBitmapString(180,32,0,GLUT_BITMAP_HELVETICA_12,(char *)"Surviving Enemies Seen\0", dwidth, dheight);

	renderBitmapString(30,52,0,GLUT_BITMAP_HELVETICA_12,(currRep->*reportfuncs[currReportTab])(), dwidth, dheight);
    
    drawEmptyRect(280,350,40,18,black,dwidth,dheight);
    renderBitmapString(290,362,0,GLUT_BITMAP_HELVETICA_12,(char *)"OK\0", dwidth, dheight);
    
    makeRect(3,3,13,13,reddish,dwidth,dheight); //The 'x' box
    glColor3f(.8,.8,.8); //greyish
    glBegin(GL_LINES);
		glVertex3f((float)6*2.0/dwidth,2.0-(float)6*2.0/dheight,0);
		glVertex3f((float)(13)*2.0/dwidth,2.0-(float)13*2.0/dheight,0);
    glEnd();
    glBegin(GL_LINES);
		glVertex3f((float)6*2.0/dwidth,2.0-(float)13*2.0/dheight,0);
		glVertex3f((float)(13)*2.0/dwidth,2.0-(float)6*2.0/dheight,0);
    glEnd();
	glutSwapBuffers();
    glutSetWindow(mainWindow);
}
void reportDialogMouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON)
    {
        if(state==GLUT_UP)
        {
            if(x>=15 && x<=85 && y>=20 && y<=38) //hit first tab-button
                currReportTab=0;
            else if(x>=88 && x<=173 && y>=20 && y<=38) //second
                currReportTab=1;
            else if(x>=178 && x<=320 && y>=20 && y<=38) //third
                currReportTab=2;
            else if(x>=280 && x<=320 && y>=350 && y<=368) //ok button
            {
                makeReportDialog=false;
                redraw=2;
            }
            else if(x>=3 && x<=13 && y>=3 && y<=13) //'x' button
            {
                makeReportDialog=false;
                redraw=2;
            }
        }
    }
}