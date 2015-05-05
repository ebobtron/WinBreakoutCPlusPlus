/**
*
*   WinBreakoutC++
*
*   breakout.cpp
*
*   2015 a collaboration of CS50x students
*
*/

/**   include our programs declarations   **/
#include "breakout.h"
#include <iostream>


/**   define some global object variables   **/
tHEBALL ballInfo;    // persistent ball information

RECT mwinRect;    // main window rectangle
RECT ballRect;    // ball rectangle
/**   making paddleRect a pointer to heap memory     **/
/**   is pointless but demonstrative                 **/
/**   we get to use pointy -> syntax and we will     **/
/**   need to free this memory                       **/
RECT* paddleRect = (RECT*)malloc(sizeof(RECT));    // paddle rectangle
RECT textRect;    // text rectangle

/**   the region of the window to redraw is define in these   **/
RECT ballRectInvaild;    // ball rectangle for future and past positions
RECT paddleRectInvaild;    // rectangle needed for paddle redraw
RECT brickRectInvaild;    // the brick to invalidate

/**   for detecting collisions   **/
RECT intersect;    // see the function IntersectRect() in detectCollision

/**   the brink array   **/
tHEBRICK infobrick[BRICKS];    // number of bricks

/**   bricks remaining   **/
int bricksremaining = BRICKS;

/**   lives remaining   **/
int livesremaining = LIVES;

/**   we need to keep track of paddle length beyond it's creation   **/
/**   maybe a paddle info structure like ball, but for now       **/
int paddlelength = 0;

/**   a counting variable to count down ball release   **/
int delaycount = 0;

/**   POINTS structure to relay mouse data for the paddle   **/
/**   from the WindowProcedure WM_MOUSEMOVE event           **/
POINTS mpoint_x;

/**   game status structure   **/
struct {
  bool paused;
  bool ended;
  bool ball_lost;
  bool running;
} status;

/**
  *
  *                    FUNCTIONs
  *
  *************************************************/


/**   make the main window   **/
HWND createMainWin(HINSTANCE hThisInstance, int sizeX, int sizeY){

    /**   declare a class name. Why? Because.  **/
    TCHAR szClassName[ ] = _T("Student Project");

    /**   declare the main window handle   **/
    HWND hwnd = NULL;

    /**   declare data structure object variable for the window class   **/
    WNDCLASSEX wincl;

    /**   initialize the window structure   **/
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;   // This function is called by windows
    wincl.style = CS_DBLCLKS;     // Catch double-clicks
    wincl.cbSize = sizeof (WNDCLASSEX);

    /**  use default icon and mouse-pointer  **/
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);

    /**   no menu   **/
    wincl.lpszMenuName = NULL;

    /**   no extra bytes after the window class structure
          or the window instance                            */
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    /**   add a custom background color   */
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    /**   Register the window class, if it fails quit the program   */
    if(!RegisterClassEx(&wincl))
        return 0;

    /**   the class is registered,
          let's create the programs main window   */
    hwnd = CreateWindowEx(
           0,                  // extended possibilities for variation
           szClassName,            // Class name
           _T(" WinBreakoutC++ "),   // Title Text
           WS_OVERLAPPEDWINDOW,     // default window style
           CW_USEDEFAULT,       // windows decides the position
           CW_USEDEFAULT,       // where the window ends up on the screen
           sizeX,             // the window width
           sizeY,             // and height in pixels
           HWND_DESKTOP,      // the window is a child-window to desktop
           NULL,              // no menu
           hThisInstance,     // the program instance handle
           NULL               // no window creation data
           );

    /**   if window creation successful return the main window handle
          any work in the window will need this handle                  **/
    if(hwnd)
        return hwnd;

    /**   if we get this far we have failed to create
          a window for our program, returning 0 will
          cause a message to the user and then quit the process.   **/

    return 0;
}

/**   pay sound   **/
void playSound(int sound){

    switch(sound){
        case 1:
        // plays in separate thread
        PlaySound(MAKEINTRESOURCE(IDW_MYBALLSOUND),
                GetModuleHandle(NULL), SND_ASYNC|SND_RESOURCE);  // SND_ASYNC  sound plays with game
        break;
        case 2:
        PlaySound(MAKEINTRESOURCE(IDW_MYBUZSOUND),
                  GetModuleHandle(NULL), SND_RESOURCE);  // no SND_ASYNC sound pauses game.
        break;
        default:;
    }
}

