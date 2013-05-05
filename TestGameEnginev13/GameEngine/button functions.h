#ifndef button_functions_h
#define buttonfunctions_h

//! \file "button functions.h" This contains functions for the function pointer in the button class

void makereg(buttonparam b, HWND hWnd);
void setstance(buttonparam b, HWND hWnd);
void designate(buttonparam b, HWND hWnd);
void build(buttonparam b, HWND hWnd);
void createnewunit(buttonparam b, HWND hWnd);
void createunit(buttonparam b, HWND hWnd);
void selectgarrison(buttonparam b, HWND hWnd);
void ungarrisonall(buttonparam b, HWND hWnd);
void ungarrison(buttonparam b, HWND hWnd=NULL);
void leaveship(buttonparam b, HWND hWnd);
void beginrecordreport(buttonparam b, HWND hWnd);
void givereport(buttonparam b, HWND hWnd);
void nextpage(buttonparam b, HWND hWnd);

#endif