#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

class AssetManager {
public:
    static void loadTexture(const std::string& id, const std::string& filename);
    static sf::Texture& getTexture(const std::string& id);

    static void loadSoundBuffer(const std::string& id, const std::string& filename);
    static sf::SoundBuffer& getSoundBuffer(const std::string& id);

    static void loadFont(const std::string& id, const std::string& filename);
    static sf::Font& getFont(const std::string& id);
};
