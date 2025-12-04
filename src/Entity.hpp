#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void update(float dt) {}
    virtual void render(sf::RenderWindow& window) = 0;

    sf::Vector2f getPosition() const { return position; }
    void setPosition(sf::Vector2f pos) { position = pos; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

protected:
    sf::Sprite sprite;
    sf::Vector2f position{0, 0};
};