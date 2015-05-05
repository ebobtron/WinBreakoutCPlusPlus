/**
*
*   WinBreakoutC++
*
*   main.cpp
*
*   2015 a collaboration of CS50x students
*
*/

#include "breakout.h"

/**   WinMain where we communicate with the system              **/
/**   after we define and instantiate our programs components   **/
int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nCmdShow)
{
    /**   this should always be NULL always   **/
    if(hPrevInstance){
       MessageBox(NULL, _T("there should be no previous instance data")
                      , _T("WinBreakoutC++"), MB_ICONSTOP|MB_OK);
       return 0;
    }

    /**   WinBreakoutc++ doesn't take command line arguments   **/
    if(lpszArgument[0]){
        MessageBox(NULL, _T("WinBreakoutC++ don't use command line arguments")
                      , _T("WinBreakoutC++"), MB_OK);
    }
    //

    /**   here messages to the application are saved   **/
    MSG messages;

   /**********************************************
    *                                            *
    *   create the main window - width, height   *
    *                                            *
    **********************************************/
    HWND hwnd = createMainWin(hThisInstance, 400, 600);

    /**   if main window handle is invalid alert and quit  **/
    if(!hwnd){
        // using wide string for the Unicode crowd
        MessageBox(NULL, _T(" can not create main window "),
                         _T(" WinBreakoutC++ "), MB_OK);

        /**   this in itself will end the program   **/
        PostQuitMessage(0);
    }

    /**   make the window visible on the screen   **/
    ShowWindow(hwnd, nCmdShow);

/**   some debugging code                                                             **/
/**   with the following statements a debug build target will open a console window   **/
/**   allowing us to use printf and cout to display data                              **/
/**   NOTE: to use 'cout' iostream.h and static library libcmtd.lib must
/**         be linked to your code                                                    **/

 #ifdef _DEBUG
    if(!AllocConsole()){
        MessageBox(NULL, _T(" can not create console window "),
                         _T(" WinBreakoutC++ "), MB_OK);
    }
    FILE* cp; // console file or stream pointer
    freopen_s(&cp, "CONOUT$", "wb", stdout);  // reassign stout handle to console windows
                                              // output file pointer / stream
#endif // _DEBUG

    /**   get our windows rectangle so we can size things   **/
    GetClientRect(hwnd, &mwinRect);

   /********************************************
    *   make the ball                          *
    *   size diameter, center x and center y   *
    ********************************************/
    createBall(20, mwinRect.right / 2, mwinRect.bottom / 2);

   /**********************************
    *   make the paddle              *
    *   y position, length, height   *
    **********************************/
    createPaddle(int(mwinRect.bottom *.8), 40, 10);

   /***************************
    *   make the brick wall   *
    ***************************/
    createWall(0);

    /**   run the message loop                        **/
    /**   It will run until GetMessage() returns 0.   **/
    /**   argument two is null, we are listening to   **/
    /**   the thread not the window.  We get all      **/
    /**   messages.  This loop is the heart of our    **/
    /**   program pumping messages not blood,         **/

    /**   "messages" is a MSG structure where messages are placed by     **/
    /**   GetMessage, defined at the top of WinMain                      **/

    while(GetMessage(&messages, NULL, 0, 0))
    {
        /**   Translate virtual-key messages into character messages   **/
        TranslateMessage(&messages);

        /**   Send message to WindowProcedure   **/
        DispatchMessage(&messages);
    }

    /**   The program return-value is 0                   **/
    /**   - The value that PostQuitMessage() gave was 0   **/
    /**   that's all folks                                **/
    return messages.wParam;
}

/**   this function is called by the Windows function                    **/
/**   DispatchMessage() from inside the main message loop of WinMain     **/
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message,
                                 WPARAM wParam, LPARAM lParam)
{
    /**   process system messages   **/
    switch (message){
        /**   message from any timer we set "ID_TIMER" and "ID_TIMER2"    */
        case WM_TIMER:
            switch(wParam){
                case ID_TIMER:     //  the main game timer
                     setUpdateRegion(hwnd);    // add rectangle to update region
                     updateGame(hwnd);    // do the game math update all rectangles
                     UpdateWindow(hwnd);    // send WM_PAINT if update region is not empty
                    break;
                case ID_TIMER2:    // ball delay timer
                     delayBall(hwnd);
                      // restart ball
                    break;
                default:;
            }
            break;
        /**   when our window is created            */
        case WM_CREATE:
             SetTimer(hwnd, ID_TIMER, 20, 0);    // set timer running
            break;
        /** whenever the mouse moves over our window    **/
        case WM_MOUSEMOVE:
             mpoint_x = MAKEPOINTS(lParam);    // store mouse position
             updatePaddle();    // update paddle rectangle immediately
            break;
        /**   whenever a character key is pressed       */
        case WM_CHAR:
             charKeyPress(hwnd, wParam);  // pause start program control
            break;
        /**   if the window is resized     **/
        case WM_SIZE:
             winSizeChange(hwnd);
            break;
        /**   when a request to close come from a user   **/
        case WM_CLOSE:
             cleanUp(hwnd);
            break;
        case WM_DESTROY:    // this turns off our program
             PostQuitMessage(0);    // send a WM_QUIT a 0 to the message queue
            break;
        /**   whenever some part of a window needs updating   **/
        case WM_PAINT:    // the system message to paint the update region
             refreshWindow(hwnd);    // paint all rectangles to update region
            break;
        default:      // for messages that we don't deal with
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