/**   make the ball   **/
void createBall(int sizeDia, int x, int y){

    if(delayBall())
        return;

    /**   set up the ball rectangles from function argument list or not  **/
    /**   each item in argument list have a default value of NULL        **/
    /**   see prototype                                                  **/

    if(sizeDia)
        ballInfo.sizeDia = sizeDia;

    if(x)
        ballInfo.x = x - sizeDia / 2;

    if(y)
        ballInfo.y = y - sizeDia / 2;

    /**  initialize the ball rectangle  **/
    ballRect.left = ballInfo.x;
    ballRect.top = ballInfo.y;
    ballRect.right = ballInfo.x + ballInfo.sizeDia;
    ballRect.bottom = ballInfo.y + ballInfo.sizeDia;

    /**   seed random number generator   **/
    srand(GetTickCount());

    /**   provide random values to the ball velocity   **/
    /**   using Faiza Harbi's randVlo() function       **/
    /**   see faiza.cpp and faiza.h                    **/
    ballInfo.vx = randVlo('x');
    ballInfo.vy = randVlo('y');

    /**   set the drawing region of the window for the ball   */
    setBallTravelRegion();

    /**   update ball status if one has been lost   **/
    if(status.ball_lost)
        status.ball_lost = false;


}

/**   make the paddle   **/
void createPaddle(int posy, int length, int height){

    /**   preserve the length of the paddle   **/
    paddlelength = length;

    /**   load paddle rectangle  **/
    paddleRect->left = mwinRect.right / 2 - length / 2;
    paddleRect->top = posy;
    paddleRect->right = paddleRect->left + length;
    paddleRect->bottom = paddleRect->top + height;

    /**   load the paddle invalidate rectangle   **/
    paddleRectInvaild = *paddleRect;
    paddleRectInvaild.left = mwinRect.left;
    paddleRectInvaild.right = mwinRect.right;

}

/**   make a brick   **/
tHEBRICK createBrick(int x, int y, int length, int height,
                                   COLORREF pen, COLORREF brush){

    /**   define a brick structure variable object  */
    tHEBRICK abrick;

    /**   load the brick structure   */
    abrick.pen = pen;
    abrick.brush = brush;
    abrick.rcBrick.left = x;
    abrick.rcBrick.top = y;
    abrick.rcBrick.right= x + length;
    abrick.rcBrick.bottom = y + height;

    /**  return the brick  */
    return abrick;
}

/**  make the brick wall   **/
void createWall(int x){

    /**   make a list of game colors  */
    COLORREF color[] = {bO_BLUE, bO_ORANGE, bO_YELLOW, bO_RED, bO_GREEN};

    /**  math to spread out the brinks across the top portion of the window
         5 rows of 10 columns                                                 */
    int length = mwinRect.right / 10 -1;
    int offset_x = (mwinRect.right - length * 10) / 2;
    int offset_y = 40;
    int height = 12;

    int w_x = 0;  // define accumulation variables
    int w_y = 0;

    int row = -1;  // because zero is our first row

    /**   iterate over our wall of bricks setting values for each  */
    for(int i = 0; i < BRICKS; i++){

        if(i % 10 == 0){  // increment row each 10 bricks
            row++;
        }

        w_x = offset_x + (length * (i % 10)) ;
        w_y = offset_y + height * row;

        infobrick[i] = createBrick(w_x, w_y+2, length-2, height-2, 0, color[row]);
    }
}

/**   update paddle rectangle when ever mouse moves   **/
void updatePaddle(){

      /**   no paddle update if paused our event loop is stopped        */
      /**   we could make the paddle independent of the games normal   */
      /**   redraw code but seems like a little much at this point     */
      if(status.paused)
          return;

      /**   alter paddle position from mouse position   **/
      paddleRect->left = mpoint_x.x - paddlelength / 2;
      paddleRect->right = paddleRect->left + paddlelength;
}

