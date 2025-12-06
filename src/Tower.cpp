#include "Tower.hpp"
#include "Enemy.hpp"
#include <cmath>
#include <iostream>
#include <climits> // for INT_MAX

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




int Tower::getUpgradeCost() const {
    // baseUpgradeCost should be a member (e.g. 120). If not present, adjust accordingly.
    // Provide fallback if baseUpgradeCost not set.
    int base = (baseUpgradeCost > 0) ? baseUpgradeCost : 120;
    if (upgradeLevel == 0) return base;
    if (upgradeLevel == 1) return static_cast<int>(base * 1.8f);
    if (upgradeLevel == 2) return static_cast<int>(base * 2.8f);
    return INT_MAX; // already maxed (or no further upgrades)
}

bool Tower::upgrade() {
    // return false if already at max level
    if (upgradeLevel >= 3) {
        std::cout << "[Tower] Already at max upgrade level\n";
        return false;
    }

    upgradeLevel++;
    startActivation(30.f);

    
    maxHp = static_cast<int>(std::max(1.0f, float(maxHp) * 1.3f));
    // restore some HP on upgrade (heal up to 30% of new max)
    int healAmount = static_cast<int>(maxHp * 0.3f);
    hp = std::min(maxHp, hp + healAmount);

    damage = damage + static_cast<int>(damage * 0.25f);

    // Reduce the firing interval slightly (so actual shots/sec increases).
    if (fireRate > 0.1f) fireRate = fireRate * 0.9f;

    std::cout << "[Tower] Upgraded to level " << upgradeLevel
              << " (hp=" << maxHp << ", dmg=" << damage << ", fireRate=" << fireRate << ")\n";

    // Trigger onUpgrade hook so higher-level code (main/wavemanager) can react
    if (onUpgrade) {
        // We use the convention: onUpgrade(float extraSeconds) -> caller interprets as "extend inter-wave by extraSeconds"
        float extraSeconds = 5.0f; // small breathing room
        onUpgrade(extraSeconds);
    }

    return true;
}
