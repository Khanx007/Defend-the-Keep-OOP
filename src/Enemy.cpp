#include "Enemy.hpp"
#include <cmath>
#include <iostream>

using namespace std;

// Helper function to calculate distance between two points
float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

// Helper function to normalize a vector
sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = sqrt(v.x * v.x + v.y * v.y);
    if (len != 0)
        return sf::Vector2f(v.x / len, v.y / len);
    return v;
}

Enemy::Enemy(const vector<sf::Vector2f>& path, float speed, int health, int damage, int goldValue)
    : path(path), speed(speed), health(health), maxHealth(health), 
      damage(damage), goldValue(goldValue), currentWaypoint(0)
{
    if (!path.empty()) {
        position = path[0];
    }
}

void Enemy::update(float dt) {
    if (currentWaypoint >= path.size()) {
        reachedEnd = true;
        return;
    }

    // update slow timer
    if (slowTimer > 0.0f) {
        slowTimer -= dt;
        if (slowTimer <= 0.0f) {
            slowTimer = 0.0f;
            slowMultiplier = 1.0f;
        }
    }

    float effectiveSpeed = speed * slowMultiplier;

    // Get target waypoint
    sf::Vector2f target = path[currentWaypoint];
    sf::Vector2f direction = target - position;
    float dist = distance(position, target);

    // If close enough to waypoint, move to next one
    if (dist < 5.0f) {
        currentWaypoint++;
        if (currentWaypoint >= path.size()) {
            reachedEnd = true;
            return;
        }
        target = path[currentWaypoint];
        direction = target - position;
    }

    // Move towards target
    sf::Vector2f normalizedDir = normalize(direction);
    position += normalizedDir * effectiveSpeed * dt;

    // Update sprite position
    sprite.setPosition(position);
}

void Enemy::render(sf::RenderWindow& window) {
    // Only draw health bar (child classes will draw their own shapes)
    drawHealthBar(window);
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    
    // Visual feedback when hit
    sprite.setColor(sf::Color::Red);
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
    bgBar.setFillColor(sf::Color::Red);

    // Foreground (green bar showing current health)
    float healthPercent = (float)health / (float)maxHealth;
    sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercent, barHeight));
    healthBar.setPosition(position.x - barWidth/2, position.y + offsetY);
    healthBar.setFillColor(sf::Color::Green);

    window.draw(bgBar);
    window.draw(healthBar);
}
