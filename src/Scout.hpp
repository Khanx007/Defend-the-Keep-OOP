#pragma once
#include "Enemy.hpp"

class Scout : public Enemy {
public:
    Scout(const std::vector<sf::Vector2f>& path);
    void render(sf::RenderWindow& window) override;

private:
    sf::CircleShape shape;
};
