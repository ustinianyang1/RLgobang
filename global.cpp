#include "global.h"

QTable qTable;


bool gameover = false;
bool robothumangamestart = false;
bool robotrobotgamestart = false;


std::deque<Experience> replay_buffer;


std::vector<std::vector<int>> cover(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 2));
std::vector<std::vector<int>> blackscore(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
std::vector<std::vector<int>> whitescore(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
std::vector<std::vector<int>> historyHeuristic(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
std::vector<std::pair<int, int>> history;
int current_player = 0;
int step_count = -1;


const GameButton undoBtn = {
    SCENESIZE - 120, 20,
    SCENESIZE - 20, 60,
    L"����"
};
const GameButton redoBtn = {
    SCENESIZE - 120, 80,
    SCENESIZE - 20, 120,
    L"����"
};
const GameButton newGameBtn = {
    SCENESIZE - 120, 140,
    SCENESIZE - 20, 180,
    L"����Ϸ"
};


const COLORREF BOARD_COLOR = RGB(255, 153, 51);    
const COLORREF BTN_COLOR = RGB(70, 130, 180);      
const COLORREF BTN_HOVER_COLOR = RGB(100, 149, 237);// ����ɫ��ͣ