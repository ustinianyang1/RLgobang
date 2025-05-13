#pragma once
#include "global.h"
#include <graphics.h>
#include <string>

class Chess
{
    public:
    int x, y;
    Color color;

    Chess(int x, int y, Color c): x(x), y(y), color(c)
    {
    }
    void draw() const;
};

struct Node
{
    int score;
    int x, y;

    Node(int s, int x, int y): score(s), x(x), y(y)
    {
    }
};


void initScore(std::vector<std::vector<int>> &score);
void updateScores(int lastX, int lastY);
int evaluate(int player, int depth);
Node minimax(int depth, int player, int alpha, int beta);
void drawBoard();
void showThinkingStatus(const std::wstring &msg);
void handleClick(int targetX, int targetY);
void aiMove(QTable &qtable);
void saveGame(const std::string &path);
void loadGame(const std::string &path);
bool checkWin(int x, int y);
int findsubstring(const std::string &s, const std::string &pattern);
bool checkthree(int &targetx, int &targety);
bool checkthreespecial(int &targetx, int &targety);
bool checkfour(int &targetx, int &targety);
bool checklong(int &targetx, int &targety);