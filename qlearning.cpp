#include "qlearning.h"
#include "global.h"
#include <cmath>
#include <iostream>

// ��ʼ�� QTable
QTable initializeQTable()
{
    return QTable();
}

// ����״̬�Ͷ�����ȡ Q ֵ
double getQValue(const QTable &qTable, const std::vector<std::vector<int>> &state, int action)
{
    auto it = qTable.find({state, action});
    if(it != qTable.end())
    {
        return it->second;
    }
    return 0.0;
}

// ���� Q ֵ
void updateQValue(QTable &qTable, const std::vector<std::vector<int>> &state, int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate, double discountFactor)
{
    double maxQNext = 0.0;
    for(int a = 0; a < BOARD_SIZE * BOARD_SIZE; ++a)
    {
        double qNext = getQValue(qTable, nextState, a);
        if(qNext > maxQNext)
        {
            maxQNext = qNext;
        }
    }

QTable loadQTable(const std::string &path)
{
    QTable table;
    std::ifstream file(path, std::ios::binary);
    if(!file.is_open())
    {
        std::cerr << "Failed to open Q-table file." << std::endl;
        return QTable();
    }
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

// ���� QTable ���ļ�
void saveQTable(const QTable &qTable, const std::string &filename)
{
    #ifdef TRAINING_MODE
    #endif
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> randProb(0.0, 1.0);

    for(int ep = 0; ep < episodes; ++ep)
    {
        gameover = false;
        // ��ʼ������
        std::fill(cover.begin(), cover.end(), std::vector<int>(BOARD_SIZE, 2));
        history.clear();
        current_player = 0;

        // ��̬����̽����
        double epsilon = EPSILON_END + (EPSILON_START - EPSILON_END) * std::exp(-ep / (episodes * EPSILON_DECAY));

        while(!gameover)
        {
            std::vector<std::vector<int>> currentState = cover;
            int action = -1;

            // epsilon - greedy����
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

// ���ļ����� QTable
QTable loadQTable(const std::string &filename)
{
    QTable qTable;
    std::ifstream file(filename);
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            std::stringstream ss(line);
            std::vector<std::vector<int>> state(BOARD_SIZE, std::vector<int>(BOARD_SIZE));
            for(int i = 0; i < BOARD_SIZE; ++i)
            {
                for(int j = 0; j < BOARD_SIZE; ++j)
                {
                    ss >> state[i][j];
                }

                // ����Qֵ
                updateQValue(qtable, currentState, action, reward, cover);

                // �л����
                current_player = 1 - current_player;

                // ���½���
                drawBoard();
                FlushBatchDraw();
                Sleep(200); //���������ٶ�
            }
            int action;
            double value;
            ss >> action >> value;
            qTable[{state, action}] = value;
        }
        file.close();
    }
    return qTable;
}