#include "CannonBall.hpp"
#include "Enemy.hpp"
#include <cmath>
#include <iostream>

CannonBall::CannonBall(sf::Vector2f pos, sf::Vector2f targetPos, float explosionRadius)
    : Projectile(pos, targetPos - pos, 300.0f, 80.0f, ProjectileType::CANNON_SHOT), explosionRadius(explosionRadius)
{
    shape.setRadius(8.0f);
    shape.setFillColor(sf::Color(100,100,100));
    shape.setOrigin(8.0f,8.0f);
    shape.setPosition(position);
}

void CannonBall::render(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(shape);
}

bool CannonBall::checkCollision(std::vector<Enemy*>& enemies) {
    // On collision with any enemy, apply AoE damage to nearby enemies and delete self
    for (Enemy* enemy : enemies) {
        if (!enemy || enemy->isDead()) continue;
        float dx = position.x - enemy->getPosition().x;
        float dy = position.y - enemy->getPosition().y;
        float dist = sqrt(dx*dx + dy*dy);
        if (dist < 18.0f) {
            // Detonate: damage all enemies within explosionRadius
            for (Enemy* e2 : enemies) {
                if (!e2 || e2->isDead()) continue;
                float dx2 = position.x - e2->getPosition().x;
                float dy2 = position.y - e2->getPosition().y;
                float dist2 = sqrt(dx2*dx2 + dy2*dy2);
                if (dist2 <= explosionRadius) {
                    e2->takeDamage((int)damage);
                }
            }
            // Mark for deletion
            markedForDeletion = true;
            return true;
        }
    }
    return false;
}
