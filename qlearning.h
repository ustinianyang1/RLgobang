#pragma once
#include <vector>
#include <fstream>
#include <string>
#include "global.h"

// ����Q��
using QTable = std::vector<std::vector<double>>;

// ��ʼ��Q��
QTable initializeQTable();

// ����״̬�Ͷ�����ȡQֵ
double getQValue(const QTable &qTable, const std::vector<std::vector<int>> &state, int action);

// ����Qֵ
void updateQValue(QTable &qTable, const std::vector<std::vector<int>> &state, int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate, double discountFactor);

// ����Q���ļ�
void saveQTable(const QTable &qTable, const std::string &filename);

// ����Q����ļ�
QTable loadQTable(const std::string &filename);