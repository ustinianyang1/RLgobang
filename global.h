#pragma once
#include <windows.h>
#include <vector>
#include <utility>
#include <deque>
#include <string>
#include "qlearning.h"
#include <unordered_map>

enum class Color
{
	Black = 0, White = 1, Empty = 2
};

const int BOARD_SIZE = 15;          
const int UNIT_SIZE = 60;           
const int BOXSIZE = 840;            
const int SCENESIZE = 900;          
const int RADIUS = UNIT_SIZE / 2;   
const int UPDATE_RADIUS = 5;        

const double LEARNING_RATE_START = 0.2;  // 学习率起始值
const double LEARNING_RATE_END = 0.01;    // 学习率结束值
const double LEARNING_RATE_DECAY = 0.99;  // 学习率衰减率

const double DISCOUNT_FACTOR = 0.95;      // 折扣因子
const int REPLAY_BUFFER_SIZE = 50000;     // 回放缓冲区
const int BATCH_SIZE = 64;                // 批次大小
const double EPSILON_START = 0.9;
const double EPSILON_END = 0.05;
const double EPSILON_DECAY = 0.99;

const int TrainNumber = 100000;           // 训练次数增加
const int TrainSleepTime = 80;            // 训练时的睡眠时间

const int CENTER_REWARD = 10;             // 中心位置奖励
const double LIVE_TWO_REWARD = 0.1;
const double LIVE_THREE_REWARD = 0.5;
const double LIVE_FOUR_REWARD = 0.8;

const int SEARCH_DEPTH = 3;          
const double ATTACK_BLACK = 1.0;     
const double ATTACK_WHITE = 1.0;     

const int SPECIAL_FIVE = 1000000;    
const int LIVE_FOUR = 50000;        
const int DEAD_FOUR = 10000;        
const int LIVE_SPECIAL_FOUR = 1010; 
const int DEAD_SPECIAL_FOUR = 105; 
const int LIVE_THREE = 1000;        
const int DEAD_THREE = 100;          
const int LIVE_SPECIAL_THREE = 30;       
const int DEAD_SPECIAL_THREE = 10;       
const int LIVE_TWO = 25;            
const int DEAD_TWO = 5;              
const int LIVE_ONE = 10;             
const int DEAD_ONE = 1;              

extern bool gameover;                
extern bool robothumangamestart;     
extern bool robotrobotgamestart;     

using Experience = std::tuple<std::vector<std::vector<int>>, int, double, std::vector<std::vector<int>> >;

extern std::vector<std::vector<int>> cover;   
extern std::vector<std::vector<int>> blackscore;  
extern std::vector<std::vector<int>> whitescore;  
extern std::vector<std::pair<int, int>> history;  
extern std::vector<std::vector<int>> historyHeuristic;

extern int current_player;           
extern int step_count;               

struct GameButton
{
	int left;
	int top;
	int right;
	int bottom;
	const wchar_t *text;
};
extern const GameButton undoBtn;     
extern const GameButton redoBtn;     
extern const GameButton newGameBtn;  

extern const COLORREF BOARD_COLOR;   
extern const COLORREF BTN_COLOR;     
extern const COLORREF BTN_HOVER_COLOR;

extern QTable qTable;

using PriorityExperience = std::tuple<double, std::vector<std::vector<int>>, int, double, std::vector<std::vector<int>>>;
struct ComparePriority
{
	bool operator()(const PriorityExperience &a, const PriorityExperience &b) const
	{
		return std::get<0>(a) < std::get<0>(b);
	}
};
extern std::deque<Experience> replay_buffer;