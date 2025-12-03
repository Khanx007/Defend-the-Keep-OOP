#pragma once
#include "Enemy.hpp"

class Grunt : public Enemy {
public:
    Grunt(const std::vector<sf::Vector2f>& path);
    void render(sf::RenderWindow& window) override;

private:
    sf::CircleShape shape;
};
