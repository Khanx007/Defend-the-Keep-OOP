#include "Scout.hpp"
#include "AssetManager.hpp"
#include <iostream>

Scout::Scout(const std::vector<sf::Vector2f>& path)
    : Enemy(path, 75.f, 50, 6, 10)
{
    setAttackRange(120.f);
    setAttackRate(0.8f);
    setAttackDamage(10);

    sf::Texture& tex = AssetManager::getTexture("scout");
    sprite.setTexture(tex);

    sf::Vector2u s = tex.getSize();
    sprite.setOrigin(s.x/2, s.y/2);
    sprite.setScale(0.14f, 0.14f);

    sprite.setPosition(position);
}

void Scout::render(sf::RenderWindow& window)
{
    sprite.setPosition(position);
    window.draw(sprite);
    Enemy::render(window);
}
