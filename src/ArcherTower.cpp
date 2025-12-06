#include "ArcherTower.hpp"
#include "Arrow.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include "AssetManager.hpp"
#include "AudioManager.hpp"
#include <cmath>
#include <iostream>

ArcherTower::ArcherTower(sf::Vector2f pos)
    : Tower(pos, 200.f, 20.f, 1.0f, 50)
{
    // Assign sprite texture
    sf::Texture& tex = AssetManager::getTexture("archer_tower");
    towerSprite.setTexture(tex);

    sf::Vector2u s = tex.getSize();
    if (s.x > 0 && s.y > 0) {
        towerSprite.setOrigin(s.x * 0.5f, s.y * 0.5f);

        // Scale sprite to a reasonable size
        float scale = 60.f / s.y;  // Adjust as needed
        towerSprite.setScale(scale, scale);
    }

    towerSprite.setPosition(position);

    // Activation delay
    startActivation(3.f);

    std::cout << "Archer Tower created at (" << pos.x << ", " << pos.y << ")\n";
}

void ArcherTower::update(float dt, const std::vector<Enemy*>& enemies)
{
    Tower::baseUpdate(dt);
    if (!isActive() || isDestroyed()) return;

    fireTimer += dt;

    // Update projectiles — use iterator erase so unique_ptr destructor frees memory
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->update(dt);
        bool hit = (*it)->checkCollision(const_cast<std::vector<Enemy*>&>(enemies));

        if ((*it)->shouldDelete()) {
            it = projectiles.erase(it); // unique_ptr automatically deletes
        } else {
            ++it;
        }
    }

    // Shoot logic
    if (fireTimer >= fireRate) {
        Enemy* target = findTarget(enemies);
        if (target) {
            // Rotate sprite to face target
            sf::Vector2f dir = target->getPosition() - position;
            float ang = std::atan2(dir.y, dir.x) * 180.f / 3.14159f;
            towerSprite.setRotation(ang);

            shoot(target);
            fireTimer = 0.f;
        }
    }
}

void ArcherTower::shoot(Enemy* target)
{
    if (!target) return;

    // create and own projectile via unique_ptr
    projectiles.push_back(std::make_unique<Arrow>(position, target->getPosition()));

    AudioManager::playSFX("shoot_arrow");
}

void ArcherTower::render(sf::RenderWindow& window)
{
    towerSprite.setPosition(position);
    window.draw(towerSprite);

    for (auto &up : projectiles) {
        if (up) up->render(window);
    }
}
