
#include <windows.h> 
#include "faiza.h"
#include "breakout.h"

/**   creates the bricks   **/
//void createBricks( wincl.sizeX,  wincl.sizeY){

	/**   preserve the length of the paddle   **/
	//printf("%d\n",wincl.sizeX);/*windows size 400, 600*/
	//printf("%d\n",wincl.sizeY);
	/**   load paddle rectangle  **/
	/*paddleRect->left = mwinRect.right / 2 - length / 2;
	paddleRect->top = posy;
	paddleRect->right = paddleRect->left + length;
	paddleRect->bottom = paddleRect->top + height;

	/**   load the paddle invalidate rectangle   **/
	/*paddleRectInvaild = *paddleRect;
	paddleRectInvaild.left = mwinRect.left;
	paddleRectInvaild.right = mwinRect.right;*/

	/*void drawPaddle(HDC hdc, COLORREF pen, COLORREF fill){

    COLORREF lastpen = NULL;
    COLORREF lastbrush =  NULL;

    if(pen || fill){
        SelectObject(hdc, GetStockObject(DC_PEN));
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        lastpen = SetDCPenColor(hdc, pen);
        lastbrush = SetDCBrushColor(hdc, fill);
    }

    Rectangle(hdc,
            paddleRect->left,     // int nLeftRect
            paddleRect->top,      // int nTopRect
            paddleRect->right,    // int nRightRect
            paddleRect->bottom);  // int nBottomRect

    if(pen || fill){
        SetDCPenColor(hdc, lastpen);
        SetDCBrushColor(hdc, lastbrush);
     }
}
*/
//}