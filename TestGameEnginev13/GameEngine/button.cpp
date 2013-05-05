#include "stdafx.h"
#include "button.h"
button::button(int px, int py, int w, int h, string txt, void (*f)(buttonparam, HWND), unsigned long long dw, int uorb)
{
	x=px;
	y=py;
	width=w;
	height=h;
	text=txt;
	func=*f;
	dispwhen=dw;
	unitorbuilding=uorb;
}