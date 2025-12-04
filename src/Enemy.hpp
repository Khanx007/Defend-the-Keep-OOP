#pragma once
#include "Entity.hpp"
#include <vector>

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

protected:
    std::vector<sf::Vector2f> path;  // Waypoints the enemy follows
    int currentWaypoint;
    
    float speed;
    int health;
    int maxHealth;
    int damage;        // Damage dealt to castle if reaches end
    int goldValue;     // Gold given to player when killed
    
    bool reachedEnd = false;

    // Slow effect
    float slowTimer = 0.0f;      // seconds remaining
    float slowMultiplier = 1.0f; // multiplier to speed (0.5 = half speed)

private:
    void drawHealthBar(sf::RenderWindow& window);
};
