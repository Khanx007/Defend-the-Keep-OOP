#pragma once
#include "Tower.hpp"
#include <vector>
#include <memory>

class Projectile;

class ArcherTower : public Tower {
public:
    ArcherTower(sf::Vector2f pos);

    void update(float dt, const std::vector<Enemy*>& enemies) override;
    void render(sf::RenderWindow& window) override;
    
    // Returns a non-owning list of raw pointers for read/render/collision use.
    std::vector<Projectile*> getProjectileRawPtrs() const {
        std::vector<Projectile*> out;
        out.reserve(projectiles.size());
        for (const auto &p : projectiles) out.push_back(p.get());
        return out;
    }

    // If we  need  to access to the owning container (use sparingly).
    std::vector<std::unique_ptr<Projectile>>& getProjectilesOwned() { return projectiles; }

private:
    void shoot(Enemy* target) override;
    sf::CircleShape shape;
    sf::CircleShape turret;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Sprite towerSprite;
};
