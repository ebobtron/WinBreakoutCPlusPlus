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

/**   declare timer ID   **/
const int ID_TIMER = 1;

/**   global rectangle structures               **/
/**   for our window, ball, bricks and paddle   **/
extern RECT mwinRect;
extern RECT ballRect;
extern RECT ballRectInvaild;
extern RECT textRect;
extern RECT paddleRectInvaild;

extern POINTS mpoint_x;


/**    windows procedure   **/
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/**   createMainWin() returns a window handle
      winHandle = createMainWin(thisInstance, width, height)   **/
HWND createMainWin(HINSTANCE, int, int);

/**   paintText()  returns void
      paintText(window handle, text to paint)    **/
void paintText(HDC, LPCTSTR);

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

/**   structure for ball information size, position, speed   **/
struct theBALL{
  int sizeDia;   // size diameter
  int x;
  int y;
  int vx;
  int vy;
};





