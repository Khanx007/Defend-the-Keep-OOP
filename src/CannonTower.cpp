#include "CannonTower.hpp"
#include "CannonBall.hpp"
#include "Enemy.hpp"
#include <cmath>
#include <iostream>

CannonTower::CannonTower(sf::Vector2f pos)
    : Tower(pos, 180.0f, 80.0f, 3.0f, 100)
{
    base.setRadius(22.0f);
    base.setOrigin(22.0f,22.0f);
    base.setFillColor(sf::Color(120,120,120));
    base.setPosition(position);

    barrel.setRadius(10.0f);
    barrel.setOrigin(4.0f,4.0f);
    barrel.setPosition(position);

    startActivation(3.0f);

    cout << "Cannon Tower created at (" << pos.x << ", " << pos.y << ")" << endl;
}

void CannonTower::update(float dt, const std::vector<Enemy*>& enemies) {
    if (!isActive()) {
        activationTimer += dt;
        if (activationTimer >= activationDelay) active = true;
        else return;
    }

    fireTimer += dt;
    // Update projectiles
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
            // rotate barrel to target
            sf::Vector2f dir = target->getPosition() - position;
            float ang = atan2(dir.y, dir.x) * 180.0f / 3.14159265f;
            barrel.setRotation(ang);
            barrel.setPosition(position);
            shoot(target);
            fireTimer = 0.0f;
        }
    }
}

void CannonTower::shoot(Enemy* target) {
    if (!target) return;
    CannonBall* ball = new CannonBall(position, target->getPosition(), 80.0f);
    projectiles.push_back(ball);
}

void CannonTower::render(sf::RenderWindow& window) {
    base.setPosition(position);
    window.draw(base);
    barrel.setPosition(position);
    window.draw(barrel);
    for (auto* p : projectiles) p->render(window);
}
