#include <graphics.h>  
#include <conio.h>  
#include <windows.h>
#include <climits>  
#include <cmath>  
#include <iostream>
#include <easyx.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include "global.h"

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

void iniscorevector(std::vector<std::vector<int>> &v);
void upgradescore();
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

void drawButton(const RECT &rect, const wchar_t *text, bool hover = false)
{
	setfillcolor(hover ? BTN_HOVER_COLOR : BTN_COLOR);
	solidroundrect(rect.left, rect.top, rect.right, rect.bottom, 10, 10);

	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, _T("微软雅黑"));
	int textWidth = textwidth(text);
	int textHeight = textheight(text);
	outtextxy(
		(rect.left + rect.right - textWidth) / 2,
		(rect.top + rect.bottom - textHeight) / 2,
		text
	);
}

void iniscorevector(std::vector<std::vector<int>> &v)
{
	const int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
	const int dy[8] = {0, -1, 1, 1, -1, -1, 0, 1};
	std::queue<std::pair<int, int>> q;
	std::vector<std::vector<bool>> visited(cover.size(), std::vector<bool>(cover[0].size(), false));
	q.push({(visited.size() - 1) / 2, (visited[0].size() - 1) / 2});
	v[(visited.size() - 1) / 2][(visited[0].size() - 1) / 2] = (visited.size() - 1) / 2;
	visited[(visited.size() - 1) / 2][(visited[0].size() - 1) / 2] = true;

	while(!q.empty())
	{
		auto temp = q.front();
		q.pop();

		for(int dir = 0; dir < 8; dir++)
		{
			int nx = temp.first + dx[dir];
			int ny = temp.second + dy[dir];

			if(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size() && !visited[nx][ny])
			{
				v[nx][ny] = max(0, v[temp.first][temp.second] - 1);
				visited[nx][ny] = true;
				q.push({nx, ny});
			}
		}
	}
}

