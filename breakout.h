/**
*
*   WinBreakoutC++
*
*   breakout.h
*
*   2015 a collaboration of CS50x students
*
*/

#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);


HWND createMainWin(HINSTANCE hThisInstance, int sizeX, int sizeY);
