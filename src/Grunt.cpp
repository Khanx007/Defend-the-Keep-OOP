// Grunt.cpp
#include "Grunt.hpp"
#include "AssetManager.hpp"   // ensure this file exists and is compiled into the project
#include <iostream>

Grunt::Grunt(const std::vector<sf::Vector2f>& path)
    : Enemy(path,
            50.0f,    // speed (medium)
            50,       // health (low)
            10,       // damage to castle
            10)       // gold value
{
    // Attack configuration (keeps previous behavior)
    setAttackRange(120.f);   // grunt fires from ~120 px away
    setAttackRate(1.2f);     // one shot every 1.2 seconds
    setAttackDamage(10);

    // Fallback visual: red circle
    shape.setRadius(15.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(15.0f, 15.0f);
    shape.setPosition(position);

    // Try to set sprite texture from AssetManager (id: "grunt")
    sf::Texture& tex = AssetManager::getTexture("grunt");
    if (tex.getSize().x > 0 && tex.getSize().y > 0) {
        sprite.setTexture(tex);
        // centre the sprite on position
        sprite.setOrigin(static_cast<float>(tex.getSize().x) / 2.f,
                         static_cast<float>(tex.getSize().y) / 2.f);
        sprite.setPosition(position);

        // Optional: automatic scaling to roughly match the fallback circle size.
        // If your image frames are large you may want to scale them down.
        const float desiredRadius = 15.f; // half-size we used for circle
        float texWidth = static_cast<float>(tex.getSize().x);
        float scale = (desiredRadius * 2.f) / texWidth;
        // only apply sensible scale if tex is larger than desired radius
        if (scale > 0.0f && scale < 1.5f) {
            sprite.setScale(scale, scale);
        }
    } else {
        // Texture not loaded — continue using fallback shape
        static bool warned = false;
        if (!warned) {
            std::cerr << "[Grunt] Asset 'grunt' not found in AssetManager. Using fallback circle.\n";
            warned = true;
        }
    }

    // initialize attack timers (keeps compatibility)
    attackRate = 1.2f;
    attackCooldown = 0.f;
}

void Grunt::render(sf::RenderWindow& window) {
    // Draw sprite if texture present, otherwise draw fallback shape
    if (sprite.getTexture() != nullptr && sprite.getTexture()->getSize().x > 0) {
        sprite.setPosition(position);
        window.draw(sprite);
    } else {
        shape.setPosition(position);
        window.draw(shape);
    }

    // Draw health bar via base class
    Enemy::render(window);
}
