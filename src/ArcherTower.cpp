#include "ArcherTower.hpp"
#include "Arrow.hpp"
#include "Enemy.hpp"
#include "Projectile.hpp"
#include <iostream>
#include <cmath>

using namespace std;

ArcherTower::ArcherTower(sf::Vector2f pos)
    : Tower(pos, 
            200.0f,    // range
            20.0f,     // damage
            1.0f,      // fireRate (1 shot per second)
            50)        // cost
{
    // Create visuals
    shape.setRadius(20.0f);
    shape.setFillColor(sf::Color(139, 90, 43));  // Brown/wooden color
    shape.setOutlineThickness(3);
    shape.setOutlineColor(sf::Color(101, 67, 33));  // Dark brown outline
    shape.setOrigin(20.0f, 20.0f);
    shape.setPosition(position);

    turret.setRadius(8.0f);
    turret.setOrigin(8.0f,8.0f);
    turret.setPosition(position);
    
    // Activation delay for newly placed towers
    startActivation(3.0f);

    cout << "Archer Tower created at (" << pos.x << ", " << pos.y << ")" << endl;
}

void ArcherTower::update(float dt, const vector<Enemy*>& enemies) {
    
    Tower::baseUpdate(dt);
    if (!isActive() || isDestroyed()) return;

    // Activation handling
    if (!isActive()) {
        activationTimer += dt;
        if (activationTimer >= activationDelay) {
            active = true;
        } else {
            return; // not active yet, cannot shoot or rotate
        }
    }

    // Update fire timer
    fireTimer += dt;
    
    // Update all projectiles
    for (int i = projectiles.size() - 1; i >= 0; i--) {
        projectiles[i]->update(dt);
        
        // Single collision check that has access to all enemies
        bool hit = projectiles[i]->checkCollision(const_cast<std::vector<Enemy*>&>(enemies));
        
        // Remove projectile if it hit or went off screen
        if (projectiles[i]->shouldDelete()) {
            delete projectiles[i];
            projectiles.erase(projectiles.begin() + i);
        }
    }
    
    // Try to shoot if fire rate allows
    if (fireTimer >= fireRate) {
        Enemy* target = findTarget(enemies);
        if (target != nullptr) {
            // rotate turret to face target
            sf::Vector2f dir = target->getPosition() - position;
            float ang = atan2(dir.y, dir.x) * 180.0f / 3.14159265f;
            turret.setRotation(ang);
            turret.setPosition(position);
            shoot(target);
            fireTimer = 0.0f;  // Reset timer
        }
    }
}

void ArcherTower::shoot(Enemy* target) {
    if (!target) return;
    
    // Create arrow projectile
    Arrow* arrow = new Arrow(position, target->getPosition());
    projectiles.push_back(arrow);
    
    // Visuals:flash white briefly
    shape.setFillColor(sf::Color::White);
}

void ArcherTower::render(sf::RenderWindow& window) {
    // Draw tower shape
    shape.setPosition(position);
    
    // Reset color back to green (fade from white flash)
    sf::Color currentColor = shape.getFillColor();
    if (currentColor == sf::Color::White) {
        shape.setFillColor(sf::Color::Green);
    }
    
    window.draw(shape);

    // Draw turret
    turret.setPosition(position);
    window.draw(turret);
    
    // Draw all projectiles
    for (Projectile* proj : projectiles) {
        proj->render(window);
    }
}
