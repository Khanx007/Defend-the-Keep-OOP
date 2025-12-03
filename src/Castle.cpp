#include "Castle.hpp"
#include <iostream>

Castle::Castle() {
    // try to load a castle texture (if you have one named "castle.png" in working dir)
    bool texLoaded = false;
    if (texture.loadFromFile("castle.png")) {
        sprite.setTexture(texture);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
        texLoaded = true;
    } else {
        // fallback: simple circle to represent castle
        fallbackShape.setRadius(40.f);
        fallbackShape.setOrigin(40.f, 40.f);
        fallbackShape.setFillColor(sf::Color(120, 120, 120));
    }

    // Setup health bar visuals
    healthBarBg.setSize({120.f, 12.f});
    healthBarBg.setFillColor(sf::Color(80, 20, 20));
    healthBarBg.setOrigin(60.f, 6.f);

    healthBarFg.setSize({120.f, 12.f});
    healthBarFg.setFillColor(sf::Color::Green);
    healthBarFg.setOrigin(60.f, 6.f);

    // default starting position (can be overridden by caller)
    position = {980.f, 360.f};
    sprite.setPosition(position);
    fallbackShape.setPosition(position);
    healthBarBg.setPosition(position);
    healthBarFg.setPosition(position);
}

bool Castle::loadTexture(const std::string& path) {
    if (texture.loadFromFile(path)) {
        sprite.setTexture(texture);
        sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
        return true;
    }
    return false;
}

void Castle::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;

    // update health bar fg width
    float pct = (maxHealth > 0) ? (float)health / (float)maxHealth : 0.f;
    healthBarFg.setSize({120.f * pct, 12.f});
}

void Castle::render(sf::RenderWindow& window) {
    // draw sprite or fallback
    if (sprite.getTexture()) {
        sprite.setPosition(position);
        window.draw(sprite);
    } else {
        fallbackShape.setPosition(position);
        window.draw(fallbackShape);
    }

    // draw health bar above castle
    healthBarBg.setPosition(position.x, position.y - 60.f);
    healthBarFg.setPosition(position.x - (120.f * (1.0f - (float)health / maxHealth)) / 2.f, position.y - 60.f);
    window.draw(healthBarBg);
    window.draw(healthBarFg);
}
