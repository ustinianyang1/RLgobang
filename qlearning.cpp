#include "qlearning.h"
#include "chess.h"
#include <fstream>
#include <random>
#include <algorithm>
#include <cmath>

QTable initQTable()
{
    return QTable();
}

// 记录训练进度
void saveTrainingProgress(int episode)
{
    std::ofstream progressFile("training_progress.txt");
    if(progressFile.is_open())
    {
        progressFile << episode;
        progressFile.close();
    }
}

// 读取训练进度
int loadTrainingProgress()
{
    std::ifstream progressFile("training_progress.txt");
    int episode = 0;
    if(progressFile.is_open())
    {
        progressFile >> episode;
        progressFile.close();
    }
    return episode;
}

double getQValue(const QTable &table, const std::vector<std::vector<int>> &state, int action)
{
    auto key = std::make_pair(state, action);
    auto it = table.find(key);
    return (it != table.end()) ? it->second : 0.0;
}

void updateQValue(QTable &table, const std::vector<std::vector<int>> &state,
    int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate)
{
    double maxNextQ = 0.0;
    for(int a = 0; a < BOARD_SIZE * BOARD_SIZE; ++a)
    {
        maxNextQ = max(maxNextQ, getQValue(table, nextState, a));
    }

    QKey key = {state, action};
    double currentQ = getQValue(table, state, action);
    double newQ = currentQ + learningRate * (reward + DISCOUNT_FACTOR * maxNextQ - currentQ);
    table[key] = newQ;

    if(replay_buffer.size() >= REPLAY_BUFFER_SIZE)
    {
        replay_buffer.pop_front();
    }
    replay_buffer.emplace_back(state, action, reward, nextState);
}

void saveQTable(const QTable &table, const std::string &path)
{
    std::ofstream file(path, std::ios::binary);
    for(const auto &[key, value] : table)
    {
        const auto &[state, action] = key;

        for(const auto &row : state)
        {
            for(int val : row) file.write((char *)&val, sizeof(int));
        }

        file.write((char *)&action, sizeof(int));
        file.write((char *)&value, sizeof(double));
    }
}

QTable loadQTable(const std::string &path)
{
    QTable table;
    std::ifstream file(path, std::ios::binary);
    while(file)
    {
        std::vector<std::vector<int>> state(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
        for(auto &row : state)
        {
            for(int &val : row) file.read((char *)&val, sizeof(int));
        }
        int action;
        double value;
        file.read((char *)&action, sizeof(int));
        file.read((char *)&value, sizeof(double));
        table[{state, action}] = value;
    }
    return table;
}

double getIntermediateReward(int x, int y, int player)
{
    double reward = 0.0;
    const int dx[] = {1, 0, 1, 1};
    const int dy[] = {0, 1, 1, -1};

    for(int dir = 0; dir < 4; dir++)
    {
        int count = 1;
        int emptyLeft = 0;
        int emptyRight = 0;

        // 检查右侧
        for(int step = 1; ; step++)
        {
            int nx = x + dx[dir] * step;
            int ny = y + dy[dir] * step;
            if(nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) break;
            if(cover[nx][ny] == player)
            {
                count++;
            }
            else if(cover[nx][ny] == 2)
            {
                emptyRight++;
                break;
            }
            else
            {
                break;
            }
        }

        // 检查左侧
        for(int step = 1; ; step++)
        {
            int nx = x - dx[dir] * step;
            int ny = y - dy[dir] * step;
            if(nx < 0 || ny < 0 || nx >= BOARD_SIZE || ny >= BOARD_SIZE) break;
            if(cover[nx][ny] == player)
            {
                count++;
            }
            else if(cover[nx][ny] == 2)
            {
                emptyLeft++;
                break;
            }
            else
            {
                break;
            }
        }

        // 根据连子情况给予奖励
        if(count == 2 && emptyLeft > 0 && emptyRight > 0)
        {
            reward += LIVE_TWO_REWARD;
        }
        else if(count == 3 && emptyLeft > 0 && emptyRight > 0)
        {
            reward += LIVE_THREE_REWARD;
        }
        else if(count == 4 && (emptyLeft > 0 || emptyRight > 0))
        {
            reward += LIVE_FOUR_REWARD;
        }
    }

    return reward;
}

void trainQLearning(QTable &qtable, int episodes, int startEpisode = 0)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randProb(0.0, 1.0);

    for(int ep = startEpisode; ep < episodes; ++ep)
    {
        gameover = false;
        std::fill(cover.begin(), cover.end(), std::vector<int>(BOARD_SIZE, 2));
        history.clear();
        current_player = 0;

        double epsilon = EPSILON_END + (EPSILON_START - EPSILON_END) * std::exp(-ep / (episodes * EPSILON_DECAY));

        while(!gameover)
        {
            std::vector<std::vector<int>> currentState = cover;
            int action = -1;

            if(randProb(gen) < epsilon)
            {
                std::vector<std::pair<int, int>> validMoves;
                for(int x = 0; x < BOARD_SIZE; ++x)
                {
                    for(int y = 0; y < BOARD_SIZE; ++y)
                    {
                        if(cover[x][y] == 2) validMoves.emplace_back(x, y);
                    }
                }
                if(!validMoves.empty())
                {
                    std::shuffle(validMoves.begin(), validMoves.end(), gen);
                    auto [x, y] = validMoves.front();
                    action = x * BOARD_SIZE + y;
                }
            }
            else
            {
                Node node = minimax(SEARCH_DEPTH, current_player, INT_MIN, INT_MAX);
                action = node.x * BOARD_SIZE + node.y;
            }

            if(action != -1 && action != -16)
            {
                int x = action / BOARD_SIZE, y = action % BOARD_SIZE;
                if(current_player == 0 && (checkthree(x, y) || checkfour(x, y) || checklong(x, y)))
                {
                    continue;
                }
                cover[x][y] = current_player;
                history.emplace_back(x, y);

                double reward = 0.0;
                // 添加中心奖励
                int center = BOARD_SIZE / 2;
                if(x == center && y == center)
                {
                    reward += CENTER_REWARD;
                }
                if(checkWin(x, y))
                {
                    reward = (current_player == 0) ? 1.0 : -1.0;
                    gameover = true;
                }
                else
                {
                    reward += getIntermediateReward(x, y, current_player);
                }
                // 计算学习率
                double learningRate = LEARNING_RATE_END + (LEARNING_RATE_START - LEARNING_RATE_END) * std::exp(-ep / (episodes * LEARNING_RATE_DECAY));
                updateQValue(qtable, currentState, action, reward, cover, learningRate);

                current_player = 1 - current_player;

                drawBoard();
                FlushBatchDraw();
                Sleep(TrainSleepTime);
            }
        }

        if(ep % 10 == 9)
        {
            saveQTable(qtable, "qtable_ep" + std::to_string(ep) + ".bin");
            saveTrainingProgress(ep + 1); // 保存训练进度
        }
    }
}