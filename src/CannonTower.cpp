#include "CannonTower.hpp"
#include "CannonBall.hpp"
#include "Enemy.hpp"
#include "AssetManager.hpp"
#include "AudioManager.hpp"
#include <cmath>
#include <iostream>
#include <memory>

CannonTower::CannonTower(sf::Vector2f pos)
    : Tower(pos, 180.f, 80.f, 3.0f, 100)
{
    sf::Texture& tex = AssetManager::getTexture("cannon_tower");
    towerSprite.setTexture(tex);

    sf::Vector2u s = tex.getSize();
    towerSprite.setOrigin(s.x*0.5f, s.y*0.5f);
    towerSprite.setScale(60.f / s.y, 60.f / s.y);
    towerSprite.setPosition(pos);

    startActivation(3.0f);

    std::cout << "Cannon Tower created at (" << pos.x << ", " << pos.y << ")\n";
}

void CannonTower::update(float dt, const std::vector<Enemy*>& enemies)
{
    Tower::baseUpdate(dt);
    if (!isActive() || isDestroyed()) return;

    fireTimer += dt;

    // Update cannonballs — use iterator erase so unique_ptr destructor frees memory
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->update(dt);
        bool hit = (*it)->checkCollision(const_cast<std::vector<Enemy*>&>(enemies));

        if ((*it)->shouldDelete()) {
            it = projectiles.erase(it); // unique_ptr deletes automatically
        } else {
            ++it;
        }
    }

    if (fireTimer >= fireRate) {
        Enemy* target = findTarget(enemies);
        if (target) {
            sf::Vector2f dir = target->getPosition() - position;
            float ang = atan2(dir.y, dir.x) * 180.f / 3.14159f;
            towerSprite.setRotation(ang);

            shoot(target);
            fireTimer = 0.f;
        }
    }
}

void CannonTower::shoot(Enemy* target)
{
    if (!target) return;

    // Use unique_ptr
    projectiles.push_back(std::make_unique<CannonBall>(position, target->getPosition(), 80.f));

    AudioManager::playSFX("explosion");
}

void CannonTower::render(sf::RenderWindow& window)
{
    towerSprite.setPosition(position);
    window.draw(towerSprite);

    for (auto &up : projectiles) {
        if (up) up->render(window);
    }
}
