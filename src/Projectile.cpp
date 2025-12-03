#include "Projectile.hpp"
#include "Enemy.hpp"
#include <cmath>

using namespace std;

Projectile::Projectile(sf::Vector2f pos, sf::Vector2f direction, float speed, float damage, ProjectileType type)
    : damage(damage), type(type)
{
    position = pos;
    
    // Normalize direction and set velocity
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        velocity = sf::Vector2f(direction.x / length * speed, direction.y / length * speed);
    }
}

void Projectile::update(float dt) {
    // Move projectile
    position += velocity * dt;
    
    // Mark for deletion if off screen
    if (position.x < -50 || position.x > 1330 || position.y < -50 || position.y > 770) {
        markedForDeletion = true;
    }
}

bool Projectile::checkCollision(vector<Enemy*>& enemies) {
    // Base projectile checks circle collision against each enemy and deletes on first hit
    for (Enemy* enemy : enemies) {
        if (!enemy || enemy->isDead()) continue;
        float dx = position.x - enemy->getPosition().x;
        float dy = position.y - enemy->getPosition().y;
        float dist = sqrt(dx * dx + dy * dy);
        if (dist < 20.0f) {
            markedForDeletion = true;
            enemy->takeDamage((int)damage);
            return true;
        }
    }
    return false;
}
