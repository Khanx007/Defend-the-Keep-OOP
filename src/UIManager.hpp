#pragma once
#include <SFML/Graphics.hpp>

enum class TowerType { None, Archer, Cannon, Magic };

class UIManager {
public:
    UIManager(sf::RenderWindow& window);

    // Rendering & input
    void render();
    void handleClick(sf::Vector2f mousePos);

    // Gold API
    // updateGold keeps backward compatibility as a setter (replace current value)
    void updateGold(int newGold);
    // addGold modifies (adds/subtracts) current gold safely
    void addGold(int delta);
    void setGold(int amount);
    int getGold() const;

    // Wave / castle HP UI
    void updateWave(int wave);                // setter (keeps old name)
    void setWave(int wave);                   // same effect; used by WaveManager callback
    void setWaveProgress(int remaining, int total);
    void updateCastleHealth(int hp);
    int getCastleHP() const { return castleHP; }

    // Placing towers
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

    // for wave progress display (optional)
    int waveRemaining = 0;
    int waveTotal = 0;

    TowerType placingTowerType = TowerType::None;
};
