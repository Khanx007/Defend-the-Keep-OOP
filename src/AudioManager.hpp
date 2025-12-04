#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class AudioManager {
public:
    static void loadSFX(const std::string& id, const std::string& filename);
    static void playSFX(const std::string& id, float volume = 80.f);

    static void playBGM(const std::string& filename, bool loop = true, float volume = 50.f);
    static void stopBGM();
};
