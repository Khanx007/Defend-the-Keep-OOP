#include "MageTower.hpp"
#include "MagicBolt.hpp"
#include "Enemy.hpp"
#include "AssetManager.hpp"
#include "AudioManager.hpp"
#include <cmath>
#include <iostream>
#include <memory>

MageTower::MageTower(sf::Vector2f pos)
    : Tower(pos, 160.f, 30.f, 1.2f, 120)
{
    sf::Texture& tex = AssetManager::getTexture("mage_tower");
    towerSprite.setTexture(tex);

    sf::Vector2u s = tex.getSize();
    towerSprite.setOrigin(s.x/2, s.y/2);
    towerSprite.setScale(60.f / s.y, 60.f / s.y);

    towerSprite.setPosition(pos);

    startActivation(3.0f);

    std::cout << "Mage Tower created at (" << pos.x << ", " << pos.y << ")\n";
}

void MageTower::update(float dt, const std::vector<Enemy*>& enemies)
{
    Tower::baseUpdate(dt);
    if (!isActive() || isDestroyed()) return;

    fireTimer += dt;

    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->update(dt);
        bool hit = (*it)->checkCollision(const_cast<std::vector<Enemy*>&>(enemies));
        if ((*it)->shouldDelete()) {
            it = projectiles.erase(it);
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

void MageTower::shoot(Enemy* target)
{
    if (!target) return;

    projectiles.push_back(std::make_unique<MagicBolt>(position, target->getPosition(), 2.0f, 0.5f));

    AudioManager::playSFX("enemy_hit");
}

void MageTower::render(sf::RenderWindow& window)
{
    towerSprite.setPosition(position);
    window.draw(towerSprite);

    for (auto &up : projectiles) {
        if (up) up->render(window);
    }
}
