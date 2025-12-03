#include "Brute.hpp"

Brute::Brute(const std::vector<sf::Vector2f>& path)
    : Enemy(path,
            65.0f,     // speed (slow)
            200,       // health (high)
            30,        // damage to castle
            50)        // gold value
{
    setAttackRange(120.f);   // grunt fires from ~120 px away
    setAttackRate(1.2f);     // one shot every 1.2 seconds
    setAttackDamage(10);
    // Create visual representation - dark gray circle (bigger)
    shape.setRadius(25.0f);
    shape.setFillColor(sf::Color(50, 50, 50)); // Dark gray
    shape.setOrigin(25.0f, 25.0f);
    shape.setPosition(position);

    attackRate = 1.8f; // slower but stronger attacks
    attackCooldown = 0.f;
}

void Brute::render(sf::RenderWindow& window) {
    // Update shape position to match entity position
    shape.setPosition(position);
    window.draw(shape);

    // Draw health bar
    Enemy::render(window);
}
