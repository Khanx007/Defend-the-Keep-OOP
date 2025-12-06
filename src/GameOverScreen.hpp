// GameOverScreen.hpp
#pragma once

#include <SFML/Graphics.hpp>

struct GameOverScreen {
    sf::RectangleShape overlay;
    sf::Text titleText;
    sf::Text statsText;
    sf::RectangleShape buttonRestartRect;
    sf::Text buttonRestartText;

    void init(sf::Font& font);
    void updateStats(int wavesSurvived, int enemiesEliminated, int defensesPlaced);
    void render(sf::RenderWindow& window) const;
    bool handleClick(const sf::Vector2f& worldPos) const; // true if RESTART clicked
};
