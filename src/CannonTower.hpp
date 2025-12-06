#pragma once
#include "Tower.hpp"
#include <vector>
#include <memory>

class Projectile;

class CannonTower : public Tower {
public:
    CannonTower(sf::Vector2f pos);

    void update(float dt, const std::vector<Enemy*>& enemies) override;
    void render(sf::RenderWindow& window) override;

    // Non-owning raw pointers for collision/render queries
    std::vector<Projectile*> getProjectileRawPtrs() const {
        std::vector<Projectile*> out;
        out.reserve(projectiles.size());
        for (const auto &p : projectiles) out.push_back(p.get());
        return out;
    }

    // Access owning container if needed
    std::vector<std::unique_ptr<Projectile>>& getProjectilesOwned() { return projectiles; }

private:
    void shoot(Enemy* target) override;
    sf::CircleShape base;
    sf::CircleShape barrel;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Sprite towerSprite;
};
