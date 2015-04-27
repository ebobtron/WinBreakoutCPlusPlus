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

// #define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "resource.h"
#include "faiza.h"



/**   declare timer ID   **/
#define ID_TIMER 1

/**   global rectangle structures               **/
/**   for our window, ball, bricks and paddle   **/
extern RECT mwinRect;

/**   global POINTS structure to tack mouse position   **/
extern POINTS mpoint_x;


/**    windows procedure   **/
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/**   createMainWin() returns a window handle
      winHandle = createMainWin(thisInstance, width, height)   **/
HWND createMainWin(HINSTANCE, int, int);

/**   drawText()  returns void
      drawText(window handle, text to paint)   **/
void drawText(HDC, LPCTSTR, COLORREF color = NULL);

/**   drawBall() returns void   **/
void drawBall(HDC hdc, COLORREF pen = NULL, COLORREF color = NULL);

/**   drawPaddle() returns void   **/
void drawPaddle(HDC hdc, COLORREF pen = NULL, COLORREF fill = NULL);

/**   refreshWindow() returns void
      refreshWindow(main window handle, optional text or NULL  **/
void refreshWindow(HWND hwnd, LPCTSTR lpOptionalText = NULL);

/**   createBall() returns void
      createBall(ball size, position x, position y)   **/
void createBall(int, int, int);

/**   updateGame() returns void
      updateGame(main window handle)  **/
void updateGame(HWND);

/**   detectCollision() returns void
      detectCollisions(main window handle)   **/
void detectCollisions(HWND);

/**   makePaddle() returns void
      makePaddle(position y, length, height)   **/
void createPaddle(int, int, int);

/**   paddle tracks mouse using POINTS structure   **/
void updatePaddle();

/**   add rectangles to the window's update region   **/
void setUpdateRegion(HWND);

/****/
void cleanUp();

/**   structure for ball information size, position, speed   **/
struct theBALL{
  int sizeDia;   // size diameter
  int x;
  int y;
  int vx;
  int vy;
};


/**   define some colors to use in the game   **/
#define bO_RED RGB(255,0,0)
#define bO_BLUE RGB(0,0,255)
#define bO_GREEN RGB(0,255,0)
#define bO_WHITE RGB(255,255,255)
#define bO_BLACK RGB(1,1,1)    // because zero is zero