void upgradescore()
{
	int findlivetwo = 0, findlivethree = 0, finddeadfour = 0;
	const int dx[4] = {1, 0, 1, 1};
	const int dy[4] = {0, 1, 1, -1};
	iniscorevector(blackscore);
	iniscorevector(whitescore);
	for(int x = 0; x < cover.size(); x++)
	{
		for(int y = 0; y < cover[0].size(); y++)
		{
			if(cover[x][y] != 2)
				continue;
			for(int player = 0; player <= 1; player++)
			{
				int score = 0;
				for(int d = 0; d < 4; d++)
				{
					int countright = 0;
					int blockright = 0;
					int emptyright = 0;
					for(int step = 1; step <= 5; step++)
					{
						int nx = x + step * dx[d];
						int ny = y + step * dy[d];
						if(nx < 0 || ny < 0 || nx >= cover.size() || ny >= cover[0].size())
						{
							blockright++;
							break;
						}
						if(cover[nx][ny] == player)
							countright++;
						else if(cover[nx][ny] == 2)
						{
							emptyright++;
							break;
						}
						else
						{
							blockright++;
							break;
						}
					}
					int countleft = 0;
					int blockleft = 0;
					int emptyleft = 0;
					for(int step = 1; step <= 5; step++)
					{
						int nx = x - step * dx[d];
						int ny = y - step * dy[d];
						if(nx < 0 || ny < 0 || nx >= cover.size() || ny >= cover[0].size())
						{
							blockleft++;
							break;
						}
						if(cover[nx][ny] == player)
							countleft++;
						else if(cover[nx][ny] == 2)
						{
							emptyleft++;
							break;
						}
						else
						{
							blockleft++;
							break;
						}
					}
					if(countleft == 4 && emptyleft == 1)//活四
						score += livefour;
					if(countright == 4 && emptyleft == 1)
						score += livefour;

					if(countright == 3 && countleft == 1)//嵌五
						score += specialfive;
					else if(countright == 2 && countleft == 2)
						score += specialfive;
					else if(countright == 1 && countleft == 3)
						score += specialfive;

					if(countleft == 4 && blockleft == 1)//冲四
						score += deadfour;
					if(countright == 4 && blockright == 1)
						score += deadfour;

					if(countright == 2 && countleft == 1 && emptyleft == 1 && emptyright == 1)//活嵌四
						score += livespecialfour;
					else if(countright == 1 && countleft == 2 && emptyleft == 1 && emptyright == 1)
						score += livespecialfour;

					if(countright == 2 && countleft == 1 && !(emptyleft == 1 && emptyright == 1))//冲嵌四
						score += deadspecialfour;
					else if(countright == 1 && countleft == 2 && !(emptyleft == 1 && emptyright == 1))
						score += deadspecialfour;

					if(countleft == 3 && emptyleft == 1)//活三
						score += livethree / (countright == 0 && blockright == 1 ? 5 : 1);
					if(countright == 3 && emptyright == 1)
						score += livethree / (countleft == 0 && blockleft == 1 ? 5 : 1);

					if(countleft == 3 && blockleft == 1 && !(countright == 0 && blockright == 1))//冲三
						score += deadthree;
					if(countright == 3 && blockright == 1 && !(countleft == 0 && blockleft == 1))
						score += deadthree;

					if(countright == 1 && countleft == 1 && emptyleft == 1 && emptyright == 1)//活嵌三
						score += livespecialthree;

					if(countright == 1 && countleft == 1 && !(emptyleft == 1 && emptyright == 1))//冲嵌三
						score += deadspecialthree;

					if(countleft == 2 && emptyleft == 1)//活二
						score += livetwo / (countright == 0 && blockright == 1 ? 5 : 1);
					if(countright == 2 && emptyright == 1 && countleft != 0)
						score += livetwo / (countleft == 0 && blockleft == 1 ? 5 : 1);

					if(countleft == 2 && blockleft == 1 && !(countright == 0 && blockright == 1))//冲二
						score += deadtwo;
					if(countright == 2 && blockright == 1 && !(countleft == 0 && blockleft == 1))
						score += deadtwo;

					if(countleft == 1 && emptyleft == 1)//活单
						score += liveone / (countright == 0 && blockright == 1 ? 5 : 1);
					if(countright == 1 && emptyright == 1)
						score += liveone / (countleft == 0 && blockleft == 1 ? 5 : 1);

					if(countleft == 1 && blockleft == 1 && !(countright == 0 && blockright == 1))//冲单
						score += deadone;
					if(countright == 1 && blockright == 1 && !(countleft == 0 && blockleft == 1))
						score += deadone;
				}
				if(player == 0)
					blackscore[x][y] += score;
				else
					whitescore[x][y] += score;
			}
		}
	}
}

int evaluate(int player, int curdepth)
{
	const float depthdecline = pow(depthdeclinerate, curdepth);
	const int center_x = cover.size() / 2;
	const int center_y = cover[0].size() / 2;

	int totalscore = 0;

	std::vector<std::vector<int>> center_dist(cover.size(), std::vector<int>(cover[0].size()));
	for(int x = 0; x < cover.size(); x++)
	{
		for(int y = 0; y < cover[0].size(); y++)
		{
			center_dist[x][y] = abs(x - center_x) + abs(y - center_y);
		}
	}

	for(int x = 0; x < cover.size(); x++)
	{
		for(int y = 0; y < cover[0].size(); y++)
		{
			if(cover[x][y] != 2)
				continue;

			int basescore = 0;
			if(player == 0)
				basescore = blackscore[x][y] * attackpreferenceblack - whitescore[x][y];
			else
				basescore = whitescore[x][y] * attackpreferencewhite - blackscore[x][y];

			int centerreward = (cover.size() - center_dist[x][y]) * 2;

			int edgepunish = 0;
			if(x <= 2 || x >= 12 || y <= 2 || y >= 12)
			{
				edgepunish = 100;
			}

			//综合得分 = (基础分 + 中心奖励 - 边缘惩罚) * 深度衰减
			int finalscore = (basescore + centerreward - edgepunish) * depthdecline;
			totalscore += finalscore;
		}
	}
	return totalscore;
}

