#pragma once
#include "Entity.hpp"
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>

class Enemy : public Entity {
public:
    Enemy(const std::vector<sf::Vector2f>& path, float speed, int health, int damage, int goldValue);

    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    void takeDamage(int amount);
    void resetColor();  // Reset sprite color after hit

    bool isDead() const;
    bool hasReachedEnd() const;
    int getDamage() const;
    int getGoldValue() const;

    // New: slow effect
    void applySlow(float duration, float multiplier);
    bool isSlowed() const { return slowTimer > 0.0f; }

    // New: death callback set by spawner / WaveManager / PlayState
    std::function<void(Enemy*)> onDeathCallback;

    // New: allow an entity target (e.g., Castle or Tower) to be set for attacking
    void setAttackTarget(Entity* t) { attackTarget = t; }
    Entity* getAttackTarget() const { return attackTarget; }

    // Multiply current movement speed by multiplier (e.g., 1.1 to increase 10%)
    void multiplySpeed(float mult);


    bool isCountedByWave() const { return countedByWave; }
    void markCountedByWave() { countedByWave = true; }


protected:
    std::vector<sf::Vector2f> path;  // Waypoints the enemy follows
    int currentWaypoint = 0;

    float speed;
    int health;
    int maxHealth;
    int damage;        // Damage dealt to castle if reaches end (also used for attack)
    int goldValue;     // Gold given to player when killed

    bool reachedEnd = false;

    // Slow effect
    float slowTimer = 0.0f;      // seconds remaining
    float slowMultiplier = 1.0f; // multiplier to speed (0.5 = half speed)

    // Attack logic
    Entity* attackTarget = nullptr;
    float attackRate = 1.0f;     // seconds between attacks when attacking a target
    float attackCooldown = 0.0f;

    // Visual / position are assumed to be provided by Entity base (sprite, position)

    

private:
    void drawHealthBar(sf::RenderWindow& window);

    bool countedByWave = false; // whether this enemy has already been reported to WaveManager

};
