#pragma once
#include <SFML/Graphics.hpp>

class Castle {
public:
    Castle();

    void takeDamage(int amount);
    bool isDestroyed() const { return health <= 0; }
    int getHealth() const { return health; }

    void render(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    int health = 100;
};