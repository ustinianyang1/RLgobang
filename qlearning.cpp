#include "qlearning.h"
#include "global.h"
#include <cmath>
#include <sstream>

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

    auto key = std::make_pair(state, action);
    qTable[key] += learningRate * (reward + discountFactor * maxQNext - qTable[key]);
}

// ���� QTable ���ļ�
void saveQTable(const QTable &qTable, const std::string &filename)
{
    std::ofstream file(filename);
    if(file.is_open())
    {
        for(const auto &pair : qTable)
        {
            const auto &state = pair.first.first;
            int action = pair.first.second;
            double value = pair.second;
            for(const auto &row : state)
            {
                for(int val : row)
                {
                    file << val << " ";
                }
            }
            file << action << " " << value << std::endl;
        }
        file.close();
    }
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