/**   the redraw region of the traveling ball   **/
void setBallTravelRegion(void){

    /**   here we stretch out the invalidate region to redraw the past location     */
    /**   of the ball and the current position of the ball,  if the ball is moving  */
    if(ballInfo.vx > 0){
        ballRectInvaild.left = ballRect.left - ballInfo.vx;
        ballRectInvaild.right = ballRect.right + ballInfo.vx;
    }
    else{
        ballRectInvaild.left = ballRect.left + ballInfo.vx;
        ballRectInvaild.right = ballRect.right - ballInfo.vx;
    }

    if(ballInfo.vy > 0){
        ballRectInvaild.top = ballRect.top - ballInfo.vy;
        ballRectInvaild.bottom = ballRect.bottom + ballInfo.vy;
    }
    else{
        ballRectInvaild.top = ballRect.top + ballInfo.vy;
        ballRectInvaild.bottom = ballRect.bottom - ballInfo.vy;
    }
}


/**    update game data the whole game is a bunch      */
/**    of rectangles  manipulated here                 */
void updateGame(HWND hwnd){

    /**   delay ball launch for 5 seconds,  a magic counter  **/
    /**   during delay the delayBall returns true            **/
    if(delayBall())
        return;

   /**********************
    *                    *
    *    Move The Ball   *
    *                    *
    **********************/
    /**   MS's Offset Rectangle function make moving the rectangle easy   **/
    OffsetRect(&ballRect, ballInfo.vx, ballInfo.vy);

    /**   copy new ball rectangle   **/
    ballRectInvaild = ballRect;

   /*************************************************************************
    *   Adjust the RECT used to invalidate the part of the screen we need   *
    *   to redraw for the ball.  The region needed to be redrawn is where   *
    *   the ball is and where it is going to be on the next screen redraw   *
    *************************************************************************/
    setBallTravelRegion();

   /******************************
    *                            *
    *   Detect Ball Collisions   *
    *                            *
    ******************************/
    detectCollisions(hwnd);

    /**   stop game after screen refresh   **/
    if(!bricksremaining || !livesremaining)
        if(status.ended)
            KillTimer(hwnd, ID_TIMER);

}

/**   detect collisions of the ball                 */
/**   with window walls, paddle and bricks        **/
void detectCollisions(HWND hwnd){

    if(delayBall())
        return;

    /**   get main window rectangle   **/
    GetClientRect(hwnd, &mwinRect);

    /**   ball can collide in corner any one or   **/
    /**   two of the following are possible       **/

    /**   if collision left   **/
    if(ballRect.left <= mwinRect.left){
       ballInfo.vx = -ballInfo.vx;
       ballRect.left = mwinRect.left + 1;
       ballRect.right = ballInfo.sizeDia + 1;
       playSound(1);
    }

    /**   if collision top   **/
    if(ballRect.top <= mwinRect.top){
        ballInfo.vy = -ballInfo.vy;
        ballRect.top = mwinRect.top + 1;
        ballRect.bottom = ballInfo.sizeDia + 1;
        playSound(1);
    }

    /**   if collision right   **/
    if(ballRect.right >= mwinRect.right){
        ballInfo.vx = -ballInfo.vx;
        ballRect.left = mwinRect.right - ballInfo.sizeDia - 1;
        ballRect.right = mwinRect.right - 1;
        playSound(1);
    }

    /**   if collision bottom   **/
    if(ballRect.bottom >= mwinRect.bottom && !status.ball_lost){

        if(livesremaining)
            livesremaining--;

        if(livesremaining){
            SetRectEmpty(&ballRect);
            ballRect.left = 2;
            ballRect.right = 2;
            SetTimer(hwnd, ID_TIMER2, 1000, 0);
            status.ball_lost = true;
            delaycount = 5;
        }
    }

    /**   detect paddle collisions   **/
    //  TODO: use intersect rectangle to deal with buzz
    if(IntersectRect(&intersect, &ballRect, paddleRect)){
        ballInfo.vy = -ballInfo.vy;
        playSound(1);
    }

    /**   detect brick   **/
    for(int i = BRICKS; i >= 0; i--){

        if(IntersectRect(&intersect,&ballRect,&infobrick[i].rcBrick)){

            ballInfo.vy = -ballInfo.vy;
            brickRectInvaild = infobrick[i].rcBrick;
            playSound(1);
            bricksremaining--;
            SetRectEmpty(&infobrick[i].rcBrick);
            break;
        }
    }
}

