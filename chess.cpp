#include "chess.h"
#include "global.h"
#include "qlearning.h"
#include <graphics.h>
#include <sstream>
#include <cmath>
#include <climits> 
#include <algorithm>
#include <fstream>
#include <random> 
#include <queue>

#define BLACK RGB(0, 0, 0)
#define WHITE RGB(255, 255, 255)

void Chess::draw() const
{
    setfillcolor(color == Color::Black ? BLACK : WHITE);
    setlinecolor(color == Color::Black ? RGB(50, 50, 50) : RGB(200, 200, 200));
    solidcircle(x * UNIT_SIZE + RADIUS, y * UNIT_SIZE + RADIUS, RADIUS - 5);
}

bool checkWin(int x, int y)
{
    int playerColor = cover[x][y];
    if(playerColor == 2)
        return false;

    const int dx[] = {1, 0, 1, 1};
    const int dy[] = {0, 1, 1, -1};

    for(int dir = 0; dir < 4; dir++)
    {
        int count = 1;
        for(int step = 1; ; step++)
        {
            int nx = x + dx[dir] * step;
            int ny = y + dy[dir] * step;
            if(nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) break;
            if(cover[nx][ny] != playerColor) break;
            count++;
        }
        for(int step = 1; ; step++)
        {
            int nx = x - dx[dir] * step;
            int ny = y - dy[dir] * step;
            if(nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) break;
            if(cover[nx][ny] != playerColor) break;
            count++;
        }
        if(count >= 5) return true;
    }
    return false;
}

