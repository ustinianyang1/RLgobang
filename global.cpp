#include "global.h"

bool gameover = false;
bool robothumangamestart = false;
bool robotrobotgamestart = false;

// 按钮区域定义
RECT undoBtn = {scenesize - 120, 20, scenesize - 20, 60};
RECT redoBtn = {scenesize - 120, 80, scenesize - 20, 120};
RECT newGameBtn = {scenesize - 120, 140, scenesize - 20, 180};
COLORREF BOARD_COLOR = RGB(255, 153, 51);
COLORREF BTN_COLOR = RGB(70, 130, 180);
COLORREF BTN_HOVER_COLOR = RGB(100, 149, 237);

std::vector<std::vector<int>> cover(15, std::vector<int>(15, 2));//0->black, 1->white, 2->empty
std::vector<std::vector<int>> historyHeuristic(15, std::vector<int>(15, 0));//历史启发表
std::vector<std::vector<int>> blackscore(15, std::vector<int>(15, 0));
std::vector<std::vector<int>> whitescore(15, std::vector<int>(15, 0));
std::vector<std::vector<int>> blackminimaxscore(15, std::vector<int>(15, 0));
std::vector<std::vector<int>> whiteminimaxscore(15, std::vector<int>(15, 0));
std::vector<std::pair<int, int>> history;
int count = 0;
int step = -1;
