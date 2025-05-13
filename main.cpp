#include "chess.h"
#include "qlearning.h"
#include <graphics.h>
#include <conio.h>
#include "global.h"
#include <iostream>

extern QTable qTable;

void resetGame();
void checkButtonClick(int x, int y);
void initGraphics();
void processMouseEvent();
void gameLoop();
void showStartScreen();
void showGameOver();


void initGraphics()
{
    initgraph(SCENESIZE, SCENESIZE);
    setbkcolor(BOARD_COLOR);
    cleardevice();
    BeginBatchDraw();
}


void processMouseEvent()
{
    ExMessage msg;
    while(peekmessage(&msg, EM_MOUSE))
    {
        if(msg.message == WM_LBUTTONDOWN)
        {
            
            int gridX = (msg.x - RADIUS + UNIT_SIZE / 2) / UNIT_SIZE;
            int gridY = (msg.y - RADIUS + UNIT_SIZE / 2) / UNIT_SIZE;

            
            if(gridX >= 0 && gridX < BOARD_SIZE &&
                gridY >= 0 && gridY < BOARD_SIZE)
            {
                handleClick(gridX, gridY);
            }

            
            checkButtonClick(msg.x, msg.y);
        }
    }
}


void gameLoop()
{
    while(!gameover)
    {
        
        if(robothumangamestart)
        {
            if(current_player == 1)
            { 
                aiMove(qTable);
                FlushBatchDraw();
            }
            else
            { 
                processMouseEvent();
            }
        }

        
        if(robotrobotgamestart)
        {
            aiMove(qTable);
            FlushBatchDraw();
        }

        
        drawBoard();
        FlushBatchDraw();
        Sleep(5); 
    }
}

void showStartScreen()
{
    cleardevice();
    settextcolor(WHITE);

    
    settextstyle(60, 0, _T("微软雅黑"));
    int titleWidth = textwidth(L"五子棋");
    int titleHeight = textheight(L"五子棋");
    outtextxy((SCENESIZE - titleWidth) / 2, (SCENESIZE - titleHeight) / 2 - 100, L"五子棋");

    
    settextstyle(30, 0, _T("微软雅黑"));
    int humanVsAiWidth = textwidth(L"人机大战");
    int humanVsAiHeight = textheight(L"人机大战");
    int humanVsAiX = (SCENESIZE - humanVsAiWidth) / 2;
    int humanVsAiY = (SCENESIZE - humanVsAiHeight) / 2;
    outtextxy(humanVsAiX, humanVsAiY, L"人机大战");

    
    int aiVsAiWidth = textwidth(L"AI对战");
    int aiVsAiHeight = textheight(L"AI对战");
    int aiVsAiX = (SCENESIZE - aiVsAiWidth) / 2;
    int aiVsAiY = (SCENESIZE - aiVsAiHeight) / 2 + 50;
    outtextxy(aiVsAiX, aiVsAiY, L"AI对战");

    
    const GameButton trainBtn = {
        aiVsAiX - 50, aiVsAiY + 50,
        aiVsAiX + aiVsAiWidth + 50, aiVsAiY + aiVsAiHeight + 50,
        L"训练模式"
    };
    setfillcolor(BTN_COLOR);
    solidroundrect(trainBtn.left, trainBtn.top, trainBtn.right, trainBtn.bottom, 10, 10);
    settextcolor(WHITE);
    setbkmode(TRANSPARENT);
    settextstyle(20, 0, _T("微软雅黑"));
    int textWidth = textwidth(trainBtn.text);
    int textHeight = textheight(trainBtn.text);
    outtextxy(
        (trainBtn.left + trainBtn.right - textWidth) / 2,
        (trainBtn.top + trainBtn.bottom - textHeight) / 2,
        trainBtn.text
    );

    FlushBatchDraw();

    while(true)
    {
        ExMessage msg;
        if(peekmessage(&msg, EM_MOUSE) && msg.message == WM_LBUTTONDOWN)
        {
            if(msg.x >= humanVsAiX && msg.x <= humanVsAiX + humanVsAiWidth &&
                msg.y >= humanVsAiY && msg.y <= humanVsAiY + humanVsAiHeight)
            {
                robothumangamestart = true;
                break;
            }
            else if(msg.x >= aiVsAiX && msg.x <= aiVsAiX + aiVsAiWidth &&
                msg.y >= aiVsAiY && msg.y <= aiVsAiY + aiVsAiHeight)
            {
                robotrobotgamestart = true;
                break;
            }
            else if(msg.x >= trainBtn.left && msg.x <= trainBtn.right &&
                msg.y >= trainBtn.top && msg.y <= trainBtn.bottom)
            {
                try
                {
                    trainQLearning(qTable, 1000); 
                }
                catch(const std::exception &e)
                {
                    std::cerr << "训练过程中出现异常: " << e.what() << std::endl;
                }
            }
        }
    }
    cleardevice();
}


void showGameOver()
{
    cleardevice();
    settextcolor(WHITE);
    settextstyle(60, 0, _T("微软雅黑"));

    
    int titleWidth = textwidth(L"游戏结束");
    int titleHeight = textheight(L"游戏结束");
    outtextxy((SCENESIZE - titleWidth) / 2, (SCENESIZE - titleHeight) / 2 - 100, L"游戏结束");

    
    const wchar_t *result = current_player == 0 ? L"黑方获胜" : L"白方获胜";
    settextstyle(40, 0, _T("微软雅黑"));
    int resultWidth = textwidth(result);
    int resultHeight = textheight(result);
    outtextxy((SCENESIZE - resultWidth) / 2, (SCENESIZE - resultHeight) / 2 + 50, result);

    FlushBatchDraw();

    
    if(MessageBox(GetHWnd(), L"再玩一局？", L"游戏结束", MB_YESNO) == IDYES)
    {
        resetGame();
    }
    else
    {
        exit(0);
    }
}


int main()
{
    
    initGraphics();
    qTable = loadQTable("qtable.dat"); 

    
    showStartScreen();

    
    while(true)
    {
        processMouseEvent();

        
        if(gameover)
        {
            showGameOver();
        }

        
        gameLoop();
    }

    
    saveQTable(qTable, "qtable_updated.dat");
    EndBatchDraw();
    closegraph();
    return 0;
}


void resetGame()
{
    cover = std::vector<std::vector<int>>(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 2));
    history.clear();
    current_player = 0;
    step_count = -1;
    gameover = false;
    robothumangamestart = false;
    robotrobotgamestart = false;
    cleardevice();
}


void checkButtonClick(int x, int y)
{
    
    if(x >= undoBtn.left && x <= undoBtn.right &&
        y >= undoBtn.top && y <= undoBtn.bottom)
    {
        if(step_count >= 0)
        {
            auto [lastX, lastY] = history[step_count];
            cover[lastX][lastY] = 2;
            step_count--;
            current_player = (current_player + 1) % 2;
        }
    }

    if(x >= newGameBtn.left && x <= newGameBtn.right &&
        y >= newGameBtn.top && y <= newGameBtn.bottom)
    {
        resetGame();
    }

    const GameButton trainBtn = {
        SCENESIZE - 120, 200,
        SCENESIZE - 20, 240,
        L"训练模式"
    };
    if(x >= trainBtn.left && x <= trainBtn.right &&
        y >= trainBtn.top && y <= trainBtn.bottom)
    {
        try
        {
            trainQLearning(qTable, 10000); 
        }
        catch(const std::exception &e)
        {
            std::cerr << "训练过程中出现异常: " << e.what() << std::endl;
        }
    }
}