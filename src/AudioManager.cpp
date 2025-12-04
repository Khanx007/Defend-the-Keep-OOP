#include "AudioManager.hpp"
#include <iostream>

static std::unordered_map<std::string, sf::SoundBuffer> _sfx;
static std::vector<sf::Sound> _players;
static sf::Music _bgm;

void AudioManager::loadSFX(const std::string& id, const std::string& filename) {
    sf::SoundBuffer buf;
    if (!buf.loadFromFile(filename)) {
        std::cerr << "[AudioManager] failed to load sfx: " << filename << "\n";
        return;
    }
    _sfx.emplace(id, std::move(buf));
    if (_players.size() < 8) _players.resize(8); // pool
}

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
    // all busy: reuse first
    _players[0].setBuffer(it->second);
    _players[0].setVolume(volume);
    _players[0].play();
}

void AudioManager::playBGM(const std::string& filename, bool loop, float volume) {
    if (!_bgm.openFromFile(filename)) {
        std::cerr << "[AudioManager] failed to open BGM: " << filename << "\n";
        return;
    }
    _bgm.setLoop(loop);
    _bgm.setVolume(volume);
    _bgm.play();
}

void AudioManager::stopBGM() {
    _bgm.stop();
}