/**    Ben's aka FreeER's detection code   *//*
bool detectCollision(rect2 a, rect2 b)
{
    // TODO: return info about which _side_ collided (enum?)
    if (a.rightbottom.x > b.lefttop.x && // a's right inside b
        a.lefttop.x < b.rightbottom.x)   // a's left inside b
    {
        if (a.rightbottom.y > b.lefttop.y && // a's bottom inside b?
            a.rightbottom.y < b.rightbottom.y)
            return true;
        else if (a.lefttop.y > b.lefttop.y &&
                 a.lefttop.y < b.rightbottom.y)
            return true;
        return false;
    }
    return false;
}
*/

/**   draw our ball  **/
void drawBall(HDC hdc, COLORREF pen, COLORREF brush){

    /**   using the device context we can change              **/
    /**   the color of pen and brush for the device context   **/
    /**   changing before drawing the ball causes the ball    **/
    /**   to change to red in this example                    **/

    /**   Note: if you don't select on object, PEN, BRUSH     **/
    /**         you can't change it                           **/

    /**  define variables to hold prior colors  */
    COLORREF lastpen = NULL;
    COLORREF lastbrush =  NULL;

    /**   if pen is not NULL change the color  */
    if(pen ){
        SelectObject(hdc, GetStockObject(DC_PEN));
        lastpen = SetDCPenColor(hdc, pen);
    }

    /**   if brush is not NULL change the color  */
    if(brush){
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        lastbrush = SetDCBrushColor(hdc, brush);
    }

    /**   draw the ellipse that is our ball    */
    Ellipse(hdc,
            ballRect.left,     // int nLeftRect
            ballRect.top,      // int nTopRect
            ballRect.right,    // int nRightRect
            ballRect.bottom);  // int nBottomRect

    /**   restore pen and or brush to previous color    */
    if(lastpen)
        SetDCPenColor(hdc, lastpen);

    if(lastbrush)
        SetDCBrushColor(hdc, lastbrush);
}

void drawPaddle(HDC hdc, COLORREF pen, COLORREF brush){

    /**  define variables to hold prior colors  */
    COLORREF lastpen = NULL;
    COLORREF lastbrush =  NULL;

    /**   if pen   */
    if(pen){
        SelectObject(hdc, GetStockObject(DC_PEN));
        lastpen = SetDCPenColor(hdc, pen);
    }
    /**   if brush  */
    if(brush){
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        lastbrush = SetDCBrushColor(hdc, brush);
    }
    /**   draw a rectangle from the paddle rectangle   */
    Rectangle(hdc,
            paddleRect->left,     // int nLeftRect
            paddleRect->top,      // int nTopRect
            paddleRect->right,    // int nRightRect
            paddleRect->bottom);  // int nBottomRect

    /**   restore the pen   */
    if(pen)
        SetDCPenColor(hdc, lastpen);

    /**   restore the brush   */
    if(brush)
        SetDCBrushColor(hdc, lastbrush);

}

/**   draw all or any one or more of our bricks    */
void drawWall(HDC hdc){

    /**   define variables to hold prior colors  */
    COLORREF lastpen = NULL;
    COLORREF lastbrush =  NULL;

    /**   add the pen and brush to the device context  */
    SelectObject(hdc, GetStockObject(DC_PEN));
    SelectObject(hdc, GetStockObject(DC_BRUSH));

    /**   iterate over our balls      */
    for(int i = 0; i < BRICKS; i++){

        /**   get brick outline(pen) from brick array  */
        lastpen = SetDCPenColor(hdc, infobrick[i].pen);

        /**   get brick fill color(brush) from brick array   */
        lastbrush = SetDCBrushColor(hdc, infobrick[i].brush);

        /**    draw a rectangle from the a brick rectangle    */
        Rectangle(hdc,
            infobrick[i].rcBrick.left,      // int nLeftRect
            infobrick[i].rcBrick.top,       // int nTopRect
            infobrick[i].rcBrick.right,     // int nRightRect
            infobrick[i].rcBrick.bottom);   // int nBottomRect

        /**   restore pen and brush    */
        SetDCPenColor(hdc, lastpen);
        SetDCBrushColor(hdc, lastbrush);
    }
}