Node minimax(int depth, int player, int alpha, int beta)
{
	if(depth == 0 || gameover)
		return Node(evaluate(player, searchdepth - depth), -1, -1);

	std::vector<std::pair<int, int>> moves;
	for(int x = 0; x < cover.size(); x++)
	{
		for(int y = 0; y < cover[0].size(); y++)
		{
			if(cover[x][y] == 2)
			{
				moves.emplace_back(x, y);
			}
		}
	}

	sort(moves.begin(), moves.end(), [&](auto a, auto b)
		{
			int historyA = historyHeuristic[a.first][a.second];
			int historyB = historyHeuristic[b.first][b.second];
			int scoreA = (player == 0 ? blackscore[a.first][a.second] : whitescore[a.first][a.second]) + historyA;
			int scoreB = (player == 0 ? blackscore[b.first][b.second] : whitescore[b.first][b.second]) + historyB;
			return player == 0 ? scoreA > scoreB : scoreA < scoreB;
		});

	Node best = Node((player == 0 ? INT_MIN : INT_MAX), -1, -1);

	for(auto &move : moves)
	{
		int x = move.first, y = move.second;
		cover[x][y] = player;

		Node cur = minimax(depth - 1, (player + 1) % 2, alpha, beta);
		cover[x][y] = 2;

		if(player == 0)
		{
			if(cur.score > best.score)
			{
				best = cur;
				alpha = max(alpha, cur.score);
			}
			if(alpha >= beta)
				break;
		}
		else
		{
			if(cur.score < best.score)
			{
				best = cur;
				beta = min(beta, cur.score);
			}
			if(beta <= alpha)
				break;
		}
	}
	if(best.x != -1 && depth == searchdepth)
		historyHeuristic[best.x][best.y] += 1 << depth;
	return best;
}

void drawBoard()
{
	// 绘制背景
	setbkcolor(BOARD_COLOR);
	cleardevice();

	// 绘制棋盘
	setlinecolor(RGB(139, 69, 19));
	setlinestyle(PS_SOLID, 2);
	for(int x = radius; x <= boxsize + radius; x += unitsize)
	{
		line(x, radius, x, boxsize + radius);
		line(radius, x, boxsize + radius, x);
	}

	// 绘制棋子
	for(int x = 0; x < cover.size(); x++)
	{
		for(int y = 0; y < cover[0].size(); y++)
		{
			if(cover[x][y] != 2)
			{
				setfillcolor(cover[x][y] == 0 ? BLACK : WHITE);
				setlinecolor(cover[x][y] == 0 ? RGB(50, 50, 50) : RGB(200, 200, 200));
				solidcircle(x * unitsize + radius, y * unitsize + radius, radius - 5);
			}
		}
	}

	// 绘制侧边栏
	setfillcolor(RGB(245, 245, 220));
	solidrectangle(boxsize + radius * 2, 0, scenesize, scenesize);

	// 绘制按钮
	drawButton(undoBtn, L"撤销");
	drawButton(redoBtn, L"恢复");
	drawButton(newGameBtn, L"新游戏");

	// 显示当前玩家
	settextcolor(BLACK);
	settextstyle(25, 0, _T("微软雅黑"));
	outtextxy(scenesize - 130, scenesize - 100,
		::count == 0 ? L"当前：黑方" : L"当前：白方");
}

