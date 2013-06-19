#ifndef GUIANDINPUT_H
#define	GUIANDINPUT_H

void processPassiveMouseMove(int x, int y);
void processMouseMove(int x, int y);
void dblClickTimer(int arg);
void processMouse(int button, int state, int x, int y);
void changeSize(int w, int h);
void renderScene();
void makeRect(float x, float y, float width, float height, RGB color);
void makeEllipse(float x, float y, float radiusx, float radiusy, RGB color);
void makeRect(float x, float y, float width, float height, ARGB color);
void makeEllipse(float x, float y, float radiusx, float radiusy, ARGB color);
void drawLine(float x, float y, float fx, float fy, RGB color);
void drawEmptyRect(float x, float y, float width, float height, RGB color);
void renderBitmapString(float x,float y,float z,void *font,char *string);
void renderBitmapString(float x,float y,float z,void *font,char *string,RGB color);
void processKeys(unsigned char key, int x, int y);
void renderBitmapString(float x,float y,float z,void *font,char *string, int winwidth, int winheight);
void makeRect(float x, float y, float width, float height, RGB color, int winwidth, int winheight);
void drawEmptyRect(float x, float y, float width, float height, RGB color, int winwidth, int winheight);
void reportDialogMouse(int button, int state, int x, int y);

#endif	/* GUIANDINPUT_H */

