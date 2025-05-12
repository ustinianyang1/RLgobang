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

// ��ť����
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

const double attackpreferenceblack = 1.5; // �ڷ�����ƫ��
const double attackpreferencewhite = 1.3; // �׷�����ƫ��

const int searchdepth = 2; // ������ȸ�Ϊ2

const int specialfive = 1000000; // ��������
const int livefour = 900000; // ����
const int deadfour = 300000; // ����
const int livespecialfour = 10010; // ��������
const int deadspecialfour = 110; // ��������
const int livethree = 10000; // ����
const int deadthree = 100; // ����
const int livespecialthree = 105; // ��������
const int deadspecialthree = 35; // ��������
const int livetwo = 100; // ���
const int deadtwo = 5; // ����
const int liveone = 20; // ��һ
const int deadone = 1; // ��һ

const int max_branches = 15; // ����֧��

const double depthdeclinerate = 0.8; // ���˥����

const int UPDATE_RADIUS = 5;  // ���°뾶