void showStartScreen()
{
	cleardevice();

	// 背景渐变
	for(int i = 0; i < 256; i++)
	{
		setfillcolor(RGB(i, 255 - i, 255));
		solidrectangle(0, i * 4, scenesize, (i + 1) * 4);
	}

	settextcolor(WHITE);
	settextstyle(50, 0, _T("微软雅黑"));
	const wchar_t *mainTitle = L"五子棋";
	int titleWidth = textwidth(mainTitle);
	outtextxy((scenesize - titleWidth) / 2, 200, mainTitle);

	// 绘制开始按钮
	RECT startBtn1 = {scenesize / 2 - 100, 350, scenesize / 2 + 100, 400};
	RECT startBtn2 = {scenesize / 2 - 100, 450, scenesize / 2 + 100, 500};

	while(true)
	{
		// 检测鼠标悬停
		ExMessage msg;
		peekmessage(&msg, EM_MOUSE);

		drawButton(startBtn1, L"人机对战",
			msg.x > startBtn1.left && msg.x < startBtn1.right &&
			msg.y > startBtn1.top && msg.y < startBtn1.bottom);

		drawButton(startBtn2, L"AI对战",
			msg.x > startBtn2.left && msg.x < startBtn2.right &&
			msg.y > startBtn2.top && msg.y < startBtn2.bottom);

		if(msg.message == WM_LBUTTONDOWN)
		{
			if(msg.x > startBtn1.left && msg.x < startBtn1.right &&
				msg.y > startBtn1.top && msg.y < startBtn1.bottom)
			{
				robothumangamestart = true;
				return;
			}
			if(msg.x > startBtn2.left && msg.x < startBtn2.right &&
				msg.y > startBtn2.top && msg.y < startBtn2.bottom)
			{
				robotrobotgamestart = true;
				return;
			}
		}
		Sleep(10);
	}
}

void showGameOver()
{
	setbkcolor(BLACK);
	cleardevice();

	settextcolor(RED);
	settextstyle(50, 0, _T("宋体"));
	const wchar_t *winText = (::count == 1) ? _T("黑方胜利!") : _T("白方胜利!");
	int textWidth = textwidth(winText);
	outtextxy((scenesize - textWidth) / 2, 300, winText);

	settextstyle(30, 0, _T("宋体"));
	const wchar_t *tipText = _T("按 R 重新开始   按 ESC 退出");
	int tipWidth = textwidth(tipText);
	outtextxy((scenesize - tipWidth) / 2, 400, tipText);

	while(true)
	{
		if(_kbhit())
		{
			int key = _getch();
			if(tolower(key) == 'r')
			{
				gameover = false;
				robothumangamestart = true;
				::count = 0;
				cover = std::vector<std::vector<int>>(16, std::vector<int>(16, 2));
				cleardevice();
				drawBoard();
				return;
			}
			else if(key == 27)
			{
				closegraph();
				exit(0);
			}
		}
		Sleep(10);
	}
}

void clearmessagequeue(ExMessage &msg)
{
	while(peekmessage(&msg, EM_MOUSE | EM_KEY, true));
}

void Chess::drawChess()
{
	setfillcolor(this->color == black ? BLACK : WHITE);
	setlinecolor(this->color == black ? BLACK : WHITE);
	fillcircle(x * unitsize + radius, y * unitsize + radius, radius - 3);
}

bool Chess::checkfive()
{
	const int dx[4] = {1, 0, 1, 1};
	const int dy[4] = {0, 1, 1, -1};
	for(int d = 0; d < 4; d++)
	{
		int inrow = 1;
		for(int step = 1; step < 5; ++step)
		{
			int nx = x + step * dx[d];
			int ny = y + step * dy[d];
			if(nx < 0 || ny < 0 || nx >= cover.size() || ny >= cover[0].size() || cover[nx][ny] != int(color))
				break;
			++inrow;
		}
		for(int step = 1; step < 5; ++step)
		{
			int nx = x - step * dx[d];
			int ny = y - step * dy[d];
			if(nx < 0 || ny < 0 || nx >= cover.size() || ny >= cover[0].size() || cover[nx][ny] != int(color))
				break;
			++inrow;
		}
		if(inrow >= 5)
			return true;
	}
	return false;
}

