// MenuScreen.hpp
#pragma once

#include <SFML/Graphics.hpp>

struct MenuScreen {
    sf::Text titleText;
    sf::Text subtitleText;
    sf::RectangleShape buttonStartRect;
    sf::Text buttonStartText;

    void init(sf::Font& font);
    void render(sf::RenderWindow& window) const;
    bool handleClick(const sf::Vector2f& worldPos) const; // returns true if START was clicked
};
