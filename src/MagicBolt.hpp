#pragma once
#include "Projectile.hpp"

class MagicBolt : public Projectile {
public:
    MagicBolt(sf::Vector2f pos, sf::Vector2f targetPos, float slowDuration = 2.0f, float slowMultiplier = 0.5f);
    void render(sf::RenderWindow& window) override;
    bool checkCollision(std::vector<Enemy*>& enemies) override;

private:
    sf::CircleShape shape;
    float slowDuration;
    float slowMultiplier;
};
