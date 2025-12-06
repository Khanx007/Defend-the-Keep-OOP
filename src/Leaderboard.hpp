// Leaderboard.hpp
#pragma once

#include <vector>
#include <string>

struct HighScore {
    int waves;
    int kills;
    int defenses;
};

std::vector<HighScore> loadLeaderboard(const std::string& filename);
void saveLeaderboard(const std::string& filename, std::vector<HighScore>& scores);