void initScore(std::vector<std::vector<int>> &score)
{
    const int dx[] = {1, 1, 1, 0, 0, -1, -1, -1};
    const int dy[] = {0, -1, 1, 1, -1, -1, 0, 1};

    std::queue<std::pair<int, int>> q;
    std::vector<std::vector<bool>> visited(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false));

    int center = BOARD_SIZE / 2;
    score[center][center] = center;
    q.push({center, center});
    visited[center][center] = true;

    while(!q.empty())
    {
        auto [x, y] = q.front();
        q.pop();

        for(int dir = 0; dir < 8; dir++)
        {
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if(nx >= 0 && ny >= 0 && nx < BOARD_SIZE && ny < BOARD_SIZE && !visited[nx][ny])
            {
                score[nx][ny] = max(0, score[x][y] - 1);
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
}

void updateScores(int lastX, int lastY)
{
    int startX = max(0, lastX - UPDATE_RADIUS);
    int endX = min(BOARD_SIZE - 1, lastX + UPDATE_RADIUS);
    int startY = max(0, lastY - UPDATE_RADIUS);
    int endY = min(BOARD_SIZE - 1, lastY + UPDATE_RADIUS);

    initScore(blackscore);
    initScore(whitescore);

    for(int x = startX; x <= endX; x++)
    {
        for(int y = startY; y <= endY; y++)
        {
            if(cover[x][y] != 2) continue;

            for(int player = 0; player <= 1; player++)
            {
                int score = 0;
                const int dx[] = {1, 0, 1, 1};
                const int dy[] = {0, 1, 1, -1};

                for(int dir = 0; dir < 4; dir++)
                {
                    int countright = 0;
                    int blockright = 0;
                    int emptyright = 0;
                    for(int step = 1; step <= 5; step++)
                    {
                        int nx = x + step * dx[dir];
                        int ny = y + step * dy[dir];
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
                        int nx = x - step * dx[dir];
                        int ny = y - step * dy[dir];
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
                    if(countleft == 4 && emptyleft == 1)
                        score += LIVE_FOUR;
                    if(countright == 4 && emptyleft == 1)
                        score += LIVE_FOUR;

                    if(countright == 3 && countleft == 1)
                        score += SPECIAL_FIVE;
                    else if(countright == 2 && countleft == 2)
                        score += SPECIAL_FIVE;
                    else if(countright == 1 && countleft == 3)
                        score += SPECIAL_FIVE;

                    if(countleft == 4 && blockleft == 1)
                        score += DEAD_FOUR;
                    if(countright == 4 && blockright == 1)
                        score += DEAD_FOUR;

                    if(countright == 2 && countleft == 1 && emptyleft == 1 && emptyright == 1)
                        score += LIVE_SPECIAL_FOUR;
                    else if(countright == 1 && countleft == 2 && emptyleft == 1 && emptyright == 1)
                        score += LIVE_SPECIAL_FOUR;

                    if(countright == 2 && countleft == 1 && !(emptyleft == 1 && emptyright == 1))
                        score += DEAD_SPECIAL_FOUR;
                    else if(countright == 1 && countleft == 2 && !(emptyleft == 1 && emptyright == 1))
                        score += DEAD_SPECIAL_FOUR;

                    if(countleft == 3 && emptyleft == 1)
                        score += LIVE_THREE / (countright == 0 && blockright == 1 ? 5 : 1);
                    if(countright == 3 && emptyright == 1)
                        score += LIVE_THREE / (countleft == 0 && blockleft == 1 ? 5 : 1);

                    if(countleft == 3 && blockleft == 1 && !(countright == 0 && blockright == 1))
                        score += DEAD_THREE;
                    if(countright == 3 && blockright == 1 && !(countleft == 0 && blockleft == 1))
                        score += DEAD_THREE;

                    if(countright == 1 && countleft == 1 && emptyleft == 1 && emptyright == 1)
                        score += LIVE_SPECIAL_THREE;

                    if(countright == 1 && countleft == 1 && !(emptyleft == 1 && emptyright == 1))
                        score += DEAD_SPECIAL_THREE;

                    if(countleft == 2 && emptyleft == 1)
                        score += LIVE_TWO / (countright == 0 && blockright == 1 ? 5 : 1);
                    if(countright == 2 && emptyright == 1 && countleft != 0)
                        score += LIVE_TWO / (countleft == 0 && blockleft == 1 ? 5 : 1);

                    if(countleft == 2 && blockleft == 1 && !(countright == 0 && blockright == 1))
                        score += DEAD_TWO;
                    if(countright == 2 && blockright == 1 && !(countleft == 0 && blockleft == 1))
                        score += DEAD_TWO;

                    if(countleft == 1 && emptyleft == 1)
                        score += LIVE_ONE / (countright == 0 && blockright == 1 ? 5 : 1);
                    if(countright == 1 && emptyright == 1)
                        score += LIVE_ONE / (countleft == 0 && blockleft == 1 ? 5 : 1);

                    if(countleft == 1 && blockleft == 1 && !(countright == 0 && blockright == 1))
                        score += DEAD_ONE;
                    if(countright == 1 && blockright == 1 && !(countleft == 0 && blockleft == 1))
                        score += DEAD_ONE;
                }

                (player == 0 ? blackscore : whitescore)[x][y] = score;
            }
        }
    }
}

Node minimax(int depth, int player, int alpha, int beta)
{
    if(depth == 0 || gameover)
    {
        std::vector<std::vector<int>> currentState = cover;
        double maxQ = -std::numeric_limits<double>::infinity();
        int bestAction = -1;
        for(int x = 0; x < BOARD_SIZE; ++x)
        {
            for(int y = 0; y < BOARD_SIZE; ++y)
            {
                if(cover[x][y] == 2)
                {
                    int action = x * BOARD_SIZE + y;
                    double qValue = getQValue(qTable, currentState, action);
                    if(qValue > maxQ)
                    {
                        maxQ = qValue;
                        bestAction = action;
                    }
                }
            }
        }
        int bestX = bestAction / BOARD_SIZE;
        int bestY = bestAction % BOARD_SIZE;
        return Node(maxQ, bestX, bestY);
    }

    std::vector<std::pair<int, int>> moves;
    for(int x = 0; x < BOARD_SIZE; ++x)
    {
        for(int y = 0; y < BOARD_SIZE; ++y)
        {
            if(cover[x][y] == 2)
            {
                if(player == 0 && (checkthree(x, y) || checkfour(x, y) || checklong(x, y)))
                {
                    continue;
                }
                moves.emplace_back(x, y);
            }
        }
    }
    std::sort(moves.begin(), moves.end(), [&](auto a, auto b)
        {
            return historyHeuristic[a.first][a.second] > historyHeuristic[b.first][b.second];
        });

    Node best(player == 0 ? -std::numeric_limits<int>::infinity() : std::numeric_limits<int>::infinity(), -1, -1);

    for(auto [x, y] : moves)
    {
        cover[x][y] = player;
        Node current = minimax(depth - 1, 1 - player, alpha, beta);
        cover[x][y] = 2;

        if(player == 0)
        {
            if(current.score > best.score)
            {
                best = current;
                alpha = max(alpha, current.score);
            }
            if(alpha >= beta) break;
        }
        else
        {
            if(current.score < best.score)
            {
                best = current;
                beta = min(beta, current.score);
            }
            if(beta <= alpha) break;
        }
    }
    return best;
}

void drawBoard()
{
    cleardevice();

    
    setlinecolor(RGB(139, 69, 19));
    setlinestyle(PS_SOLID, 2);
    for(int i = 0; i <= BOARD_SIZE; i++)
    {
        line(RADIUS + i * UNIT_SIZE, RADIUS,
            RADIUS + i * UNIT_SIZE, RADIUS + BOXSIZE);
        line(RADIUS, RADIUS + i * UNIT_SIZE,
            RADIUS + BOXSIZE, RADIUS + i * UNIT_SIZE);
    }

    
    for(int x = 0; x < BOARD_SIZE; x++)
    {
        for(int y = 0; y < BOARD_SIZE; y++)
        {
            if(cover[x][y] != 2)
            {
                Chess(x, y, (Color)cover[x][y]).draw();
            }
        }
    }

    
    setfillcolor(RGB(245, 245, 220));
    solidrectangle(BOXSIZE + 2 * RADIUS, 0, SCENESIZE, SCENESIZE);

    
    auto drawButton = [](const GameButton &btn, bool hover)
        {
            setfillcolor(hover ? BTN_HOVER_COLOR : BTN_COLOR);
            solidroundrect(btn.left, btn.top, btn.right, btn.bottom, 10, 10);
            settextcolor(WHITE);
            setbkmode(TRANSPARENT);
            settextstyle(20, 0, _T("微软雅黑"));
            int textWidth = textwidth(btn.text);
            int textHeight = textheight(btn.text);
            outtextxy(
                (btn.left + btn.right - textWidth) / 2,
                (btn.top + btn.bottom - textHeight) / 2,
                btn.text
            );
        };

    drawButton(undoBtn, false);
    drawButton(redoBtn, false);
    drawButton(newGameBtn, false);
    
    settextcolor(BLACK);
    settextstyle(25, 0, _T("微软雅黑"));
    std::wstring status = current_player == 0 ? L"黑方回合" : L"白方回合";
    outtextxy(SCENESIZE - 150, SCENESIZE - 100, status.c_str());
}

void showThinkingStatus(const std::wstring &msg)
{
    
    setfillcolor(RGB(245, 245, 220));
    solidrectangle(BOXSIZE + 2 * RADIUS, SCENESIZE - 150, SCENESIZE, SCENESIZE - 120);

    settextcolor(RGB(255, 0, 0));
    settextstyle(20, 0, _T("΢���ź�"));
    int textWidth = textwidth(msg.c_str());
    outtextxy(SCENESIZE - 30 - textWidth, SCENESIZE - 150, msg.c_str());

    FlushBatchDraw();
}

void handleClick(int targetX, int targetY)
{
    if(current_player == 0)
    {
        if(targetX >= 0 && targetX < BOARD_SIZE &&
            targetY >= 0 && targetY < BOARD_SIZE &&
            cover[targetX][targetY] == 2)
        {
            if(checkthree(targetX, targetY) || checkfour(targetX, targetY) || checklong(targetX, targetY))
            {
                return;
            }
            cover[targetX][targetY] = current_player;
            history.emplace_back(targetX, targetY);
            step_count++;

            Chess chess(targetX, targetY, (Color)current_player);
            chess.draw();

            if(checkWin(targetX, targetY))
            {
                gameover = true;
            }
            else
            {
                current_player = 1 - current_player;
                updateScores(targetX, targetY);
            }

            drawBoard();
            FlushBatchDraw();
        }
    }
}

void aiMove(QTable &qtable)
{
    showThinkingStatus(L"AI思考中……");
    drawBoard(); // 添加棋盘绘制
    FlushBatchDraw(); // 强制刷新

    int lastX = -1, lastY = -1;
    if(!history.empty())
    {
        lastX = history.back().first;
        lastY = history.back().second;
    }
    updateScores(lastX, lastY);

    Node target = minimax(SEARCH_DEPTH, current_player, INT_MIN, INT_MAX);

    showThinkingStatus(L"");

    if(target.x == -1 || target.y == -1)
    {
        int targetX = (BOARD_SIZE - 1) / 2, targetY = (BOARD_SIZE - 1) / 2;
        if(current_player == 0)
        {
            for(int x = 0; x < BOARD_SIZE; x++)
            {
                for(int y = 0; y < BOARD_SIZE; y++)
                {
                    if(cover[x][y] == 2)
                    {
                        cover[x][y] = 0;
                        if(checkthree(x, y) || checkfour(x, y) || checklong(x, y))
                            blackscore[x][y] = -1;
                        cover[x][y] = 2;
                    }
                }
            }
        }

        int highestScore = INT_MIN;
        for(int x = 0; x < BOARD_SIZE; x++)
        {
            for(int y = 0; y < BOARD_SIZE; y++)
            {
                if(cover[x][y] == 2)
                {
                    if(current_player == 0 && blackscore[x][y] == -1)
                        continue;
                    int curScore = (current_player == 0 ? blackscore[x][y] : whitescore[x][y]) * (current_player == 0 ? ATTACK_BLACK : ATTACK_WHITE) + (current_player == 0 ? whitescore[x][y] : blackscore[x][y]);
                    if(curScore > highestScore)
                    {
                        highestScore = curScore;
                        targetX = x;
                        targetY = y;
                    }
                }
            }
        }

        if(highestScore == INT_MIN)
            std::system("pause");
        target.x = targetX;
        target.y = targetY;
    }

    cover[target.x][target.y] = current_player;
    history.emplace_back(target.x, target.y);
    step_count++;

    Chess chess(target.x, target.y, (Color)current_player);
    chess.draw();

    if(checkWin(target.x, target.y))
    {
        gameover = true;
    }
    else
    {
        current_player = 1 - current_player;
        updateScores(target.x, target.y);
    }

    drawBoard();
    FlushBatchDraw();
}

void saveGame(const std::string &path)
{
    std::ofstream file(path, std::ios::binary);
    for(const auto &row : cover)
    {
        for(int val : row)
        {
            file.write((char *)&val, sizeof(int));
        }
    }
    file.write((char *)&current_player, sizeof(int));
    file.write((char *)&step_count, sizeof(int));
    for(const auto &[x, y] : history)
    {
        file.write((char *)&x, sizeof(int));
        file.write((char *)&y, sizeof(int));
    }
    saveQTable(qTable, "qtable_save.bin");
}

void loadGame(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    for(auto &row : cover)
    {
        for(int &val : row)
        {
            file.read((char *)&val, sizeof(int));
        }
    }
    file.read((char *)&current_player, sizeof(int));
    file.read((char *)&step_count, sizeof(int));
    history.clear();
    for(int i = 0; i <= step_count; ++i)
    {
        int x, y;
        file.read((char *)&x, sizeof(int));
        file.read((char *)&y, sizeof(int));
        history.emplace_back(x, y);
    }
    qTable = loadQTable("qtable_save.bin");
    drawBoard();
    FlushBatchDraw();
}

bool checkthree(int &targetx, int &targety)
{
    if(current_player != 0) 
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

bool checkthreespecial(int &targetx, int &targety)
{
    if(current_player != 0) 
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

bool checkfour(int &targetx, int &targety)
{
    if(current_player != 0) 
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

bool checklong(int &targetx, int &targety)
{
    if(current_player != 0) 
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