int findsubstring(const std::string &s, const std::string &pattern)
{
	int line = 0;
	size_t pos = 0;
	while((pos = s.find(pattern, pos)) != std::string::npos)
	{
		line++;
		pos++;
		if(pos >= s.size())
			break;
	}
	return line;
}

bool checkfour(int &targetx, int &targety)
{
	if(::count)
		return false;
	int line = 0;
	const int dx[4] = {1, 0, 1, 1};
	const int dy[4] = {0, 1, 1, -1};

	for(int d = 0; d < 4; d++)
	{
		int inrow = 0;
		for(int step = 1; step <= 4; step++)
		{
			int nx = targetx + step * dx[d];
			int ny = targety + step * dy[d];
			if(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size() && cover[nx][ny] == 0)
				inrow++;
			else
				break;
		}
		for(int step = 1; step <= 4; step++)
		{
			int nx = targetx - step * dx[d];
			int ny = targety - step * dy[d];
			if(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size() && cover[nx][ny] == 0)
				inrow++;
			else
				break;
		}
		if(inrow >= 3)
		{
			bool left = false, right = false;
			if(targetx + 5 * dx[d] >= 0 && targety + 5 * dy[d] >= 0 && targetx + 5 * dx[d] < cover.size() && targety + 5 * dy[d] < cover[0].size())
				right = (cover[targetx + 5 * dx[d]][targety + 5 * dy[d]] == 2);
			if(targetx - 5 * dx[d] >= 0 && targety - 5 * dy[d] >= 0 && targetx - 5 * dx[d] < cover.size() && targety - 5 * dy[d] < cover[0].size())
				left = (cover[targetx - 5 * dx[d]][targety - 5 * dy[d]] == 2);
			if(left || right)
				line++;
		}
	}
	return line >= 2;
}

int checkthreespecial(int &targetx, int &targety)
{
	if(::count)
		return false;
	int line = 0;
	const int dx[4] = {1, 0, 1, 1};
	const int dy[4] = {0, 1, 1, -1};
	for(int d = 0; d < 4; d++)
	{
		std::string temp;
		int nx = targetx, ny = targety;
		while(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size())
		{
			nx -= dx[d], ny -= dy[d];
		}
		while(1)
		{
			nx += dx[d], ny += dy[d];
			if(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size())
				temp.push_back('0' + cover[nx][ny]);
			else
				break;
		}
		size_t pos = 0;
		while((pos = temp.find("20002", pos)) != std::string::npos)
		{
			if((pos > 0 && temp[pos - 1] == '2') || (pos + 5 < temp.length() && temp[pos + 5] == '2'))
				line++;
			pos++;
		}
	}
	return line >= 2;
}

bool checkthree(int &targetx, int &targety)
{
	if(::count)
		return false;
	int line = 0;
	const int dx[4] = {1, 0, 1, 1};
	const int dy[4] = {0, 1, 1, -1};
	for(int d = 0; d < 4; d++)
	{
		std::string temp;
		int nx = targetx, ny = targety;
		while(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size())
		{
			nx -= dx[d], ny -= dy[d];
		}
		while(1)
		{
			nx += dx[d], ny += dy[d];
			if(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size())
				temp.push_back('0' + cover[nx][ny]);
			else
				break;
		}
		line += checkthreespecial(targetx, targety);
		if(!temp.empty())
		{
			line += findsubstring(temp, "202002");
			line += findsubstring(temp, "200202");
		}
	}
	if(line >= 2)
		return true;
	return false;
}

