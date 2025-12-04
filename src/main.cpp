// main.cpp - Testing Enemy Movement with Split Paths and Towers
#include <SFML/Graphics.hpp>
#include "UIManager.hpp"
#include "Grunt.h"
#include "Brute.h"
#include "Scout.h"
#include "ArcherTower.hpp"
#include "CannonTower.hpp"
#include "MageTower.hpp"
#include <iostream>
#include <cmath>


using namespace std; 
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Defend The Keep - Enemy Test");
    window.setFramerateLimit(60);

    UIManager ui(window);

    // ===== CREATE TWO PATHS FOR ENEMIES =====
    // Path splits into upper and lower lanes, then merges near castle
    
    // UPPER PATH (top lane)
    vector<sf::Vector2f> upperPath = {
        {50, 360},      // Start: far left, center
        {200, 360},     // Move right
        {200, 200},     // Split UP to top lane
        {800, 200},     // Move right along top lane
        {900, 300},     // Merge down toward castle
        {980, 360}      // End: castle entrance
    };
    
    // LOWER PATH (bottom lane)
    vector<sf::Vector2f> lowerPath = {
        {50, 360},  // Start: far left, center (same start)
        {200, 360}, // Move right
        {200, 520}, // Split DOWN to bottom lane
        {800, 520},   // Move right along bottom lane
        {900, 420},  // Merge up toward castle
        {980, 360} // End: castle entrance (same end)
    };

    // Draw BOTH paths as circles so you can see them
    vector<sf::CircleShape> pathMarkers;
    
    // Draw upper path in YELLOW
    for (const auto& point : upperPath) {
        sf::CircleShape marker(8.0f);
        marker.setPosition(point.x - 8, point.y - 8);
        marker.setFillColor(sf::Color(255, 255, 0, 150)); // Yellow
        pathMarkers.push_back(marker);
    }
    
    // Draw lower path in ORANGE
    for (const auto& point : lowerPath) {
        sf::CircleShape marker(8.0f);
        marker.setPosition(point.x - 8, point.y - 8);
        marker.setFillColor(sf::Color(255, 165, 0, 150)); // Orange
        pathMarkers.push_back(marker);
    }
    
    // Draw valid tower plots as green squares
    vector<sf::Vector2f> validPlots = {
        {200, 100}, {400, 100}, {600, 100}, {800, 100},
        {200, 400}, {400, 400}, {600, 400}, {800, 400}
    };

    vector<sf::RectangleShape> plotMarkers;
    for (const auto& plot : validPlots) {
        sf::RectangleShape square(sf::Vector2f(60.0f, 60.0f));
        square.setPosition(plot.x - 30.0f, plot.y - 30.0f);
        square.setFillColor(sf::Color(0, 255, 0, 50));  // Semi-transparent green
        square.setOutlineThickness(2);
        square.setOutlineColor(sf::Color(0, 255, 0, 150));
        plotMarkers.push_back(square);
    }

    // ===== CREATE TEST ENEMIES =====
    vector<Enemy*> enemies;
    vector<Tower*> towers;
    
    // Spawn enemies on BOTH paths for testing
    enemies.push_back(new Grunt(upperPath));    // Grunt takes top lane
    enemies.push_back(new Brute(lowerPath));    // Brute takes bottom lane
    enemies.push_back(new Scout(upperPath));    // Scout takes top lane

    cout << "=== ENEMY TEST STARTED ===" << endl;
    cout << "Red circle = Grunt (medium speed)" << endl;
    cout << "Gray circle = Brute (slow tank)" << endl;
    cout << "Cyan circle = Scout (fast)" << endl;
    cout << "Yellow dots = UPPER path" << endl;
    cout << "Orange dots = LOWER path" << endl;
    cout << "Green squares = Valid tower plots" << endl;
    cout << "\nWatch them move along TWO split paths!" << endl;
    cout << "\nControls:" << endl;
    cout << "  SPACE - Spawn more enemies (alternates paths)" << endl;
    cout << "  R - Reset/clear all enemies" << endl;

    sf::Clock clock;

    // ===== GAME LOOP =====
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds(); // clock: time since last frame

        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle UI clicks (tower placement)
            if (event.type == sf::Event::MouseButtonPressed && 
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                
                cout << "Click at: (" << worldPos.x << ", " << worldPos.y << ")" << endl;
                
                // First, let UI handle the click (this updates the placing state)
                bool wasPlacing = ui.isPlacingTower();
                TowerType placingType = ui.getPlacingTowerType();
                
                cout << "Before handleClick - wasPlacing: " << wasPlacing << endl;
                
                ui.handleClick(worldPos);
                
                cout << "After handleClick - isPlacing: " << ui.isPlacingTower() << endl;
                
                // Now check if a tower was just placed
                if (wasPlacing && placingType != ui.getPlacingTowerType()) {
                    // Find which plot was clicked
                    for (const auto& plot : validPlots) {
                        float dx = worldPos.x - plot.x;
                        float dy = worldPos.y - plot.y;
                        float dist = sqrt(dx*dx + dy*dy);
                        
                        if (dist < 40.0f) {
                            // Place tower here depending on type
                            if (placingType == TowerType::Archer) {
                                towers.push_back(new ArcherTower(plot));
                            } else if (placingType == TowerType::Cannon) {
                                towers.push_back(new CannonTower(plot));
                            } else if (placingType == TowerType::Magic) {
                                towers.push_back(new MageTower(plot));
                            }
                            cout << "✓ TOWER ACTUALLY PLACED at (" << plot.x << ", " << plot.y << ")" << endl;
                            break;
                        }
                    }
                }
            }

            // Press SPACE to spawn more enemies (alternating paths)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                // Alternate between upper and lower paths
                static bool useUpperPath = true;
                
                if (useUpperPath) {
                    enemies.push_back(new Grunt(upperPath));
                    enemies.push_back(new Scout(upperPath));
                    cout << "Spawned enemies on UPPER path!" << endl;
                } else {
                    enemies.push_back(new Brute(lowerPath));
                    enemies.push_back(new Grunt(lowerPath));
                    cout << "Spawned enemies on LOWER path!" << endl;
                }
                
                useUpperPath = !useUpperPath;
                cout << "Total enemies: " << enemies.size() << endl;
            }

            // Press R to reset/clear all enemies
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                for (auto* enemy : enemies) {
                    delete enemy;
                }
                enemies.clear();
                cout << "All enemies cleared!" << endl;
            }
        }

        // ===== UPDATE =====
        // Update all towers
        for (auto* tower : towers) {
            tower->update(dt, enemies);
        }
        
        // Update all enemies
        for (auto* enemy : enemies) {
            enemy->update(dt);
            enemy->resetColor();  // Reset hit color flash
        }

        // Remove dead or finished enemies
        for (int i = enemies.size() - 1; i >= 0; i--) {
            if (enemies[i]->isDead()) {
                int goldGained = enemies[i]->getGoldValue();
                cout << "Enemy died! Gained " << goldGained << " gold" << endl;
                ui.updateGold(ui.isPlacingTower() ? 0 : goldGained);  // Simple gold tracking
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
            }
            else if (enemies[i]->hasReachedEnd()) {
                cout << "Enemy reached castle! Lost " << enemies[i]->getDamage() << " HP" << endl;
                delete enemies[i];
                enemies.erase(enemies.begin() + i);
            }
        }

        // ===== RENDER =====
        window.clear(sf::Color(50, 150, 50));  // Grass green background

        // Draw valid tower plot markers (green squares)
        for (const auto& marker : plotMarkers) {
            window.draw(marker);
        }

        // Draw path markers (yellow and orange dots)
        for (const auto& marker : pathMarkers) {
            window.draw(marker);
        }

        // Draw all enemies
        for (auto* enemy : enemies) {
            enemy->render(window);
        }
        
        // Draw all towers
        for (auto* tower : towers) {
            tower->render(window);
        }

        // Draw UI (gold, wave, HP, tower buttons)
        ui.render();

        window.display();
    }

    // ===== CLEANUP =====
    for (auto* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
    
    for (auto* tower : towers) {
        delete tower;
    }
    towers.clear();

    cout << "Game closed. Goodbye!" << endl;
    return 0;
}
