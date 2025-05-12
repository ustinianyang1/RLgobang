#include "qlearning.h"
#include "global.h"
#include <cmath>
#include <sstream>

// ��ʼ��Q��
QTable initializeQTable()
{
    int stateSize = std::pow(3, BOARD_SIZE * BOARD_SIZE); // ÿ��������3��״̬���ա��ڡ��ף�
    int actionSize = BOARD_SIZE * BOARD_SIZE;
    return QTable(stateSize, std::vector<double>(actionSize, 0.0));
}

// ��״̬ת��Ϊ��������
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

// ����״̬�Ͷ�����ȡQֵ
double getQValue(const QTable &qTable, const std::vector<std::vector<int>> &state, int action)
{
    int stateIndex = stateToIndex(state);
    return qTable[stateIndex][action];
}

// ����Qֵ
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

// ����Q���ļ�
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

// ����Q����ļ�
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