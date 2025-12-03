#include "Castle.hpp"
#include <iostream>

Castle::Castle() {
    // Try to load a texture if available. If not, we'll use a rectangle shape.
    bool texLoaded = false;
    // Replace "assets/castle.png" with your project's asset path if available.
    if (texture.loadFromFile("assets/castle.png")) {
        sprite.setTexture(texture);
        sprite.setOrigin(sprite.getGlobalBounds().width/2.f, sprite.getGlobalBounds().height/2.f);
        texLoaded = true;
    } else {
        // fallback rectangle
        body.setSize({120.f, 120.f});
        body.setOrigin(body.getSize() / 2.f);
        body.setFillColor(sf::Color(120, 120, 200));
    }

    // default position (you can set via setPosition)
    sf::Vector2f defaultPos(780.f, 350.f);
    if (texLoaded) sprite.setPosition(defaultPos);
    body.setPosition(defaultPos);
}

void Castle::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
    // Simple feedback
    std::cout << "[Castle] took " << amount << " dmg, health=" << health << "\n";
}

sf::Vector2f Castle::getPosition() const {
    if (sprite.getTexture()) return sprite.getPosition();
    return body.getPosition();
}

void Castle::setPosition(const sf::Vector2f& pos) {
    if (sprite.getTexture()) sprite.setPosition(pos);
    body.setPosition(pos);
}

void Castle::render(sf::RenderWindow& window) {
    if (sprite.getTexture()) window.draw(sprite);
    else window.draw(body);

    // Draw HP bar above castle
    sf::RectangleShape bg({140.f, 12.f});
    bg.setFillColor(sf::Color(40,40,40));
    bg.setPosition(getPosition().x - 70.f, getPosition().y - 90.f);
    window.draw(bg);

    float pct = std::max(0.f, (float)health / 100.f);
    sf::RectangleShape fg({140.f * pct, 12.f});
    fg.setFillColor(sf::Color::Red);
    fg.setPosition(bg.getPosition());
    window.draw(fg);
}
