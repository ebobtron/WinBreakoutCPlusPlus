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


/*  Declare Windows procedure         */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Declare createMainWin() returns a window handle
    winHandle = createMainWin(thisInstance, width, height)     */
HWND createMainWin(HINSTANCE, int, int);

/*  Declare paintText()  returns void
    paintText(window handle, text to paint)          */
void paintText(HWND, LPCTSTR);

/*  Declare refreshWindow() returns void
    refreshWindow(main window handle, optional text or NULL  */
void refreshWindow(HWND hwnd, LPCTSTR lpOptionalText = NULL);
