#pragma once
#include <windows.h>
#include <vector>
#include <utility>
const int unitsize = 60;
const int boxsize = 840;
const int scenesize = 900;
const int radius = unitsize / 2;
extern bool gameover;
extern bool robothumangamestart;
extern bool robotrobotgamestart;

// 按钮区域定义
extern RECT undoBtn;
extern RECT redoBtn;
extern RECT newGameBtn;
extern COLORREF BOARD_COLOR;
extern COLORREF BTN_COLOR;
extern COLORREF BTN_HOVER_COLOR;

extern std::vector <std::vector<int>>cover;
extern std::vector <std::vector<int>> historyHeuristic;
extern std::vector <std::vector<int>>blackscore;
extern std::vector <std::vector<int>>whitescore;
extern std::vector <std::vector<int>>blackminimaxscore;
extern std::vector <std::vector<int>>whiteminimaxscore;
extern std::vector <std::pair<int, int>> history;
extern int count;
extern int step;
enum Color
{
	black = 0, white = 1
};

const double attackpreferenceblack = 1.5;//黑方攻击分倍率
const double attackpreferencewhite = 1.3;//白方攻击分倍率

const int searchdepth = 3;//搜索深度

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

const int max_branches = 15;//着法分支上限

const double depthdeclinerate = 0.8;//深度衰减倍率

const int UPDATE_RADIUS = 5;  // 更新半径
