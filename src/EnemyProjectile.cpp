#include "EnemyProjectile.hpp"
#include <cmath>

static float dist(const sf::Vector2f& a, const sf::Vector2f& b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrt(dx*dx + dy*dy);
}

static sf::Vector2f normalize(const sf::Vector2f& v) {
    float l = sqrt(v.x*v.x + v.y*v.y);
    if (l == 0.f) return sf::Vector2f(0.f,0.f);
    return sf::Vector2f(v.x / l, v.y / l);
}

EnemyProjectile::EnemyProjectile(const sf::Vector2f& start, Entity* target_, float speed_, int damage_)
: target(target_), speed(speed_), damage(damage_) {
    position = start;
    // you can set a simple shape for debug by using sprite or not
    sprite.setPosition(position);
}

// EnemyProjectile.cpp: robust update logic (replace your current update)
void EnemyProjectile::update(float dt) {
    // If already marked for removal, skip.
    if (removeFlag) return;

    // If target pointer is null or target died, mark for removal.
    if (target == nullptr || !target->isAlive()) {
        removeFlag = true;
        return;
    }

    // Move projectile toward target position
    sf::Vector2f targetPos = target->getPosition();
    sf::Vector2f dir = targetPos - position;
    float len = sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len > 1e-6f) {
        dir /= len;
        position += dir * speed * dt;
        sprite.setPosition(position);
    }

    // If close enough to hit, apply damage and remove
    if (len <= hitRadius) {
        // Protect target access with isAlive() check
        if (target && target->isAlive()) {
            target->takeDamage(damage);
        }
        removeFlag = true;
    }
}


void EnemyProjectile::render(sf::RenderWindow& window) {
    // Draw as a small circle for now
    sf::CircleShape s(4.f);
    s.setOrigin(4.f, 4.f);
    s.setPosition(position);
    s.setFillColor(sf::Color::Red);
    window.draw(s);
}
