#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include <vector>

class Enemy;  
class Tower : public Entity {
public:
    Tower(sf::Vector2f pos, float range, float damage, float fireRate, int cost);

    virtual void update(float dt, const std::vector<Enemy*>& enemies) = 0;
    // Base logic all towers must use
    void baseUpdate(float dt);

    virtual void render(sf::RenderWindow& window) override;

    sf::CircleShape getRangeCircle() const;
    int getCost() const { return cost; }

    // New: HP and damage handling
    void takeDamage(int amount);
    bool isDestroyed() const;

    // Activation / cooldown after placement
    void startActivation(float delaySeconds = 3.0f);
    bool isActive() const;

    void setLifetime(float seconds) { lifetimeSeconds = seconds; lifetimeTimer = seconds; }
    bool isExpired() const { return lifetimeSeconds > 0.f && lifetimeTimer <= 0.f; }

protected:
    Enemy* findTarget(const std::vector<Enemy*>& enemies);
    virtual void shoot(Enemy* target) = 0;

    float range;
    float damage;
    float fireRate; // seconds between shots
    float fireTimer = 0.0f;
    int cost;
    sf::CircleShape rangeCircle;

    // HP
    int hp = 100;
    int maxHp = 100;

    // Activation delay
    float activationDelay = 0.0f;
    float activationTimer = 0.0f;
    bool active = true;

    // turret rotation (angle in degrees)
    float turretRotation = 0.0f;

    float lifetimeSeconds = 0.f;    // 0 = infinite; >0 = lifetime
    float lifetimeTimer = 0.f;
    float fireCooldown    = 0.f;

    // in Tower class (private or protected)
    sf::Sprite sprite;

};
