#include "MagicBolt.hpp"
#include "Enemy.hpp"
#include <cmath>

MagicBolt::MagicBolt(sf::Vector2f pos, sf::Vector2f targetPos, float slowDuration, float slowMultiplier)
    : Projectile(pos, targetPos - pos, 350.0f, 30.0f, ProjectileType::MAGIC_BOLT),
      slowDuration(slowDuration), slowMultiplier(slowMultiplier)
{
    shape.setRadius(6.0f);
    shape.setFillColor(sf::Color(150,50,255));
    shape.setOrigin(6.0f,6.0f);
    shape.setPosition(position);
}

void MagicBolt::render(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(shape);
}

bool MagicBolt::checkCollision(std::vector<Enemy*>& enemies) {
    for (Enemy* enemy : enemies) {
        if (!enemy || enemy->isDead()) continue;
        float dx = position.x - enemy->getPosition().x;
        float dy = position.y - enemy->getPosition().y;
        float dist = sqrt(dx*dx + dy*dy);
        if (dist < 18.0f) {
            // Apply damage and slow to this enemy
            enemy->takeDamage((int)damage);
            enemy->applySlow(slowDuration, slowMultiplier);
            markedForDeletion = true;
            return true;
        }
    }
    return false;
}