/**   draw text in window   **/
void drawText(HDC hdc, LPCTSTR tsText, COLORREF color){

    /**   write character string to location x, y   **/

    /**   get the current default fonts handle   */
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

    /**   load default fonts attributes into a structure which defines    */
    /**   the attributes of a font                                        */
    LOGFONT logfont;
    GetObject(hFont, sizeof(LOGFONT), &logfont);

    /**   set the fonts new height  */
    logfont.lfHeight = 60;  // Now change the member

    /**   create a new font from the LOGFONT structure  */
    HFONT hNewFont = CreateFontIndirect(&logfont);

    /**   select our new font as the default font   */
    HFONT hOldFont = (HFONT)SelectObject(hdc, hNewFont);

    /**   width and height or text rectangle  */
    int width = 250;
    int height = 120;

    /**   like everything we draw to the screen     **/
    /**   create the text rectangle for DrawText()  **/
    /**   you can do this without the rectangle     **/
    /**   but only one line at a time               **/
    textRect.left = (mwinRect.right - width) / 2 ;    // center the text rectangle left and right
    textRect.top = LONG(mwinRect.bottom * .45);      // how far down the window the text starts
    textRect.right = textRect.left + width;
    textRect.bottom = textRect.top + height;

    /**   prepare for a custom color  **/
    COLORREF last = NULL;
    if(color)
        last = SetTextColor(hdc, color);

    /**   draw the text using rectangle   **/
    DrawTextEx(hdc, (LPSTR)tsText, _tcslen(tsText), &textRect, DT_CENTER, 0);

    /**   restore text color to previous color   **/
    if(color)
        SetTextColor(hdc, last);

    /**   restore previous font   **/
    SelectObject(hdc, hOldFont);

    /**   delete memory object    **/
    DeleteObject(hNewFont);

}

/**   add rectangles to our window's update region   **/
void setUpdateRegion(HWND hwnd){

    /**   tell windows which areas of window need redrawing   **/
    InvalidateRect(hwnd, &ballRectInvaild, true);
    InvalidateRect(hwnd, &textRect, true);
    InvalidateRect(hwnd, &paddleRectInvaild, true);
    InvalidateRect(hwnd, &brickRectInvaild, true);

}

/**   refresh the window, here we call all drawing functions   **/
void refreshWindow(HWND hwnd, LPCTSTR lpOptionalText){

    /**   we need to deal with optional text           **/
    /**   calling function can omit the text           **/
    /**   this is demonstrative at this point          **/

    // TODO: there is a wide character issue here, important ??
    /**   define a wide CHAR buffer for the Unicode crowd   **/
    /**   some call this a string                         **/
    TCHAR tcText[64];

    /**   define a wall of bricks  */
    int wall = BRICKS;

    /**   make main window score like the label in the CS50 version   */
    sprintf_s(tcText, sizeof tcText, "%i", wall - bricksremaining);

    /**   while delayBall show delay count   */
    if(delayBall()){
        sprintf_s(tcText,sizeof(tcText),"%i", delaycount);
    }

    /**   refreshWindow becomes our center for info update to the screen so
          makes since to track game status here                              **/

    /**   the game is won           */
    if(livesremaining && !bricksremaining){
        sprintf_s(tcText, sizeof tcText, "you win");
        status.ended = true;
        status.running = false;
    }

    /**   the game is lost           */
    if(!livesremaining && bricksremaining){
        sprintf_s(tcText,sizeof(tcText),"sorry\nyou lose");
        status.ended = true;
        status.running = false;
    }

    /**   track ball delay and starts next ball   */
    if(!delayBall() && status.ball_lost){
         KillTimer(hwnd, ID_TIMER2);
         if(status.ball_lost)
            createBall();
    }

    /**   a character buffer for our Window's Title Bar   */
    TCHAR titleText[48];

    /**   set title bar text a needed      */
    if(status.ended){
        sprintf_s(titleText, sizeof titleText,
                  "WinBreakoutC++   press \" s \" to start a game");
    }
    else{
        sprintf_s(titleText, sizeof titleText,
                  "WinBreakoutC++   %i Bricks  %i lives", bricksremaining ,
                         livesremaining);
    }

    /**   set the title bar text             */
    SetWindowText(hwnd, (LPCSTR)titleText);

    /**   if calling function sends text, use it   **/
    if(lpOptionalText)
        lstrcpy((LPSTR)tcText, lpOptionalText);


    /**   declare the paint structure object variable   **/
    /**   needed by the device context                  **/
    PAINTSTRUCT ps;

    /**   get the handle of the device context for our window needed   **/
    /**   by functions that paint or draw to the window                **/
    HDC hdc = BeginPaint(hwnd, &ps);

   /*******************
    *                 *
    *   DRAW PADDLE   *
    *                 *
    *******************/
    drawPaddle(hdc, bO_BLACK, bO_RED);   //&paddleRect,

   /**********************************
    *                                *
    *    DRAW TEXT TO THE WINDOW     *
    *                                *
    **********************************/
    drawText(hdc, (LPCTSTR)tcText, 0);    // color 0 default

   /*********************
    *                   *
    *   DRAW THE BALL   *
    *                   *
    *********************/
    drawBall(hdc, NULL, bO_RED);    // draw a black ball

   /*********************
    *   DRAW THE WALL   *
    *********************/
    drawWall(hdc);

    /**   much like free we use end paint to return        **/
    /**   the device context to the system.  if we         **/
    /**   don't release the DC the resource will be lost   **/
    /**   as a memory leak                                 **/
    EndPaint(hwnd, &ps);

    /**   play a starting sound / a natural game pause   **/
    if(!status.running && !status.ended){
        playSound(2);
        status.running = true;
    }
}

