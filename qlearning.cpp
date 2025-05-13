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

double getQValue(const QTable &table, const std::vector<std::vector<int>> &state, int action)
{
    auto key = std::make_pair(state, action);
    auto it = table.find(key);
    return (it != table.end()) ? it->second : 0.0;
}

void updateQValue(QTable &table, const std::vector<std::vector<int>> &state,
    int action, double reward, const std::vector<std::vector<int>> &nextState)
{
    
    double maxNextQ = 0.0;
    for(int a = 0; a < BOARD_SIZE * BOARD_SIZE; ++a)
    {
        maxNextQ = max(maxNextQ, getQValue(table, nextState, a));
    }

    
    QKey key = {state, action};
    double currentQ = getQValue(table, state, action);
    double newQ = currentQ + LEARNING_RATE * (reward + DISCOUNT_FACTOR * maxNextQ - currentQ);
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

void trainQLearning(QTable &qtable, int episodes)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randProb(0.0, 1.0);

    for(int ep = 0; ep < episodes; ++ep)
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

            
            if(action != -1)
            {
                int x = action / BOARD_SIZE, y = action % BOARD_SIZE;
                cover[x][y] = current_player;
                history.emplace_back(x, y);

                
                double reward = 0.0;
                if(checkWin(x, y))
                {
                    reward = (current_player == 0) ? 1.0 : -1.0;
                    gameover = true;
                }

                
                updateQValue(qtable, currentState, action, reward, cover);

                
                current_player = 1 - current_player;

                
                drawBoard();
                FlushBatchDraw();
                Sleep(200); 
            }
        }

        
        if(ep % 10 == 9) saveQTable(qtable, "qtable_ep" + std::to_string(ep) + ".bin");
    }
}