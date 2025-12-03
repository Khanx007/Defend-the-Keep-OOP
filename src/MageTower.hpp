#pragma once
#include "Tower.hpp"
#include <vector>

class Projectile;

class MageTower : public Tower {
public:
    MageTower(sf::Vector2f pos);

    void update(float dt, const std::vector<Enemy*>& enemies) override;
    void render(sf::RenderWindow& window) override;

private:
    void shoot(Enemy* target) override;
    sf::CircleShape base;
    sf::CircleShape crystal;
    std::vector<Projectile*> projectiles;
};
