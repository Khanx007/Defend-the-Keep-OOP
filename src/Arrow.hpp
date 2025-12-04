#pragma once
#include "Projectile.hpp"

class Arrow : public Projectile {
public:
    Arrow(sf::Vector2f pos, sf::Vector2f targetPos);
    void render(sf::RenderWindow& window) override;

private:
    sf::CircleShape shape;
};
