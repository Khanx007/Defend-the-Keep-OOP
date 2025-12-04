#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include <vector>

class Enemy;

enum class ProjectileType { ARROW, CANNON_SHOT, MAGIC_BOLT };

class Projectile : public Entity {
public:
    Projectile(sf::Vector2f pos, sf::Vector2f direction, float speed, float damage, ProjectileType type);

    virtual void update(float dt) override;
    // Check collisions against all enemies; returns true if it hit any (or detonated)
    virtual bool checkCollision(std::vector<Enemy*>& enemies);
    float getDamage() const { return damage; }

protected:
    sf::Vector2f velocity;
    float damage;
    bool markedForDeletion = false;
    ProjectileType type = ProjectileType::ARROW;

public:
    bool shouldDelete() const { return markedForDeletion; }
};
