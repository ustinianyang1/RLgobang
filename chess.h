#pragma once
#include "global.h"
#include <string>

class Chess
{
    public:
    int x, y;
    Color color;
    Chess(int x, int y, Color c): x(x), y(y), color(c)
    {
    };
    void drawChess();
    bool checkfive();
};

class Node
{
    public:
    int score;
    int x, y;
    Node(int score, int x, int y): score(score), x(x), y(y)
    {
    };
};

// º¯ÊýÉùÃ÷
void iniscorevector(std::vector<std::vector<int>> &v);
void upgradescore(int lastX, int lastY);
int evaluate(int player, int curdepth);
Node minimax(int depth, int player, int alpha, int beta);
void drawBoard();
void showStartScreen();
void showGameOver();
void clearmessagequeue(ExMessage &msg);
int findsubstring(const std::string &s, const std::string &pattern);
bool checkfour(int &targetx, int &targety);
int checkthreespecial(int &targetx, int &targety);
bool checkthree(int &targetx, int &targety);
bool checklong(int &targetx, int &targety);
void chechClick(ExMessage &msg);
void automatch();
void aimove();
void drawButton(const RECT &rect, const wchar_t *text, bool hover);
void showThinkingStatus(const wchar_t *message);