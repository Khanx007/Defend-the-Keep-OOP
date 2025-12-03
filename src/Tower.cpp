#include "Tower.hpp"
#include "Enemy.hpp"
#include <cmath>
#include <iostream>

using namespace std;

// Helper function to calculate distance
static float distanceBetween(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

Tower::Tower(sf::Vector2f pos, float range, float damage, float fireRate, int cost)
    : range(range), damage(damage), fireRate(fireRate), cost(cost)
{
    position = pos;

    // Setup range circle for visualization
    rangeCircle.setRadius(range);
    rangeCircle.setFillColor(sf::Color(255, 255, 255, 30));  // semi-transparent white
    rangeCircle.setOutlineThickness(2);
    rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 100));
    rangeCircle.setOrigin(range, range);  // center the circle
    rangeCircle.setPosition(position);

    // Default HP
    maxHp = 100;
    hp = maxHp;

    // Default active / activation delay
    active = true;
    activationDelay = 0.0f;
    activationTimer = 0.0f;

    // Lifetime defaults (0 = permanent)
    lifetimeSeconds = 0.0f;
    lifetimeTimer = 0.0f;

    // fire cooldown
    fireCooldown = 0.0f;
}

void Tower::render(sf::RenderWindow& window) {
    // Base class does not draw visuals; derived towers should draw themselves.
    // But keep range circle in sync if you want to visualize it for debugging:
    rangeCircle.setPosition(position);
    // window.draw(rangeCircle); // draw only if you want to show ranges all the time
}

sf::CircleShape Tower::getRangeCircle() const {
    return rangeCircle;
}

Enemy* Tower::findTarget(const vector<Enemy*>& enemies) {
    Enemy* closestEnemy = nullptr;
    float closestDistance = range + 1;  // Start beyond range

    // Find closest enemy within range
    for (Enemy* enemy : enemies) {
        if (!enemy) continue;
        if (enemy->isDead()) continue;  // Skip dead enemies

        float dist = distanceBetween(position, enemy->getPosition());

        if (dist <= range && dist < closestDistance) {
            closestEnemy = enemy;
            closestDistance = dist;
        }
    }

    return closestEnemy;  // Returns nullptr if no enemy in range
}

void Tower::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}

bool Tower::isDestroyed() const {
    return hp <= 0;
}

void Tower::startActivation(float delaySeconds) {
    activationDelay = delaySeconds;
    activationTimer = 0.0f;
    active = false;
}

bool Tower::isActive() const {
    return active;
}

// ----------------- NEW/UPDATED METHODS -----------------

// Update: handle activation delay, lifetime countdown, and maintain visual state.
// Note: actual firing/targeting is intended to be implemented in derived classes' update().
// But this base update ensures activation/lifetime work consistently.
void Tower::baseUpdate(float dt)
{
    // --- Activation delay handling ---
    if (!active) {
        activationTimer += dt;
        if (activationTimer >= activationDelay) {
            active = true;
            activationTimer = activationDelay;
            fireCooldown = 0.f;
        } else {
            return; // still inactive → derived update should stop here
        }
    }

    // --- Lifetime countdown ---
    if (lifetimeSeconds > 0.f) {
        lifetimeTimer -= dt;
        if (lifetimeTimer <= 0.f) {
            hp = 0;  // mark destroyed
            return;
        }
    }

    // --- Cooldown tick ---
    if (fireCooldown > 0.f) {
        fireCooldown -= dt;
        if (fireCooldown < 0.f) fireCooldown = 0.f;
    }

    // --- Keep range circle aligned ---
    rangeCircle.setPosition(position);
}


// void Tower::setLifetime(float seconds) {
//     if (seconds <= 0.0f) {
//         lifetimeSeconds = 0.0f;
//         lifetimeTimer = 0.0f;
//     } else {
//         lifetimeSeconds = seconds;
//         lifetimeTimer = seconds;
//     }
// }

// bool Tower::isExpired() const {
//     return (lifetimeSeconds > 0.0f) && (lifetimeTimer <= 0.0f);
// }
