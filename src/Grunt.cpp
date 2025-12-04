#include "Grunt.h"

Grunt::Grunt(const std::vector<sf::Vector2f>& path)
    : Enemy(path, 
            50.0f,    // speed (medium)
            50,        // health (low)
            10,        // damage to castle
            10)        // gold value
{
    // Create visual representation - red circle
    shape.setRadius(15.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(15.0f, 15.0f);
    shape.setPosition(position);
}

void Grunt::render(sf::RenderWindow& window) {
    // Update shape position to match entity position
    shape.setPosition(position);
    window.draw(shape);
    
    // Draw health bar
    Enemy::render(window);  // This calls the base class health bar drawing
}