/**   handle character key presses like 'p' for pause and others   **/
void charKeyPress(HWND hwnd, WPARAM key){

    /**  when pause key pressed pause or unpause  **/
    if(key == pause && status.ended == false){
        // toggle the event pause
        if(status.paused){
            SetTimer(hwnd, ID_TIMER, 20, 0);
            status.paused = false;
        }
        else{
           KillTimer(hwnd, ID_TIMER);
           status.paused = true;
        }
    }

    /**  start again if we have ended a game  **/
    if(key == start && status.ended){
        bricksremaining = BRICKS;  // set bricks
        livesremaining = 3;
        createWall(0);  // make the wall again
        createBall(0, mwinRect.right / 2 - ballInfo.sizeDia / 2,
                      mwinRect.bottom  / 2 - ballInfo.sizeDia / 2);
        InvalidateRect(hwnd,NULL,true);  // force window redraw
        paddleRectInvaild.right = mwinRect.right; // adjust if resized window
        status.ended = false;  // set status
        SetTimer(hwnd, ID_TIMER, 20, 0);  // start the frame timer
    }

}

/**   then updates the our window rectangle & allows the redraw  **/
void winSizeChange(HWND hwnd){
    GetClientRect(hwnd, &mwinRect);
    InvalidateRect(hwnd,NULL,true);
}


/**   delay the balls return to the game   **/
bool delayBall(HWND hwnd){

    /**  default argument is a NULL  **/
    /**  we may need the window handle in a future version  **/
    /**  this function gets double duty  **/

    if(hwnd)  //  if not hull decrement the counter
        delaycount--;

    if(delaycount < 0)  // lock the delay count at zero
        delaycount = 0;

    if(delaycount)  // report the status if not zero
        return true;

    return false;  // report false if no delay count
}

/**   must I explain   **/
void cleanUp(HWND hwnd){

    /**   big message to bore the user    */
    TCHAR mesString [] =
                "WinBreakoutC++ 1.04 a CS50 style game of "
                "Breakout © 2015 Rob Clark et al.\n\n"
                "https://github.com/ebobtron/WinBreakoutCPlusPlus\n\n"
                "A demonstration of MS Windows programming "
                "with contributions from Faiza Harbi\nand "
                "Ben aka FreeER\n\n "
                "did you know that CTRL+C will copy this message.\n\n"
                "Click OK to exit WinBreakoutC++";

    /**   halt the game   */
    KillTimer(hwnd, ID_TIMER);

    /**   pop the message box but be prepare to return to the program   */
    if(MessageBox(hwnd, mesString, "Exit WinBreakoutC++ ?", MB_OKCANCEL) == IDOK){

        /**   our paddle rectangle is on the heap  */
        if(paddleRect)
            free(paddleRect);
        /**   quick an easy way to terminate the program could use exit   */
        DestroyWindow(hwnd);
    }
    else{

        /**   restart the game timer   */
        SetTimer(hwnd,ID_TIMER, 20, 0);
    }
}


