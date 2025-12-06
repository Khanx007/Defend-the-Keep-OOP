#include "AssetManager.hpp"
#include <iostream>

static std::unordered_map<std::string, sf::Texture> _textures;
static std::unordered_map<std::string, sf::SoundBuffer> _soundBuffers;
static std::unordered_map<std::string, sf::Font> _fonts;

void AssetManager::loadTexture(const std::string& id, const std::string& filename) {
    sf::Texture tex;
    if (!tex.loadFromFile(filename)) {
        std::cerr << "[AssetManager] failed to load texture: " << filename << "\n";
        return;
    }
    tex.setSmooth(true);
    _textures.emplace(id, std::move(tex));
}

sf::Texture& AssetManager::getTexture(const std::string& id) {
    auto it = _textures.find(id);
    if (it == _textures.end()) {
        static sf::Texture dummy;
        return dummy;
    }
    return it->second;
}

void AssetManager::loadSoundBuffer(const std::string& id, const std::string& filename) {
    sf::SoundBuffer buf;
    if (!buf.loadFromFile(filename)) {
        std::cerr << "[AssetManager] failed to load sound: " << filename << "\n";
        return;
    }
    _soundBuffers.emplace(id, std::move(buf));
}

sf::SoundBuffer& AssetManager::getSoundBuffer(const std::string& id) {
    auto it = _soundBuffers.find(id);
    if (it == _soundBuffers.end()) {
        static sf::SoundBuffer dummy;
        return dummy;
    }
    return it->second;
}

void AssetManager::loadFont(const std::string& id, const std::string& filename) {
    sf::Font f;
    if (!f.loadFromFile(filename)) {
        std::cerr << "[AssetManager] failed to load font: " << filename << "\n";
        return;
    }
    _fonts.emplace(id, std::move(f));
}

sf::Font& AssetManager::getFont(const std::string& id) {
    auto it = _fonts.find(id);
    if (it == _fonts.end()) {
        static sf::Font dummy;
        return dummy;
    }
    return it->second;
}

