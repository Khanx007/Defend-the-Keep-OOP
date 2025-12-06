#include "AudioManager.hpp"
#include <iostream>

// static member definitions
std::unordered_map<std::string, sf::SoundBuffer> AudioManager::_sfx;
std::vector<sf::Sound> AudioManager::_players;
sf::Music AudioManager::_bgm;
sf::Music AudioManager::_ambience;
sf::Sound AudioManager::_loopingSfxPlayer;

// Load a sound buffer into the map
void AudioManager::loadSFX(const std::string& id, const std::string& filename) {
    sf::SoundBuffer buf;
    if (!buf.loadFromFile(filename)) {
        std::cerr << "[AudioManager] failed to load sfx: " << filename << "\n";
        return;
    }
    _sfx.emplace(id, std::move(buf));
    if (_players.size() < 8) _players.resize(8);
}

// Play a one-shot SFX using the pool
void AudioManager::playSFX(const std::string& id, float volume) {
    auto it = _sfx.find(id);
    if (it == _sfx.end()) return;

    for (auto &p : _players) {
        if (p.getStatus() != sf::Sound::Playing) {
            p.setBuffer(it->second);
            p.setVolume(volume);
            p.play();
            return;
        }
    }

    if (!_players.empty()) {
        _players[0].setBuffer(it->second);
        _players[0].setVolume(volume);
        _players[0].play();
    }
}

// Play background music (streamed). Signature: (filename, loop, volume)
bool AudioManager::playBGM(const std::string& filename, bool loop, float volume) {
    if (!_bgm.openFromFile(filename)) {
        std::cerr << "[AudioManager] Failed to open BGM: " << filename << "\n";
        return false;
    }
    _bgm.setLoop(loop);
    _bgm.setVolume(volume);
    _bgm.play();
    return true;
}

void AudioManager::stopBGM() {
    if (_bgm.getStatus() == sf::Music::Playing) _bgm.stop();
}

// Play ambience (second streamed track) - optional
bool AudioManager::playAmbience(const std::string& filename, bool loop, float volume) {
    if (!_ambience.openFromFile(filename)) {
        std::cerr << "[AudioManager] Failed to open ambience: " << filename << "\n";
        return false;
    }
    _ambience.setLoop(loop);
    _ambience.setVolume(volume);
    _ambience.play();
    return true;
}

void AudioManager::stopAmbience() {
    if (_ambience.getStatus() == sf::Music::Playing) _ambience.stop();
}

// Loop a short SFX buffer (if you loaded it via loadSFX)
void AudioManager::loopSFX(const std::string& id, float volume) {
    auto it = _sfx.find(id);
    if (it == _sfx.end()) return;
    _loopingSfxPlayer.setBuffer(it->second);
    _loopingSfxPlayer.setLoop(true);
    _loopingSfxPlayer.setVolume(volume);
    _loopingSfxPlayer.play();
}

void AudioManager::stopLoopSFX() {
    if (_loopingSfxPlayer.getStatus() == sf::Sound::Playing) _loopingSfxPlayer.stop();
}

