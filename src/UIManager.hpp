#pragma once
#include <SFML/Graphics.hpp>

enum class TowerType { None, Archer, Cannon, Magic };

class UIManager {
public:
    UIManager(sf::RenderWindow& window);

    void render();
    void handleClick(sf::Vector2f mousePos);

    // value update and on screen update
    void updateGold(int newGold);
    void updateWave(int wave);
    void updateCastleHealth(int hp);

    bool isPlacingTower() const { return placingTowerType != TowerType::None; }
    TowerType getPlacingTowerType() const { return placingTowerType; }
    void cancelPlacing() { placingTowerType = TowerType::None; }

private:
    sf::RenderWindow& window;

    sf::Font font;

    sf::Text goldText;
    sf::Text waveText;
    sf::Text healthText;

    sf::RectangleShape panelBg;
    sf::RectangleShape archerButton;
    sf::RectangleShape cannonButton;
    sf::RectangleShape magicButton;
    sf::Text           archerButtonText;
    sf::Text           cannonButtonText;
    sf::Text           magicButtonText;

    sf::CircleShape    rangePreview;

    int currentGold = 200;
    int currentWave = 1;
    int castleHP    = 100;

    TowerType placingTowerType = TowerType::None;
};
