#pragma once
#include <windows.h>
#include <vector>
#include <utility>
const int unitsize = 60;
const int boxsize = 840;
const int scenesize = 900;
const int radius = unitsize / 2;
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

std::vector <std::vector<int>>cover(15, std::vector<int>(15, 2));//0->black, 1->white, 2->empty
std::vector <std::vector<int>> historyHeuristic(15, std::vector<int>(15, 0));//历史启发表
std::vector <std::vector<int>>blackscore(15, std::vector<int>(15, 0));
std::vector <std::vector<int>>whitescore(15, std::vector<int>(15, 0));
std::vector <std::vector<int>>blackminimaxscore(15, std::vector<int>(15, 0));
std::vector <std::vector<int>>whiteminimaxscore(15, std::vector<int>(15, 0));
std::vector <std::pair<int, int>> history;
static int count = 0;
static int step = -1;
enum Color
{
	black = 0, white = 1
};

const double attackpreferenceblack = 2;//黑方攻击分倍率
const double attackpreferencewhite = 1.5;//白方攻击分倍率

const int searchdepth = 4;//搜索深度

const int specialfive = 1000000;//嵌五
const int livefour = 900000;//活四
const int deadfour = 300000;//冲四
const int livespecialfour = 10010;//活嵌四
const int deadspecialfour = 110;//冲嵌四
const int livethree = 10000;//活三
const int deadthree = 100;//冲三
const int livespecialthree = 105;//活嵌三
const int deadspecialthree = 35;//冲嵌三
const int livetwo = 100;//活二
const int deadtwo = 5;//冲二
const int liveone = 20;//活单
const int deadone = 1;//冲单

const int max_branches = 20;//着法分支上限

const double depthdeclinerate = 0.9;//深度衰减倍率
