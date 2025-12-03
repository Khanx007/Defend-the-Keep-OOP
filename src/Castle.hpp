#pragma once
#include <SFML/Graphics.hpp>

class Castle {
public:
    Castle();

    void takeDamage(int amount);
    bool isDestroyed() const { return health <= 0; }
    int getHealth() const { return health; }

    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

    // Optional: set position if you want to move castle in tests
    void setPosition(const sf::Vector2f& pos);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    int health = 100;

    // fallback visual if texture isn't loaded
    sf::RectangleShape body;
};
