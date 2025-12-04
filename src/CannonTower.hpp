#pragma once
#include "Tower.hpp"
#include <vector>

class Projectile;

class CannonTower : public Tower {
public:
    CannonTower(sf::Vector2f pos);

    void update(float dt, const std::vector<Enemy*>& enemies) override;
    void render(sf::RenderWindow& window) override;

private:
    void shoot(Enemy* target) override;
    sf::CircleShape base;
    sf::CircleShape barrel;
    std::vector<Projectile*> projectiles;
    sf::Sprite towerSprite;

};
