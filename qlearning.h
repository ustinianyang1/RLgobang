#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include "global.h"

// 定义状态 - 动作对的类型
using StateActionPair = std::pair<std::vector<std::vector<int>>, int>;

// 自定义哈希函数
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

// 定义 QTable 为哈希表
using QTable = std::unordered_map<StateActionPair, double, StateActionHash>;

// 初始化 QTable
QTable initializeQTable();

// 根据状态和动作获取 Q 值
double getQValue(const QTable &qTable, const std::vector<std::vector<int>> &state, int action);

// 更新 Q 值
void updateQValue(QTable &qTable, const std::vector<std::vector<int>> &state, int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate, double discountFactor);

// 保存 QTable 到文件
void saveQTable(const QTable &qTable, const std::string &filename);

// 从文件加载 QTable
QTable loadQTable(const std::string &filename);