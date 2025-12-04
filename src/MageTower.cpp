#include "MageTower.hpp"
#include "MagicBolt.hpp"
#include "Enemy.hpp"
#include <cmath>
#include <iostream>

MageTower::MageTower(sf::Vector2f pos)
    : Tower(pos, 160.0f, 30.0f, 1.2f, 120)
{
    base.setRadius(20.0f);
    base.setOrigin(20.0f,20.0f);
    base.setFillColor(sf::Color(80,80,200));
    base.setPosition(position);

    crystal.setRadius(10.0f);
    crystal.setOrigin(10.0f,10.0f);
    crystal.setFillColor(sf::Color(150,50,255));
    crystal.setPosition(position);

    startActivation(3.0f);

    cout << "Mage Tower created at (" << pos.x << ", " << pos.y << ")" << endl;
}

void MageTower::update(float dt, const std::vector<Enemy*>& enemies) {
    if (!isActive()) {
        activationTimer += dt;
        if (activationTimer >= activationDelay) active = true;
        else return;
    }

    fireTimer += dt;
    for (int i = projectiles.size()-1; i>=0; --i) {
        projectiles[i]->update(dt);
        bool hit = projectiles[i]->checkCollision(const_cast<std::vector<Enemy*>&>(enemies));
        if (projectiles[i]->shouldDelete()) {
            delete projectiles[i];
            projectiles.erase(projectiles.begin()+i);
        }
    }

    if (fireTimer >= fireRate) {
        Enemy* target = findTarget(enemies);
        if (target) {
            // rotate crystal to target (visual)
            sf::Vector2f dir = target->getPosition() - position;
            float ang = atan2(dir.y, dir.x) * 180.0f / 3.14159265f;
            crystal.setRotation(ang);
            crystal.setPosition(position);
            shoot(target);
            fireTimer = 0.0f;
        }
    }
}

void MageTower::shoot(Enemy* target) {
    if (!target) return;
    MagicBolt* bolt = new MagicBolt(position, target->getPosition(), 2.0f, 0.5f);
    projectiles.push_back(bolt);
}

void MageTower::render(sf::RenderWindow& window) {
    base.setPosition(position);
    window.draw(base);
    crystal.setPosition(position);
    window.draw(crystal);
    for (auto* p : projectiles) p->render(window);
}
