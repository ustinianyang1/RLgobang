#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

using QKey = std::pair<std::vector<std::vector<int>>, int>;

namespace std
{
    template<>
    struct hash<std::vector<std::vector<int>>>
    {
        size_t operator()(const std::vector<std::vector<int>> &state) const noexcept
        {
            size_t seed = 0;
            for(const auto &row : state)
            {
                for(int val : row)
                {
                    seed ^= hash<int>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                }
            }
            return seed;
        }
    };

    template<>
    struct hash<QKey>
    {
        size_t operator()(const QKey &key) const noexcept
        {
            size_t h1 = hash<std::vector<std::vector<int>>>()(key.first);
            size_t h2 = hash<int>()(key.second);
            return h1 ^ (h2 << 1);
        }
    };
}

using QTable = std::unordered_map<QKey, double>;


QTable initQTable();
double getQValue(const QTable &table, const std::vector<std::vector<int>> &state, int action);
void updateQValue(QTable &table, const std::vector<std::vector<int>> &state,
    int action, double reward, const std::vector<std::vector<int>> &nextState, double learningRate);
double getIntermediateReward(int x, int y, int player);
void saveQTable(const QTable &table, const std::string &path);
QTable loadQTable(const std::string &path);
void trainQLearning(QTable &qtable, int episodes, int startEpisode);