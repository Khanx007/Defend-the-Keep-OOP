// UIManager.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

enum class TowerType { None, Archer, Cannon, Magic };

class UIManager {
public:
    UIManager(sf::RenderWindow& window);

    void render();
    void handleClick(sf::Vector2f mousePos);

    // value update and on screen update
    void updateGold(int newGold);
    void addGold(int delta);
    void setGold(int amount);
    int getGold() const;

    void updateWave(int wave);
    void setWave(int wave);
    void setWaveProgress(int remaining, int total);

    void updateCastleHealth(int hp);

    // Tower placement helpers
    bool isPlacingTower() const { return placingTowerType != TowerType::None; }
    TowerType getPlacingTowerType() const { return placingTowerType; }
    void cancelPlacing() { placingTowerType = TowerType::None; }

    // Upgrade & Heal UI
    bool isUpgradeMode() const { return upgrading; }     // UI waiting for a tower click to upgrade
    bool isHealMode() const { return false; }            // kept for future extension

    // Callbacks (set by main)
    // Called when UI wants the game to attempt upgrade at clicked position (world coords)
    std::function<void(sf::Vector2f)> onUpgradeRequest = nullptr;

    // Called when UI wants to heal castle; parameter = heal amount (int)
    std::function<void(int)> onHealRequested = nullptr;

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

    // New: upgrade & heal buttons
    sf::RectangleShape upgradeButton;
    sf::Text           upgradeButtonText;
    sf::RectangleShape healButton;
    sf::Text           healButtonText;

    sf::CircleShape    rangePreview;

    int currentGold = 400;
    int currentWave = 1;
    int castleHP    = 100;

    TowerType placingTowerType = TowerType::None;

    // Upgrade mode state & costs
    bool upgrading = false;
    int upgradeCost = 80;      // cost to attempt an upgrade (change as you wish)
    int healCost = 100;        // cost to heal castle
    int healAmount = 30;       // hp restored by healCost

    // Wave progress numbers (small display)
    int waveRemaining = 0;
    int waveTotal = 0;
};
