#include "Enemy.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>

// Note: your Entity.hpp should provide:
//   - sf::Vector2f position;
//   - sf::Sprite sprite;  (or equivalent)
//   - virtual sf::Vector2f getPosition() const;  (optional)
// If those names differ, adapt the uses of 'position' and 'sprite' below.

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

    // If we have a specific attack target (castle or tower) and we've reached the end or are in attack range,
    // handle attacking using attackCooldown/attackRate.
    if (attackTarget && attackTarget->isAlive()) {
        // Compute distance to target if getPosition() exists in Entity base; otherwise we assume attackTarget has getPosition()
        sf::Vector2f tpos = attackTarget->getPosition();
        float dist = distance(position, tpos);

        // If target is close enough (or the enemy has reached the path end), attack.
        // We'll use a small tolerance for "in range" — if you want ranged enemies change this.
        const float attackRangeTolerance = 40.0f; // adjustable
        if (reachedEnd || dist <= attackRangeTolerance) {
            attackCooldown -= dt;
            if (attackCooldown <= 0.0f) {
                attackCooldown = attackRate;
                // Deal damage to the target
                attackTarget->takeDamage(damage);
            }
            // while attacking we do not move further
            return;
        }
    }

    // Movement along the path (existing behavior)
    if (currentWaypoint >= static_cast<int>(path.size())) {
        reachedEnd = true;
        // nothing else to do here (target handling above will handle attacks)
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

    // Background (red bar)
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

