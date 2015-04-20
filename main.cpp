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


int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszArgument, int nCmdShow)
{
    MSG messages;  /* Here messages to the application are saved */

    /* create the main window - width, height */
    HWND hwnd = createMainWin(hThisInstance, 400, 600);

    /* if main window handle invalid alert and quit  */
    if(!hwnd){
        MessageBox(NULL, _T(" can not create main window "),
                         _T(" WinBreakoutC++ "), MB_OK);
        PostQuitMessage(0);
    }

    /* make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
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
    createPaddle(mwinRect.bottom - 60, 40, 10);

    /* run the message loop. It will run until GetMessage() returns 0 */
    while(GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/**   This function is called by the Windows function DispatchMessage()   **/

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message,
                                 WPARAM wParam, LPARAM lParam)
{
    /**   process system messages   **/
    switch (message)
    {
        case WM_TIMER:
            InvalidateRect(hwnd, &ballRectInvaild, TRUE);
            InvalidateRect(hwnd, &textRect, TRUE);
            InvalidateRect(hwnd, &paddleRectInvaild, TRUE);
            updateGame(hwnd);
            UpdateWindow(hwnd);
            break;
        case WM_CREATE:
            SetTimer(hwnd, ID_TIMER, 20, 0);
            break;
        case WM_MOUSEMOVE:
            mpoint_x = MAKEPOINTS(lParam);
            updatePaddle();
            break;
        case WM_DESTROY:
            PostQuitMessage(0);  // send a WM_QUIT to the message queue
            break;
        case WM_PAINT:
            refreshWindow(hwnd);
            break;
        default:      // for messages that we don't deal with
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}
