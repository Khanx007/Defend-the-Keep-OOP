#pragma once
#include "Tower.hpp"
#include <vector>

class Projectile;

class ArcherTower : public Tower {
public:
    ArcherTower(sf::Vector2f pos);

    void update(float dt, const std::vector<Enemy*>& enemies) override;
    void render(sf::RenderWindow& window) override;
    
    std::vector<Projectile*>& getProjectiles() { return projectiles; }

private:
    void shoot(Enemy* target) override;
    sf::CircleShape shape;
    sf::CircleShape turret;
    std::vector<Projectile*> projectiles;
};
