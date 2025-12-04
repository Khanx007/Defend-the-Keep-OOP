#include "Brute.hpp"
#include "AssetManager.hpp"
#include <iostream>

Brute::Brute(const std::vector<sf::Vector2f>& path)
    : Enemy(path, 65.f, 75, 30, 50)
{
    setAttackRange(120.f);
    setAttackRate(1.8f);
    setAttackDamage(10);

    sf::Texture& tex = AssetManager::getTexture("brute");
    sprite.setTexture(tex);

    sf::Vector2u s = tex.getSize();
    sprite.setOrigin(s.x/2, s.y/2);
    sprite.setScale(0.14f, 0.14f);

    sprite.setPosition(position);
}

void Brute::render(sf::RenderWindow& window)
{
    sprite.setPosition(position);
    window.draw(sprite);
    Enemy::render(window);
}
