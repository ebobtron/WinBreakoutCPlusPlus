/**
*
*   WinBreakoutC++
*
*   breakout.h
*
*   2015 a collaboration of CS50x students
*
*/

#include "breakout.h"

/**
*   make the main window
***********************************************/
HWND createMainWin(HINSTANCE hThisInstance, int sizeX, int sizeY){

    /**  make a class name. Why? Because.  */
    TCHAR szClassName[ ] = _T("Student Project");
    HWND hwnd;   /* the main window handle*/
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */

    /* add a custom background color */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));  // ebt

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T(" WinBreakoutC++ "),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           sizeX,                 /* The programs width */
           sizeY,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    if(hwnd)
        return hwnd;

    return 0;
}

/*   paint text in window    */
void paintText(HWND hwnd, LPCTSTR lpText){

    /* to paint one must have the PAINTSTRUCT   */
    PAINTSTRUCT ps;
    /* get handle to device context */
    HDC hdc = BeginPaint(hwnd, &ps);
    /* write character string to location x, y   */
    TextOut(hdc, 80, 10, lpText, lstrlen(lpText));
    /* release the device context */
    EndPaint(hwnd, &ps);
}


/*    refresh the window     */
void refreshWindow(HWND hwnd, LPCTSTR lpOptionalText){

    /* we need to deal with optional text */
    LPCTSTR lpText[64];
    lstrcpy((LPSTR)lpText, _T(" main window "));

    if(lpOptionalText)
        lstrcpy((LPSTR)lpText, lpOptionalText);

    /* paint the text to the window*/
    paintText(hwnd, (LPCTSTR)lpText);

    /* some sample ball code at least it's a circle  */
    HDC hdc = GetDC(hwnd);
    Ellipse(hdc,
            50,    // int nLeftRect
            50,    // int nTopRect
            100,   // int nRightRect
            100);  // int nBottomRect
    DeleteDC(hdc);
}
