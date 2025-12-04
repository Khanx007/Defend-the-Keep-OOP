#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

class Castle : public Entity {
public:
    Castle();

    // Position helpers are inherited from Entity (setPosition/getPosition)

    // Damage / state
    void takeDamage(int amount) override;
    bool isAlive() const override;   // required by Enemy/tower logic
    bool isDestroyed() const { return health <= 0; }
    int  getHealth() const { return health; }

    // Rendering
    void render(sf::RenderWindow& window) override;

private:
    // Sprite + fallback texture in case asset missing
    sf::Sprite sprite;
    sf::Texture fallbackTexture;

    // Health
    int health = 100;
    int maxHealth = 100;

    // draw hp bar
    void drawHealthBar(sf::RenderWindow& window);
};
