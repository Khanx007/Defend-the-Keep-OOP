// UIManager.cpp
#include "UIManager.hpp"
#include <iostream>
#include <cmath>
#include <string>
#include "placement.hpp"

using namespace std;

static float length(const sf::Vector2f& v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

UIManager::UIManager(sf::RenderWindow& window): window(window) {

    //Load arial font
    if(!font.loadFromFile("assets/fonts/arial.ttf")){
        cerr<< "ERROR: could not load the font" << endl;
    }

    // Setup Gold text (top right)
    goldText.setFont(font);
    goldText.setCharacterSize(28);
    goldText.setStyle(sf::Text::Bold);
    goldText.setFillColor(sf::Color::White);
    goldText.setPosition(1030.f, 20.f); // top right
    goldText.setString("Gold: " + to_string(currentGold));

    // Setup Wave text
    waveText.setFont(font);
    waveText.setCharacterSize(28);
    waveText.setStyle(sf::Text::Bold);
    waveText.setFillColor(sf::Color::White);
    waveText.setPosition(1030.f, 70.f); // top right
    waveText.setString("Wave: " + to_string(currentWave));

    // Setup castle health text
    healthText.setFont(font);
    healthText.setCharacterSize(28);
    healthText.setStyle(sf::Text::Bold);
    healthText.setFillColor(sf::Color::White);
    healthText.setPosition(1030.f, 120.f); // top right
    healthText.setString("HP: " + to_string(castleHP));

    // Right side dark panel
    panelBg.setSize(sf::Vector2f(250.f, 720.f));
    panelBg.setPosition(1030.f, 0.f);
    panelBg.setFillColor(sf::Color(0, 0, 0, 180));        // semi-transparent black

    // Set up archer button and text
    archerButton.setSize(sf::Vector2f(200.f, 70.f));
    archerButton.setPosition(1055.f, 150.f);
    archerButton.setFillColor(sf::Color(70, 70, 70, 255));
    archerButton.setOutlineThickness(4);
    archerButton.setOutlineColor(sf::Color::Yellow);

    archerButtonText.setFont(font);
    archerButtonText.setCharacterSize(18);
    archerButtonText.setFillColor(sf::Color::White);
    archerButtonText.setStyle(sf::Text::Bold);
    archerButtonText.setString("Archer\n50 Gold");
    archerButtonText.setPosition(1080.f, 160.f);

    // Cannon button
    cannonButton.setSize(sf::Vector2f(200.f, 70.f));
    cannonButton.setPosition(1055.f, 230.f);
    cannonButton.setFillColor(sf::Color(70,70,70));
    cannonButton.setOutlineThickness(4);
    cannonButton.setOutlineColor(sf::Color::Yellow);
    cannonButtonText.setFont(font);
    cannonButtonText.setCharacterSize(18);
    cannonButtonText.setFillColor(sf::Color::White);
    cannonButtonText.setStyle(sf::Text::Bold);
    cannonButtonText.setString("Cannon\n100 Gold");
    cannonButtonText.setPosition(1080.f, 240.f);

    // Magic button
    magicButton.setSize(sf::Vector2f(200.f, 70.f));
    magicButton.setPosition(1055.f, 310.f);
    magicButton.setFillColor(sf::Color(70,70,70));
    magicButton.setOutlineThickness(4);
    magicButton.setOutlineColor(sf::Color::Yellow);
    magicButtonText.setFont(font);
    magicButtonText.setCharacterSize(18);
    magicButtonText.setFillColor(sf::Color::White);
    magicButtonText.setStyle(sf::Text::Bold);
    magicButtonText.setString("Mage\n120 Gold");
    magicButtonText.setPosition(1080.f, 320.f);

    // Upgrade button (click + then click a placed tower to upgrade)
    upgradeButton.setSize(sf::Vector2f(200.f, 50.f));
    upgradeButton.setPosition(1055.f, 390.f);
    upgradeButton.setFillColor(sf::Color(60, 120, 200));
    upgradeButton.setOutlineThickness(3);
    upgradeButton.setOutlineColor(sf::Color::White);

    upgradeButtonText.setFont(font);
    upgradeButtonText.setCharacterSize(16);
    upgradeButtonText.setFillColor(sf::Color::White);
    upgradeButtonText.setStyle(sf::Text::Bold);
    upgradeButtonText.setString("Upgrade Tower");
    upgradeButtonText.setPosition(1080.f, 396.f);

    // Heal button (spend gold to heal castle)
    healButton.setSize(sf::Vector2f(200.f, 50.f));
    healButton.setPosition(1055.f, 460.f);
    healButton.setFillColor(sf::Color(150, 60, 60));
    healButton.setOutlineThickness(3);
    healButton.setOutlineColor(sf::Color::White);

    healButtonText.setFont(font);
    healButtonText.setCharacterSize(16);
    healButtonText.setFillColor(sf::Color::White);
    healButtonText.setStyle(sf::Text::Bold);
    healButtonText.setString("Heal Castle\n100 Gold (+60 HP)");
    healButtonText.setPosition(1080.f, 466.f);

    rangePreview.setRadius(150.f);                           // Archer range default
    rangePreview.setFillColor(sf::Color(0, 255, 0, 50));     // green + low opacity
    rangePreview.setOutlineThickness(3);
    rangePreview.setOutlineColor(sf::Color(0, 255, 0, 200));
    rangePreview.setOrigin(150.f, 150.f);                    // center of circle
    rangePreview.setPosition(-9999.f, -9999.f);              // hide off-screen initially

     
    currentGold = 700;
    currentWave = 1;
    castleHP    = 100;
    waveRemaining = 0;
    waveTotal = 0;

    upgrading = false;
    upgradeCost = 120;
    healCost = 100;
    healAmount = 60;
}

void UIManager::render(){

    // Draw panel
    window.draw(panelBg);

    // Keep text in sync (in case someone changed values externally)
    goldText.setString("Gold: " + to_string(currentGold));
    waveText.setString("Wave: " + to_string(currentWave));
    healthText.setString("HP: " + to_string(castleHP));

    // Draw HUD elements & tower buttons/label
    window.draw(goldText);
    window.draw(waveText);
    // window.draw(healthText);

    window.draw(archerButton);
    window.draw(archerButtonText);
    window.draw(cannonButton);
    window.draw(cannonButtonText);
    window.draw(magicButton);
    window.draw(magicButtonText);

    // new buttons
    window.draw(upgradeButton);
    window.draw(upgradeButtonText);
    window.draw(healButton);
    window.draw(healButtonText);

    // Optionally render small progress text for wave remaining / total
    if (waveTotal > 0) {
        sf::Text progress;
        progress.setFont(font);
        progress.setCharacterSize(14);
        progress.setFillColor(sf::Color::White);
        progress.setPosition(1030.f, 100.f);
        progress.setString("Remaining: " + to_string(waveRemaining) + " / " + to_string(waveTotal));
        window.draw(progress);
    }

    // When player tries to place a tower, respond to his cursor movement
    if (placingTowerType != TowerType::None || upgrading){

        // Get pixel coord, convert to real coord
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

        float previewRadius = 150.f;
        if (placingTowerType == TowerType::Archer) previewRadius = 150.f;
        if (placingTowerType == TowerType::Cannon) previewRadius = 180.f;
        if (placingTowerType == TowerType::Magic) previewRadius = 160.f;
        // if upgrading show a small highlight circle
        if (upgrading) previewRadius = 30.f;

        rangePreview.setRadius(previewRadius);
        rangePreview.setOrigin(previewRadius, previewRadius);
        rangePreview.setPosition(worldPos);

        const auto& validPlots = getValidPlots();

        bool onValidPlot = false;
        for (const auto& plot : validPlots)
        {
            if (length(worldPos - plot) < 40.0f) // placement radius
            {
                onValidPlot = true;
                break;
            }
        }

        if (onValidPlot)
        {
            rangePreview.setFillColor(sf::Color(0, 255, 0, 60));   // GREEN = valid
            rangePreview.setOutlineColor(sf::Color(0, 255, 0, 180));
        }
        else
        {
            rangePreview.setFillColor(sf::Color(255, 0, 0, 60));   // RED = invalid
            rangePreview.setOutlineColor(sf::Color(255, 0, 0, 180));
        }

        window.draw(rangePreview);
    }
}

void UIManager::handleClick(sf::Vector2f mousePos)
{
    // Handle tower-purchase buttons first (unchanged)
    if (archerButton.getGlobalBounds().contains(mousePos) && currentGold >= 50){
        placingTowerType = TowerType::Archer;
        archerButton.setFillColor(sf::Color(100, 200, 100)); // green highlight
        cout << "Placing Archer Tower! Click on map." << endl;
        return;
    }

    if (cannonButton.getGlobalBounds().contains(mousePos) && currentGold >= 100){
        placingTowerType = TowerType::Cannon;
        cannonButton.setFillColor(sf::Color(100,200,100));
        cout << "Placing Cannon Tower! Click on map." << endl;
        return;
    }

    if (magicButton.getGlobalBounds().contains(mousePos) && currentGold >= 120){
        placingTowerType = TowerType::Magic;
        magicButton.setFillColor(sf::Color(100,200,100));
        cout << "Placing Mage Tower! Click on map." << endl;
        return;
    }

    // Upgrade button clicked -> enable upgrade mode (next click chooses tower)
    if (upgradeButton.getGlobalBounds().contains(mousePos)) {
        if (currentGold >= upgradeCost) {
            upgrading = !upgrading; // toggle
            upgradeButton.setFillColor(upgrading ? sf::Color(100,180,250) : sf::Color(60,120,200));
            cout << (upgrading ? "Upgrade mode ON: Click a tower to upgrade." : "Upgrade mode OFF") << endl;
        } else {
            cout << "Not enough gold to upgrade. Need " << upgradeCost << " gold.\n";
        }
        return;
    }

    // Heal button clicked -> immediate heal if enough gold
    if (healButton.getGlobalBounds().contains(mousePos)) {
        if (currentGold >= healCost) {
            addGold(-healCost);
            if (onHealRequested) onHealRequested(healAmount);
            cout << "Requested castle heal: +" << healAmount << " HP for " << healCost << " gold.\n";
        } else {
            cout << "Not enough gold to heal castle. Need " << healCost << " gold.\n";
        }
        return;
    }

    // If currently upgrading, the next click is interpreted as upgrade target selection.
    if (upgrading) {
        // Pass world position back to main (so main can detect which tower was clicked)
        if (onUpgradeRequest) {
            onUpgradeRequest(mousePos); // main should handle finding tower at position and performing upgrade
            // Deduct gold only if main confirms success; but to keep UI responsive, we deduct here
            // and if main returns failure you can addGold(upgradeCost) back, or instead have main
            // return bool and only deduct here when successful. For simplicity we'll deduct here
            // and assume main will perform upgrade.
            addGold(-upgradeCost);
            cout << "Upgrade requested at (" << mousePos.x << ", " << mousePos.y << "). Cost: " << upgradeCost << "\n";
        } else {
            cout << "[UI] No onUpgradeRequest callback set. Cannot upgrade.\n";
        }
        // turn off upgrade mode after request
        upgrading = false;
        upgradeButton.setFillColor(sf::Color(60,120,200));
        return;
    }

    // If placing tower, try to place it (existing behaviour)
    if (placingTowerType != TowerType::None){
        const auto& validPlots = getValidPlots();

        // Check if clicked on a valid spot
        for (const auto& plot: validPlots){
            if (length(mousePos - plot) < 40.0f) {// 80 pxls diameter clickable area
                // Deduct gold depending on tower type (use addGold so we don't overwrite)
                if (placingTowerType == TowerType::Archer) addGold(-50);
                else if (placingTowerType == TowerType::Cannon) addGold(-100);
                else if (placingTowerType == TowerType::Magic) addGold(-120);

                // updateGold kept for compatibility (text will be updated in render anyway)
                updateGold(currentGold);
                placingTowerType = TowerType::None; // reset
                archerButton.setFillColor(sf::Color(70, 70, 70));  // normal color
                cannonButton.setFillColor(sf::Color(70,70,70));
                magicButton.setFillColor(sf::Color(70,70,70));
                cout << "Tower placed at (" << plot.x << "," << plot.y << ")! Current gold: " << currentGold << "\n";
                return;
            }
        }
        // Placed in wrong spot -> display msg
        cout << "Invalid plot! Click on green square areas only." << endl;
        return;
    }

    // if clicked somewhere else, cancel placing
    if (placingTowerType != TowerType::None)
    {
        cancelPlacing();
        archerButton.setFillColor(sf::Color(70, 70, 70));  // reset color
        cannonButton.setFillColor(sf::Color(70,70,70));
        magicButton.setFillColor(sf::Color(70,70,70));
        std::cout << "Placing cancelled.\n";
    }
}

void UIManager::updateGold(int newGold){
    if (newGold < 0) newGold = 0;
    currentGold = newGold;
    goldText.setString("Gold: "+ to_string(currentGold));
}

void UIManager::addGold(int delta) {
    long long temp = (long long)currentGold + (long long)delta;
    if (temp < 0) temp = 0;
    currentGold = static_cast<int>(temp);
    goldText.setString("Gold: " + to_string(currentGold));
}

void UIManager::setGold(int amount) {
    updateGold(amount);
}

int UIManager::getGold() const {
    return currentGold;
}

void UIManager::updateWave(int wave){
    currentWave = wave;
    waveText.setString("Wave: "+ to_string(currentWave));
}

void UIManager::setWave(int wave) {
    updateWave(wave);
}

void UIManager::setWaveProgress(int remaining, int total) {
    waveRemaining = remaining;
    waveTotal = total;
    
}

void UIManager::updateCastleHealth(int hp){
    castleHP = hp;
    if (castleHP < 0) castleHP = 0;
    healthText.setString("HP: " + to_string(castleHP));

    // If hp < 0, color it red
    if (castleHP <= 0){
        healthText.setFillColor(sf::Color::Red);
    } else {
        healthText.setFillColor(sf::Color::White);
    }
}
