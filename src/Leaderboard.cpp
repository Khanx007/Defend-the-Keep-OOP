// Leaderboard.cpp
#include "Leaderboard.hpp"
#include <fstream>
#include <algorithm>

// Sorts by wave (descending), then kills (descending)
static bool compareScores(const HighScore& a, const HighScore& b) {
    if (a.waves != b.waves) return a.waves > b.waves;
    return a.kills > b.kills;
}

std::vector<HighScore> loadLeaderboard(const std::string& filename) {
    std::vector<HighScore> scores;
    std::ifstream file(filename);
    if (file.is_open()) {
        HighScore temp;
        while (file >> temp.waves >> temp.kills >> temp.defenses) {
            scores.push_back(temp);
        }
        file.close();
    }
    return scores;
}

void saveLeaderboard(const std::string& filename, std::vector<HighScore>& scores) {
    // 1. Sort
    std::sort(scores.begin(), scores.end(), compareScores);
    
    // 2. Keep only top 5
    if (scores.size() > 5) {
        scores.resize(5);
    }

    // 3. Save to file
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& s : scores) {
            file << s.waves << " " << s.kills << " " << s.defenses << "\n";
        }
        file.close();
    }
}
