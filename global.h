#pragma once
#include <windows.h>
#include <vector>
#include <utility>

const int unitsize = 60;
const int boxsize = 840;
const int scenesize = 900;
const int BOARD_SIZE = 15;
const int radius = unitsize / 2;
extern bool gameover;
extern bool robothumangamestart;
extern bool robotrobotgamestart;

// 按钮区域
extern RECT undoBtn;
extern RECT redoBtn;
extern RECT newGameBtn;
extern COLORREF BOARD_COLOR;
extern COLORREF BTN_COLOR;
extern COLORREF BTN_HOVER_COLOR;

extern std::vector <std::vector<int>> cover;
extern std::vector <std::vector<int>> historyHeuristic;
extern std::vector <std::vector<int>> blackscore;
extern std::vector <std::vector<int>> whitescore;
extern std::vector <std::vector<int>> blackminimaxscore;
extern std::vector <std::vector<int>> whiteminimaxscore;
extern std::vector <std::pair<int, int>> history;
extern int count;
extern int step;
enum Color
{
    black = 0, white = 1
};

const double attackpreferenceblack = 1.5; // 黑方攻击偏好
const double attackpreferencewhite = 1.3; // 白方攻击偏好

const int searchdepth = 2; // 搜索深度改为2

const int specialfive = 1000000; // 特殊五子
const int livefour = 900000; // 活四
const int deadfour = 300000; // 死四
const int livespecialfour = 10010; // 活特殊四
const int deadspecialfour = 110; // 死特殊四
const int livethree = 10000; // 活三
const int deadthree = 100; // 死三
const int livespecialthree = 105; // 活特殊三
const int deadspecialthree = 35; // 死特殊三
const int livetwo = 100; // 活二
const int deadtwo = 5; // 死二
const int liveone = 20; // 活一
const int deadone = 1; // 死一

const int max_branches = 15; // 最大分支数

const double depthdeclinerate = 0.8; // 深度衰减率

const int UPDATE_RADIUS = 5;  // 更新半径