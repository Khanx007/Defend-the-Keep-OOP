#pragma once
#include "Enemy.h"

class Brute : public Enemy {
public:
    Brute(const std::vector<sf::Vector2f>& path);
    void render(sf::RenderWindow& window) override;

private:
    sf::CircleShape shape;
};