bool checklong(int &targetx, int &targety)
{
	if(::count)
		return false;
	int line = 0;
	const int dx[4] = {1, 0, 1, 1};
	const int dy[4] = {0, 1, 1, -1};
	for(int d = 0; d < 4; d++)
	{
		std::string temp;
		int nx = targetx, ny = targety;
		while(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size())
		{
			nx -= dx[d], ny -= dy[d];
		}
		while(1)
		{
			nx += dx[d], ny += dy[d];
			if(nx >= 0 && ny >= 0 && nx < cover.size() && ny < cover[0].size())
				temp.push_back('0' + cover[nx][ny]);
			else
				break;
		}
		if(!temp.empty())
		{
			if(temp.find("020000") != std::string::npos || temp.find("002000") != std::string::npos || temp.find("000200") != std::string::npos || temp.find("000020") != std::string::npos)
				return true;
		}
	}
	return false;
}

void automatch()
{
	upgradescore();
	Node target = minimax(searchdepth, ::count, INT_MIN, INT_MAX);
	if(target.x == -1 || target.y == -1)
	{
		int targetx = (cover.size() - 1) / 2, targety = (cover[0].size() - 1) / 2;
		if(::count == 0)
		{
			for(int x = 0; x < cover.size(); x++)
			{
				for(int y = 0; y < cover[0].size(); y++)
				{
					if(cover[x][y] == 2)
					{
						cover[x][y] = 0;
						if(checkfour(x, y) || checkthree(x, y) || checklong(x, y))
							blackscore[x][y] = -1;
						cover[x][y] = 2;
					}
				}
			}
		}
		int highestscore = INT_MIN;
		for(int x = 0; x < cover.size(); x++)
		{
			for(int y = 0; y < cover[0].size(); y++)
			{
				if(cover[x][y] == 2)
				{
					if(::count == 0 && blackscore[x][y] == -1)
						continue;
					int curscore = (::count == 0 ? blackscore[x][y] : whitescore[x][y]) * (::count == 0 ? attackpreferenceblack : attackpreferencewhite) + (::count == 0 ? whitescore[x][y] : blackscore[x][y]);
					if(curscore > highestscore)
					{
						highestscore = curscore;
						targetx = x;
						targety = y;
					}
				}
			}
		}
		if(highestscore == INT_MIN)
			system("pause");
		target.x = targetx, target.y = targety;
	}
	history.push_back(std::make_pair(target.x, target.y));
	step++;
	Chess chess(target.x, target.y, Color(::count));
	cover[target.x][target.y] = ::count;
	chess.drawChess();
	::count = (::count + 1) % 2;
	if(chess.checkfive())
	{
		//Sleep(2000);
		system("pause");
		robotrobotgamestart = false;
		gameover = true;
	}
}

void aimove()
{
	upgradescore();
	Node target = minimax(searchdepth, ::count, INT_MIN, INT_MAX);
	if(target.x == -1 || target.y == -1)
	{
		int targetx = (cover.size() - 1) / 2, targety = (cover[0].size() - 1) / 2;
		if(::count == 0)
		{
			for(int x = 0; x < cover.size(); x++)
			{
				for(int y = 0; y < cover[0].size(); y++)
				{
					if(cover[x][y] == 2)
					{
						cover[x][y] = 0;
						if(checkfour(x, y) || checkthree(x, y) || checklong(x, y))
							blackscore[x][y] = -1;
						cover[x][y] = 2;
					}
				}
			}
		}
		int highestscore = INT_MIN;
		for(int x = 0; x < cover.size(); x++)
		{
			for(int y = 0; y < cover[0].size(); y++)
			{
				if(cover[x][y] == 2)
				{
					if(::count == 0 && blackscore[x][y] == -1)
						continue;
					int curscore = (::count == 0 ? blackscore[x][y] : whitescore[x][y]) * (::count == 0 ? attackpreferenceblack : attackpreferencewhite) + (::count == 0 ? whitescore[x][y] : blackscore[x][y]);
					if(curscore > highestscore)
					{
						highestscore = curscore;
						targetx = x;
						targety = y;
					}
				}
			}
		}
		if(highestscore == INT_MIN)
			system("pause");
		target.x = targetx, target.y = targety;
	}
	history.push_back(std::make_pair(target.x, target.y));
	step++;
	Chess chess(target.x, target.y, Color(::count));
	cover[target.x][target.y] = ::count;
	chess.drawChess();
	::count = (::count + 1) % 2;
	if(chess.checkfive())
	{
		Sleep(2000);
		robothumangamestart = false;
		gameover = true;
	}
}

