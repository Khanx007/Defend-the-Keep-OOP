#include "Castle.hpp"
#include "AssetManager.hpp"   
#include <iostream>

Castle::Castle() {
    // Try to get 'castle' texture from AssetManager (should be loaded in main)
    sf::Texture& tex = AssetManager::getTexture("castle");

    if (tex.getSize().x == 0 || tex.getSize().y == 0) {
        // AssetManager returned an empty texture -> use fallback
        std::cerr << "[Castle] Warning: castle texture not found, using fallback.\n";
        // Create a simple grey fallback image (64x64)
        fallbackTexture.create(64, 64);
        sf::Image img;
        img.create(64, 64, sf::Color(150,150,150));
        fallbackTexture.update(img);
        sprite.setTexture(fallbackTexture);
    } else {
        sprite.setTexture(tex);
    }

    // Center origin so position corresponds to center
    sf::Vector2u size = sprite.getTexture() ? sprite.getTexture()->getSize() : fallbackTexture.getSize();
    sprite.setOrigin(float(size.x)/2.f, float(size.y)/2.f);

    // Scale to a reasonable on-screen width (tweak if required)
    const float desiredWidth = 140.f; // px on screen
    float scale = desiredWidth / float(size.x);
    if (scale <= 0.f) scale = 1.f;
    sprite.setScale(scale, scale);

    // Default position -- caller will setPosition as needed
    position = {980.f, 360.f};
    sprite.setPosition(position);

    maxHealth = 1000;
    health = maxHealth;
}

void Castle::takeDamage(int amount) {
    if (amount <= 0) return;
    health -= amount;
    if (health < 0) health = 0;
}

bool Castle::isAlive() const {
    return health > 0;
}

void Castle::drawHealthBar(sf::RenderWindow& window) {
    const float barWidth = 160.f;
    const float barHeight = 10.f;
    sf::RectangleShape bg(sf::Vector2f(barWidth, barHeight));
    bg.setFillColor(sf::Color(40,40,40,220));
    bg.setPosition(position.x - barWidth/2.f, position.y - 90.f);

    float pct = float(health) / float(maxHealth);
    if (pct < 0.f) pct = 0.f;
    sf::RectangleShape fg(sf::Vector2f(barWidth * pct, barHeight));
    fg.setFillColor(sf::Color::Green);
    fg.setPosition(position.x - barWidth/2.f, position.y - 90.f);

    window.draw(bg);
    window.draw(fg);
}

void Castle::render(sf::RenderWindow& window) {
    // ensure sprite follows position
    sprite.setPosition(position);
    window.draw(sprite);
    drawHealthBar(window);
}



void Castle::heal(int amount) {
    if (amount <= 0) return;
    health += amount;
    if (health > maxHealth) health = maxHealth;
}





