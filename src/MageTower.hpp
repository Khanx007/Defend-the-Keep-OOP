#pragma once
#include "Tower.hpp"
#include <vector>
#include <memory>

class Projectile;

class MageTower : public Tower {
public:
    MageTower(sf::Vector2f pos);

    void update(float dt, const std::vector<Enemy*>& enemies) override;
    void render(sf::RenderWindow& window) override;

    std::vector<Projectile*> getProjectileRawPtrs() const {
        std::vector<Projectile*> out;
        out.reserve(projectiles.size());
        for (const auto &p : projectiles) out.push_back(p.get());
        return out;
    }

    std::vector<std::unique_ptr<Projectile>>& getProjectilesOwned() { return projectiles; }

private:
    void shoot(Enemy* target) override;
    sf::CircleShape base;
    sf::CircleShape crystal;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    sf::Sprite towerSprite;
};
