#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include "global.h"

// ����״̬ - �����Ե�����
using StateActionPair = std::pair<std::vector<std::vector<int>>, int>;

// �Զ����ϣ����
struct StateActionHash
{
    size_t operator()(const StateActionPair &pair) const
    {
        size_t seed = pair.second;
        for(const auto &row : pair.first)
        {
            for(int val : row)
            {
                seed ^= val + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
        }
        return seed;
    }
};

// ���� QTable Ϊ��ϣ��
using QTable = std::unordered_map<StateActionPair, double, StateActionHash>;

// ��ʼ�� QTable
QTable initializeQTable();

// ����״̬�Ͷ�����ȡ Q ֵ
double getQValue(const QTable &qTable, const std::vector<std::vector<int>> &state, int action);

// ���� Q ֵ
void updateQValue(QTable &qTable, const std::vector<std::vector<int>> &state, int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate, double discountFactor);

// ���� QTable ���ļ�
void saveQTable(const QTable &qTable, const std::string &filename);

// ���ļ����� QTable
QTable loadQTable(const std::string &filename);