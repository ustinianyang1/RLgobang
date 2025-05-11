#include <graphics.h>
#include <easyx.h>
#include "chess.h"

int main()
{
    initgraph(scenesize, scenesize);
    BeginBatchDraw();

    showStartScreen();

    ExMessage msg;
    while(true)
    {
        if(gameover)
        {
            showGameOver();
            continue;
        }

        while(peekmessage(&msg, EM_MOUSE | EM_KEY))
        {
            if(msg.message == WM_LBUTTONDOWN)
                chechClick(msg);
        }

        if(robotrobotgamestart)
        {
            automatch();
            drawBoard();
            FlushBatchDraw();
        }

        else if(robothumangamestart && ::count == 1)
        {
            aimove();
        }

        drawBoard();
        Sleep(10);
    }

    EndBatchDraw();
    return 0;
}