#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class Castle : public Entity {
public:
    Castle();

    // health
    void takeDamage(int amount) override;
    bool isDestroyed() const { return health <= 0; }
    bool isAlive() const override { return health > 0; }

    int getHealth() const { return health; }

    // rendering
    void render(sf::RenderWindow& window) override;

    // optionally load texture from file; returns true on success
    bool loadTexture(const std::string& path);

private:
    sf::Texture texture;
    sf::RectangleShape healthBarBg;
    sf::RectangleShape healthBarFg;

    int health = 200;
    int maxHealth = 200;

    // visual fallback if texture not loaded
    sf::CircleShape fallbackShape;
};
