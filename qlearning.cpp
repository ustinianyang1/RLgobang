#include "qlearning.h"
#include "global.h"
#include <cmath>
#include <sstream>

// 初始化Q表
QTable initializeQTable()
{
    int stateSize = std::pow(3, BOARD_SIZE * BOARD_SIZE); // 每个格子有3种状态（空、黑、白）
    int actionSize = BOARD_SIZE * BOARD_SIZE;
    return QTable(stateSize, std::vector<double>(actionSize, 0.0));
}

// 将状态转换为整数索引
int stateToIndex(const std::vector<std::vector<int>> &state)
{
    int index = 0;
    int power = 1;
    for(int i = 0; i < BOARD_SIZE; ++i)
    {
        for(int j = 0; j < BOARD_SIZE; ++j)
        {
            index += state[i][j] * power;
            power *= 3;
        }
    }
    return index;
}

// 根据状态和动作获取Q值
double getQValue(const QTable &qTable, const std::vector<std::vector<int>> &state, int action)
{
    int stateIndex = stateToIndex(state);
    return qTable[stateIndex][action];
}

// 更新Q值
void updateQValue(QTable &qTable, const std::vector<std::vector<int>> &state, int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate, double discountFactor)
{
    int stateIndex = stateToIndex(state);
    int nextStateIndex = stateToIndex(nextState);

    double maxQNext = 0.0;
    for(int a = 0; a < BOARD_SIZE * BOARD_SIZE; ++a)
    {
        if(qTable[nextStateIndex][a] > maxQNext)
        {
            maxQNext = qTable[nextStateIndex][a];
        }
    }

    qTable[stateIndex][action] += learningRate * (reward + discountFactor * maxQNext - qTable[stateIndex][action]);
}

// 保存Q表到文件
void saveQTable(const QTable &qTable, const std::string &filename)
{
    std::ofstream file(filename);
    if(file.is_open())
    {
        for(const auto &row : qTable)
        {
            for(double value : row)
            {
                file << value << " ";
            }
            file << std::endl;
        }
        file.close();
    }
}

// 加载Q表从文件
QTable loadQTable(const std::string &filename)
{
    QTable qTable;
    std::ifstream file(filename);
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            std::vector<double> row;
            std::stringstream ss(line);
            double value;
            while(ss >> value)
            {
                row.push_back(value);
            }
            qTable.push_back(row);
        }
        file.close();
    }
    return qTable;
}