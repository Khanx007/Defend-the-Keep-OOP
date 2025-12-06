#pragma once
#include "Entity.hpp"
#include <SFML/System.hpp>

class Entity;

class EnemyProjectile : public Entity {
public:
    EnemyProjectile(const sf::Vector2f& start, Entity* target, float speed, int damage);
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    bool hasHitTarget() const;
    bool shouldRemove() const { return removeFlag; }
    

    // Add a getter for target (so main can check projectiles)
    Entity* getTarget() const { return target; }

    // Ensure there is a flag to mark removal

    void markForRemoval() { removeFlag = true; }

    // accessors for the non-owning target pointer (used by main when clearing targets)
    void clearTarget();



private:
    Entity* target = nullptr;
    float speed = 200.f;
    int damage = 10;
    bool removeFlag = false;
    float hitRadius = 20.f;

};
