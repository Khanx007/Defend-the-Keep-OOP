#pragma once
#include "Projectile.hpp"

class CannonBall : public Projectile {
public:
    CannonBall(sf::Vector2f pos, sf::Vector2f targetPos, float explosionRadius = 60.0f);
    void render(sf::RenderWindow& window) override;
    bool checkCollision(std::vector<Enemy*>& enemies) override;

private:
    sf::CircleShape shape;
    float explosionRadius;
};
