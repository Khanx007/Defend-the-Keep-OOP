#pragma once
#include "Entity.hpp"
#include <SFML/System.hpp>
#include <functional>
#include <vector>

class Entity; // forward

class Enemy : public Entity {
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int health, int damage, int goldValue);

    virtual void update(float dt) override;
    virtual void render(sf::RenderWindow& window) override;

    void takeDamage(int amount) override;
    void resetColor();

    bool isDead() const;
    bool hasReachedEnd() const;
    int getDamage() const;
    int getGoldValue() const;

    // slow effect
    void applySlow(float duration, float multiplier);
    bool isSlowed() const { return slowTimer > 0.0f; }

    // Targeting / attacking API
    void setAttackTarget(Entity* t) { attackTarget = t; }
    Entity* getAttackTarget() const { return attackTarget; }
    void clearAttackTarget() { attackTarget = nullptr; }

    // Set attack parameters (exposed so tower types or spawn config can tune)
    void setAttackRange(float r) { attackRange = r; }
    void setAttackDamage(int d) { attackDamage = d; }
    void setAttackRate(float r) { attackRate = r; } // seconds between attacks

    int getAttackDamage() const { return damage; }
    float getAttackRange() const { return attackRange; }
    float getAttackRate() const { return attackRate; }
    // speed multiplier helper
    void multiplySpeed(float mult);

    // single-wave accounting helper (avoid double counting)
    bool isCountedByWave() const { return countedByWave; }
    void markCountedByWave() { countedByWave = true; }

    // in Enemy.hpp (public)
    using EnemyProjectileSpawner = std::function<void(Enemy*, sf::Vector2f, Entity*, int)>;

// Set the enemy projectile spawn callback so enemies can create projectiles without knowing game containers
    void setProjectileSpawner(EnemyProjectileSpawner fn) { spawnEnemyProjectileFn = fn; }   

    // force projectile usage on/off (default true)
    void setUseProjectiles(bool v) { useProjectiles = v; }
    bool getUseProjectiles() const { return useProjectiles; }

    void setFallbackTarget(Entity* t) { fallbackTarget = t; }
    bool hasProjectileSpawner() const { return (bool)spawnEnemyProjectileFn; }
    Entity* getFallbackTarget() const { return fallbackTarget; }



protected:
    std::vector<sf::Vector2f> path;
    int currentWaypoint = 0;

    float speed;
    int health;
    int maxHealth;
    int damage;        // damage to castle on reaching end
    int goldValue;

    bool reachedEnd = false;

    // Slow effect
    float slowTimer = 0.0f;
    float slowMultiplier = 1.0f;

    // Attack mechanics
    Entity* attackTarget = nullptr;
    float attackRange = 100.f;   // default attack range (pixels)
    int attackDamage = 8;       // damage per hit
    float attackRate = 1.0f;    // seconds per attack
    float attackCooldown = 0.0f;

    // Attack mode: if true spawn projectile; else apply direct damage
    bool useProjectiles = true;
    // in protected:
    std::function<void(class Enemy*, sf::Vector2f, Entity*, int)> spawnEnemyProjectileFn;

    // spawnEnemyProjectileFn(this, originPos, target, damage) must create projectile
    // in public (or protected) of Enemy.hpp
    std::function<void(Enemy*)> onDeathCallback;

    Entity* fallbackTarget = nullptr;


private:
    void drawHealthBar(sf::RenderWindow& window);

    bool countedByWave = false;
};
