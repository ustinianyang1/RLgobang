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


const double LEARNING_RATE = 0.1;    
const double DISCOUNT_FACTOR = 0.9;  
const int REPLAY_BUFFER_SIZE = 20000;
const int BATCH_SIZE = 32;           
const double EPSILON_START = 0.9;    
const double EPSILON_END = 0.1;      
const double EPSILON_DECAY = 0.995;  


const int SEARCH_DEPTH = 3;          
const double ATTACK_BLACK = 1.5;     
const double ATTACK_WHITE = 1.3;     


const int SPECIAL_FIVE = 1000000;    
const int LIVE_FOUR = 900000;        
const int DEAD_FOUR = 300000;        
const int LIVE_SPECIAL_FOUR = 10050; 
const int DEAD_SPECIAL_FOUR = 105; 
const int LIVE_THREE = 10000;        
const int DEAD_THREE = 100;          
const int LIVE_SPECIAL_THREE = 105;       
const int DEAD_SPECIAL_THREE = 10;       
const int LIVE_TWO = 100;            
const int DEAD_TWO = 5;              
const int LIVE_ONE = 20;             
const int DEAD_ONE = 1;              


extern bool gameover;                
extern bool robothumangamestart;     
extern bool robotrobotgamestart;     


using Experience = std::tuple<std::vector<std::vector<int>>, int, double, std::vector<std::vector<int>> >;
extern std::deque<Experience> replay_buffer;  


extern std::vector<std::vector<int>> cover;   
extern std::vector<std::vector<int>> blackscore;  
extern std::vector<std::vector<int>> whitescore;  
extern std::vector<std::pair<int, int>> history;  
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

const double DEPTH_DECLINE_RATE = 0.8; 
extern std::vector<std::vector<int>> historyHeuristic; 
extern QTable qTable; // ȫ��Q������