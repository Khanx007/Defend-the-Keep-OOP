#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class AudioManager {
public:
    // Load a short sound buffer into memory (SFX)
    static void loadSFX(const std::string& id, const std::string& filename);

    // Play a previously loaded SFX (uses internal pool)
    static void playSFX(const std::string& id, float volume = 100.f);

    // Streamed background music: returns true on success
    // Signature: (filename, loop, volume)
    static bool playBGM(const std::string& filename, bool loop = true, float volume = 60.f);
    static void stopBGM();

    // Optional second streamed track for ambience/background layers
    static bool playAmbience(const std::string& filename, bool loop = true, float volume = 50.f);
    static void stopAmbience();

    // Loop a short SFX buffer (useful for short ambient loops)
    static void loopSFX(const std::string& id, float volume = 50.f);
    static void stopLoopSFX();

private:
    // storage for loaded short sounds (SFX)
    static std::unordered_map<std::string, sf::SoundBuffer> _sfx;

    // pool of sf::Sound objects for overlapping playback
    static std::vector<sf::Sound> _players;

    // streamed music objects (do not store whole file in RAM)
    static sf::Music _bgm;
    static sf::Music _ambience;

    // dedicated player for looping a short sound (if needed)
    static sf::Sound _loopingSfxPlayer;
};
