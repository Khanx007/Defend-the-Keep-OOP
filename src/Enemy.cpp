#include "Enemy.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

using namespace std;

// Helper function to calculate distance between two points
static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

// Helper function to normalize a vector
static sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = sqrt(v.x * v.x + v.y * v.y);
    if (len != 0)
        return sf::Vector2f(v.x / len, v.y / len);
    return v;
}

Enemy::Enemy(const vector<sf::Vector2f>& path, float speed, int health, int damage, int goldValue)
    : path(path),
      speed(speed),
      health(health),
      maxHealth(health),
      damage(damage),
      goldValue(goldValue),
      currentWaypoint(0)
{
    if (!this->path.empty()) {
        position = this->path[0]; // position is expected from Entity base
    }
    // default attackRate - can be tuned per subclass
    attackRate = 1.0f;
    attackCooldown = 0.0f;

    // default to use projectile attacks if a spawner is provided
    useProjectiles = true;
    attackTarget = nullptr;
}

void Enemy::update(float dt) {
    // If dead, nothing to do
    if (health <= 0) {
        return;
    }

    // Update slow timer
    if (slowTimer > 0.0f) {
        slowTimer -= dt;
        if (slowTimer <= 0.0f) {
            slowTimer = 0.0f;
            slowMultiplier = 1.0f;
        }
    }
    

    // If we've reached the end and have no attack target, use fallbackTarget (castle) if set.
    if (reachedEnd && attackTarget == nullptr && fallbackTarget != nullptr && fallbackTarget->isAlive()) {
        attackTarget = fallbackTarget;
        // optional debug:
        std::cout << "[Enemy] Reached end and set fallback target (castle)\n";
    }

    // If we have a specific attack target (castle or tower) and we've reached the end or are in attack range,
    // handle attacking using attackCooldown/attackRate.
    if (attackTarget && attackTarget->isAlive()) {
        // Compute distance to target if getPosition() exists in Entity base; otherwise we assume attackTarget has getPosition()
        sf::Vector2f tpos = attackTarget->getPosition();
        float dist = distance(position, tpos);

        // If target is close enough (or the enemy has reached the path end), attack.
        // Use the enemy's configured attackRange (plus a small tolerance)
        const float effectiveAttackRange = attackRange; // already an Enemy member
        if (reachedEnd || dist <= effectiveAttackRange){
            attackCooldown -= dt;
            if (attackCooldown <= 0.0f) {
                attackCooldown = attackRate;

                // Prefer spawning a projectile if a spawner callback exists and projectiles are enabled
                if (useProjectiles && spawnEnemyProjectileFn) {
                    std::cout << "[Enemy] Spawning projectile from (" << position.x << "," << position.y
                              << ") -> target (" << tpos.x << "," << tpos.y << "), dmg=" << damage << "\n";
                    // Spawn projectile (main owns container via callback)
                    spawnEnemyProjectileFn(this, position, attackTarget, damage);
                } else {
                    // Fallback: direct damage (legacy behaviour)
                    std::cout << "[Enemy] Direct damage fallback on target. dmg=" << damage << "\n";
                    attackTarget->takeDamage(damage);
                }
            }
            // While attacking we do not move further
            sprite.setPosition(position);
            return;
        } else {
            // target out of effective attack range; clear and resume moving
            attackTarget = nullptr;
        }
    }

    // Movement along the path (existing behavior)
    if (currentWaypoint >= static_cast<int>(path.size())) {
        reachedEnd = true;
        // nothing else to do here (target handling above will handle attacks if set)
        return;
    }

    // compute effective speed considering slow effects
    float effectiveSpeed = speed * slowMultiplier;

    // Get target waypoint
    sf::Vector2f target = path[currentWaypoint];
    sf::Vector2f direction = target - position;
    float dist = distance(position, target);

    // If close enough to waypoint, move to next one
    const float waypointThreshold = 5.0f;
    if (dist < waypointThreshold) {
        currentWaypoint++;
        if (currentWaypoint >= static_cast<int>(path.size())) {
            reachedEnd = true;
            return;
        }
        target = path[currentWaypoint];
        direction = target - position;
        dist = distance(position, target);
    }

    // Move towards target
    if (dist > 0.001f) {
        sf::Vector2f normalizedDir = normalize(direction);
        position += normalizedDir * effectiveSpeed * dt;
    }

    // Update sprite position (if you use sprite)
    sprite.setPosition(position);
}

void Enemy::render(sf::RenderWindow& window) {
    // Base class only draws health bar; concrete subclasses draw shapes/sprites
    drawHealthBar(window);
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;

    // Visual feedback when hit
    sprite.setColor(sf::Color::Red);

    if (health == 0) {
        // Call death callback so WaveManager / PlayState can respond
        if (onDeathCallback) {
            onDeathCallback(this);
        }
    }
}

void Enemy::resetColor() {
    sprite.setColor(sf::Color::White);
}

bool Enemy::isDead() const {
    return health <= 0;
}

bool Enemy::hasReachedEnd() const {
    return reachedEnd;
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getGoldValue() const {
    return goldValue;
}

void Enemy::applySlow(float duration, float multiplier) {
    if (duration <= 0.0f) return;
    slowTimer = std::max(slowTimer, duration); // extend if longer
    slowMultiplier = multiplier;
}

void Enemy::drawHealthBar(sf::RenderWindow& window) {
    // Health bar dimensions
    float barWidth = 40.0f;
    float barHeight = 5.0f;
    float offsetY = -15.0f; // above the enemy

    // Background (grey bar)
    sf::RectangleShape bgBar(sf::Vector2f(barWidth, barHeight));
    bgBar.setPosition(position.x - barWidth/2, position.y + offsetY);
    bgBar.setFillColor(sf::Color(120,120,120));

    // Foreground (green bar showing current health)
    float healthPercent = (float)health / (float)maxHealth;
    if (healthPercent < 0.f) healthPercent = 0.f;
    sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercent, barHeight));
    healthBar.setPosition(position.x - barWidth/2, position.y + offsetY);
    healthBar.setFillColor(sf::Color::Green);

    window.draw(bgBar);
    window.draw(healthBar);
}

// Multiply current movement speed by multiplier
void Enemy::multiplySpeed(float mult) {
    if (mult <= 0.0f) return;
    speed *= mult;
}
