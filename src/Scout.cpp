#include "Scout.hpp"

Scout::Scout(const std::vector<sf::Vector2f>& path)
    : Enemy(path,
            75.0f, // speed (very fast)
            30,    // health (very low)
            15,    // damage to castle
            25)    // gold value
{
    // Create visual representation - cyan circle (smaller)
    shape.setRadius(12.0f);
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(12.0f, 12.0f);
    shape.setPosition(position);

    attackRate = 0.8f; // faster attack if close
    attackCooldown = 0.f;
}

void Scout::render(sf::RenderWindow& window) {
    // Update shape position to match entity position
    shape.setPosition(position);
    window.draw(shape);

    // Draw health bar
    Enemy::render(window);
}
