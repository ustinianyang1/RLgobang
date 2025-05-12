#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "global.h"

// 定义Q表
using QTable = std::vector<std::vector<double>>;

// 初始化Q表
QTable initializeQTable();

// 根据状态和动作获取Q值
double getQValue(const QTable &qTable, const std::vector<std::vector<int>> &state, int action);

// 更新Q值
void updateQValue(QTable &qTable, const std::vector<std::vector<int>> &state, int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate, double discountFactor);

// 保存Q表到文件
void saveQTable(const QTable &qTable, const std::string &filename);

// 加载Q表从文件
QTable loadQTable(const std::string &filename);