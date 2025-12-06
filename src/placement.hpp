// placement.hpp
#pragma once
#include <SFML/System.hpp>
#include <vector>

inline const std::vector<sf::Vector2f>& getValidPlots() {
    static std::vector<sf::Vector2f> plots = {
        {200, 100}, {400, 100}, {600, 100}, {800, 100}, {1000,200},
        {400, 400}, {600, 400}, {800, 400},
        {200, 580}, {400, 580}, {600, 580}, {800, 580}, {1000,480}
    };
    return plots;
}
