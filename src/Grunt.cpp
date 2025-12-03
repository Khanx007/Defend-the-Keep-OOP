#include "Grunt.hpp"

Grunt::Grunt(const std::vector<sf::Vector2f>& path)
    : Enemy(path,
            50.0f,    // speed (medium)
            50,       // health (low)
            10,       // damage to castle
            10)       // gold value
{
    // Create visual representation - red circle
    shape.setRadius(15.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(15.0f, 15.0f);
    shape.setPosition(position);

    // Example: set a slightly slower attack rate for grunt if it attacks towers/castle repeatedly
    attackRate = 1.2f;
    attackCooldown = 0.f;
}
    
void Grunt::render(sf::RenderWindow& window) {
    // Update shape position to match entity position
    shape.setPosition(position);
    window.draw(shape);

    // Draw health bar via base class
    Enemy::render(window);
}
