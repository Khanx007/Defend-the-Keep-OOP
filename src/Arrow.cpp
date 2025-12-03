#include "Arrow.hpp"
#include <cmath>

Arrow::Arrow(sf::Vector2f pos, sf::Vector2f targetPos)
    : Projectile(pos, targetPos - pos, 400.0f, 20.0f, ProjectileType::ARROW)  // speed=400, damage=20
{
    // Create visual representation - small yellow circle
    shape.setRadius(5.0f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(5.0f, 5.0f);
    shape.setPosition(position);
}

void Arrow::render(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(shape);
}