void chechClick(ExMessage &msg)
{
	if(msg.message == WM_LBUTTONDOWN)
	{
		// 检测按钮点击
		if(msg.x > undoBtn.left && msg.x < undoBtn.right &&
			msg.y > undoBtn.top && msg.y < undoBtn.bottom)
		{
			if(step >= 0)
			{
				cover[history[step].first][history[step].second] = 2;
				step--;
				::count = (::count + 1) % 2;
				drawBoard();
			}
			return;
		}
		if(msg.x > redoBtn.left && msg.x < redoBtn.right &&
			msg.y > redoBtn.top && msg.y < redoBtn.bottom)
		{
			if(!history.empty() && step + 1 < history.size())
			{
				step++;
				cover[history[step].first][history[step].second] = step % 2;
				::count = (step + 1) % 2;
				drawBoard();
			}
			return;
		}
		if(msg.x > newGameBtn.left && msg.x < newGameBtn.right &&
			msg.y > newGameBtn.top && msg.y < newGameBtn.bottom)
		{
			gameover = false;
			::count = 0;
			cover = std::vector<std::vector<int>>(15, std::vector<int>(15, 2));
			history.clear();
			step = -1;
			drawBoard();
			return;
		}

		// 原棋盘点击处理
		if(::count == 0)
		{
			int dis = INT_MAX, targetx, targety;
			for(int xpos = radius; xpos <= boxsize + radius; xpos += unitsize)
			{
				for(int ypos = radius; ypos <= boxsize + radius; ypos += unitsize)
				{
					int curdis = int(pow(msg.x - xpos, 2)) + int(pow(msg.y - ypos, 2));
					if(curdis < dis)
					{
						dis = curdis;
						targetx = int((xpos - radius) / unitsize);
						targety = int((ypos - radius) / unitsize);
					}
				}
			}
			if(cover[targetx][targety] != 2) return;

			cover[targetx][targety] = ::count;
			if(::count == 0 && (checkthree(targetx, targety) || checkfour(targetx, targety) || checklong(targetx, targety)))
			{
				HWND hwnd = GetHWnd();
				SetWindowText(hwnd, _T("��ʾ"));
				if(checkthree(targetx, targety) || checkfour(targetx, targety) || checklong(targetx, targety))
					MessageBox(hwnd, _T("��������ᴥ������"), _T("��ʾ"), MB_OKCANCEL);
				cover[targetx][targety] = 2;
				return;
			}
			history.push_back(std::make_pair(targetx, targety));
			step++;
			Chess chess(targetx, targety, Color(::count));
			chess.drawChess();
			::count = (::count + 1) % 2;
			if(chess.checkfive())
			{
				Sleep(2000);
				robothumangamestart = false;
				gameover = true;
			}
		}
	}
}

int main()
{
	initgraph(scenesize, scenesize);
	showStartScreen();
	drawBoard();
	ExMessage msg;

	while(!gameover)
	{
		if(robothumangamestart || robotrobotgamestart)
		{
			while(peekmessage(&msg, EM_MOUSE))
			{
				chechClick(msg);
			}

			// AI自动移动逻辑
			if(robothumangamestart && ::count == 1)
			{
				aimove();
				drawBoard();
			}
			else if(robotrobotgamestart)
			{
				automatch();
				drawBoard();
			}
		}
		Sleep(10);
	}
	showGameOver();
	return 0;
}