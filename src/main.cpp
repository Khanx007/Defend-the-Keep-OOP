// main.cpp - Testing Enemy Movement with Split Paths and Towers
// Standard
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <functional>

// SFML
#include <SFML/Graphics.hpp>

// Project headers
#include "UIManager.hpp"
#include "placement.hpp"
#include "EnemyProjectile.hpp"
#include "WaveManager.hpp"
#include "Castle.hpp"
#include "Grunt.hpp"
#include "Scout.hpp"
#include "Brute.hpp"
#include "ArcherTower.hpp"
#include "CannonTower.hpp"
#include "MageTower.hpp"

using namespace std;

// ----- GAME STATE & STATS -----
enum class GameState { MENU, PLAYING, GAMEOVER };
GameState gameState = GameState::MENU;

int enemiesEliminatedCount = 0;
int defensesPlacedCount = 0;
int wavesSurvived = 0;
int currentWaveLocal = 0;

// UI elements for menu / endscreen (we create them in main())
sf::Text titleText;
sf::Text subtitleText;
sf::Text buttonStartText;
sf::Text buttonRestartText;
sf::RectangleShape buttonStartRect;
sf::RectangleShape buttonRestartRect;
sf::Font globalFont; // small font used by menu/end screens

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Defend The Keep - Enemy Test");
    window.setFramerateLimit(60);

    UIManager ui(window);

    // Load font for menu and gameover text. We assume "arial.TTF" exists in working dir (UIManager already uses it).
    if (!globalFont.loadFromFile("arial.TTF")) {
        std::cerr << "Warning: menu font not loaded (arial.TTF)\n";
    }

    // Title
    titleText.setFont(globalFont);
    titleText.setString("DEFEND THE KEEP");
    titleText.setCharacterSize(56);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(120.f, 80.f);

    // Subtitle
    subtitleText.setFont(globalFont);
    subtitleText.setString("Protect the castle from waves of intruders");
    subtitleText.setCharacterSize(20);
    subtitleText.setFillColor(sf::Color(200,200,200));
    subtitleText.setPosition(120.f, 150.f);

    // Start button
    buttonStartRect.setSize({280.f, 60.f});
    buttonStartRect.setFillColor(sf::Color(80,180,80));
    buttonStartRect.setPosition(120.f, 240.f);
    buttonStartRect.setOutlineThickness(3);
    buttonStartRect.setOutlineColor(sf::Color::White);

    buttonStartText.setFont(globalFont);
    buttonStartText.setString("START GAME");
    buttonStartText.setCharacterSize(24);
    buttonStartText.setFillColor(sf::Color::Black);
    buttonStartText.setPosition(buttonStartRect.getPosition().x + 20.f, buttonStartRect.getPosition().y + 12.f);

    // Restart button (used on GameOver screen)
    buttonRestartRect.setSize({220.f, 50.f});
    buttonRestartRect.setFillColor(sf::Color(120,200,120));
    buttonRestartRect.setPosition(120.f, 420.f);
    buttonRestartRect.setOutlineThickness(3);
    buttonRestartRect.setOutlineColor(sf::Color::White);

    buttonRestartText.setFont(globalFont);
    buttonRestartText.setString("RESTART");
    buttonRestartText.setCharacterSize(20);
    buttonRestartText.setFillColor(sf::Color::Black);
    buttonRestartText.setPosition(buttonRestartRect.getPosition().x + 20.f, buttonRestartRect.getPosition().y + 10.f);

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
    const auto& validPlots = getValidPlots();

    vector<sf::RectangleShape> plotMarkers;
    for (const auto& plot : validPlots) {
        sf::RectangleShape square(sf::Vector2f(60.0f, 60.0f));
        square.setPosition(plot.x - 30.0f, plot.y - 30.0f);
        square.setFillColor(sf::Color(0, 255, 0, 50));  // Semi-transparent green
        square.setOutlineThickness(2);
        square.setOutlineColor(sf::Color(0, 255, 0, 150));
        plotMarkers.push_back(square);
    }

    // ===== CREATE TEST ENEMIES & TOWERS =====
    vector<Enemy*> enemies;
    vector<Tower*> towers;

    std::vector<EnemyProjectile*> enemyProjectiles;

    // Create Castle
    Castle castle;
    castle.setPosition({980.f, 360.f}); // make sure matches path end

    // ===== WaveManager spawn function =====
    // corrected spawnFunc — captures castle too
    std::function<Enemy*(const std::string&)> spawnFunc =
        [&enemies, &upperPath, &lowerPath, &enemyProjectiles, &castle](const std::string& type) -> Enemy* {
            static bool useUpper = true;
            const auto& path = useUpper ? upperPath : lowerPath;
            useUpper = !useUpper;

            Enemy* e = nullptr;
            if (type == "Grunt") e = new Grunt(path);
            else if (type == "Scout") e = new Scout(path);
            else if (type == "Brute") e = new Brute(path);

            if (e) {
                enemies.push_back(e);

                // projectile spawner
                e->setProjectileSpawner([&enemyProjectiles](Enemy* self, sf::Vector2f origin, Entity* target, int damage) {
                    enemyProjectiles.push_back(new EnemyProjectile(origin, target, 300.f, damage));
                });

                // fallback = castle
                e->setFallbackTarget(&castle);
            }

            // STEP 4: SAFETY CHECK
            if (e) {
                if (!e->hasProjectileSpawner()) {
                    std::cout << "[ERROR] Enemy spawned WITHOUT projectile spawner! type=" << type << "\n";
                }
                if (!e->getFallbackTarget()) {
                    std::cout << "[ERROR] Enemy spawned WITHOUT fallback target! type=" << type << "\n";
                }
            }

            return e;
    };

    // Create WaveManager using spawnFunc
    WaveManager waveManager(spawnFunc);

    // Tell the UI when waves start and progress and capture wave locally
    waveManager.onWaveStart = [&ui](int w){
        ui.setWave(w);
        currentWaveLocal = w; // currentWaveLocal is global, no capture needed
    };

    waveManager.onWaveProgress = [&ui](int rem, int total){ ui.setWaveProgress(rem, total); };

    // Start first wave (but don't start until user hits Start in menu)
    // We'll start automatically when player presses START.

    cout << "=== ENEMY TEST STARTED ===" << endl;
    cout << "Red circle = Grunt (medium speed)" << endl;
    cout << "Gray circle = Brute (slow tank)" << endl;
    cout << "Cyan circle = Scout (fast)" << endl;
    cout << "Yellow dots = UPPER path" << endl;
    cout << "Orange dots = LOWER path" << endl;
    cout << "Green squares = Valid tower plots" << endl;
    cout << "\nWatch them move along TWO split paths!" << endl;
    cout << "\nControls:" << endl;
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

            // Mouse click handling -- behavior depends on game state
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i pixelPos(event.mouseButton.x, event.mouseButton.y);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                if (gameState == GameState::MENU) {
                    // Start button
                    if (buttonStartRect.getGlobalBounds().contains(worldPos)) {
                        // Reset stats & clear world (just in case)
                        enemiesEliminatedCount = 0;
                        defensesPlacedCount = 0;
                        wavesSurvived = 0;
                        currentWaveLocal = 0;

                        for (auto* e : enemies) delete e;
                        enemies.clear();
                        for (auto* t : towers) delete t;
                        towers.clear();
                        for (auto* p : enemyProjectiles) delete p;
                        enemyProjectiles.clear();

                        castle = Castle();
                        castle.setPosition({980.f, 360.f});

                        ui.updateGold(20000); // or whatever starting gold you want
                        waveManager.startWave(1);
                        gameState = GameState::PLAYING;
                        std::cout << "[Main] Game started from menu\n";
                    }
                }
                else if (gameState == GameState::GAMEOVER) {
                    // Restart button
                    if (buttonRestartRect.getGlobalBounds().contains(worldPos)) {
                        // Reset and restart
                        enemiesEliminatedCount = 0;
                        defensesPlacedCount = 0;
                        wavesSurvived = 0;
                        currentWaveLocal = 0;

                        for (auto* e : enemies) delete e;
                        enemies.clear();
                        for (auto* t : towers) delete t;
                        towers.clear();
                        for (auto* p : enemyProjectiles) delete p;
                        enemyProjectiles.clear();

                        castle = Castle();
                        castle.setPosition({980.f, 360.f});

                        ui.updateGold(20000); // reset gold UI
                        waveManager.startWave(1);
                        gameState = GameState::PLAYING;
                        std::cout << "[Main] Restarting game\n";
                    }
                }
                else if (gameState == GameState::PLAYING) {
                    // Gameplay: tower placement & UI handling
                    cout << "Click at: (" << worldPos.x << ", " << worldPos.y << ")" << endl;

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
                                Tower* newTower = nullptr;

                                // Place tower here depending on type
                                if (placingType == TowerType::Archer) {
                                    newTower = new ArcherTower(plot);
                                } else if (placingType == TowerType::Cannon) {
                                    newTower = new CannonTower(plot);
                                } else if (placingType == TowerType::Magic) {
                                    newTower = new MageTower(plot);
                                }

                                if (newTower) {
                                    newTower->setLifetime(30.0f); // tower will expire after 30s
                                    towers.push_back(newTower);
                                    defensesPlacedCount++;
                                    cout << "✓ TOWER ACTUALLY PLACED at (" << plot.x << ", " << plot.y << ")"
                                              << "  (total defenses placed: " << defensesPlacedCount << ")" << endl;
                                } else {
                                    cout << "Failed to create tower object (null)." << endl;
                                }
                                break;
                            }
                        }
                    }
                }
            } // end mouse click

            // KEY HANDLING (common, allow R to clear anytime)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                for (auto* enemy : enemies) {
                    delete enemy;
                }
                enemies.clear();

                for (auto* p : enemyProjectiles) {
                    delete p;
                }
                enemyProjectiles.clear();

                cout << "All enemies & enemy projectiles cleared!" << endl;
            }

            // optional debug spawn with SPACE while playing - won't affect waves
            if (gameState == GameState::PLAYING && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                // debug spawn (counts via spawnFunc)
                spawnFunc("Grunt");
                cout << "[DEBUG] Spawned manual Grunt via SPACE\n";
            }

        } // end event polling

        // ===== UPDATE =====
        if (gameState == GameState::PLAYING) {
            // Update wave manager (spawns enemies over time)
            waveManager.update(dt);

            // Update all towers
            for (auto* tower : towers) {
                tower->update(dt, enemies);
            }

            // --- Remove expired or destroyed towers (and clear enemy targets that pointed to them) ---
            for (int ti = static_cast<int>(towers.size()) - 1; ti >= 0; --ti) {
                Tower* t = towers[ti];
                if (!t) continue;

                // If tower lifetime ended or it was destroyed by damage, delete it.
                if (t->isExpired() || t->isDestroyed()) {
                    // Clear any enemy that had this tower as its attack target to avoid dangling pointers.
                    for (Enemy* e : enemies) {
                        if (!e) continue;
                        if (e->getAttackTarget() == t) {
                            e->clearAttackTarget();
                        }
                    }

                    // before deleting tower 't', remove any enemy projectiles targeting it
                    for (int pi = static_cast<int>(enemyProjectiles.size()) - 1; pi >= 0; --pi) {
                        EnemyProjectile* p = enemyProjectiles[pi];
                        if (!p) continue;
                        if (p->getTarget() == t) {
                            delete p;
                            enemyProjectiles.erase(enemyProjectiles.begin() + pi);
                        }
                    }

                    delete t;
                    towers.erase(towers.begin() + ti);
                    std::cout << "[Main] Tower expired/destroyed and removed.\n";
                }
            }

            // --- Enemy target acquisition (prioritize nearest tower, else castle) ---
            const float enemySearchRadius = 220.f; // tune this (in pixels) per your game balance

            for (Enemy* e : enemies) {
                if (!e) continue;
                if (e->isDead()) { e->clearAttackTarget(); continue; }

                // If enemy already has a valid target and it's alive, keep it.
                Entity* currentTarget = e->getAttackTarget();
                if (currentTarget) {
                    if (currentTarget->isAlive()) continue;
                    e->clearAttackTarget();
                }

                // Find nearest tower in range
                Tower* nearest = nullptr;
                float bestDist = 1e9f;
                for (Tower* t : towers) {
                    if (!t) continue;
                    if (t->isDestroyed()) continue;

                    float dx = e->getPosition().x - t->getPosition().x;
                    float dy = e->getPosition().y - t->getPosition().y;
                    float d = std::sqrt(dx*dx + dy*dy);
                    if (d <= enemySearchRadius && d < bestDist) {
                        bestDist = d;
                        nearest = t;
                    }
                }

                if (nearest) {
                    e->setAttackTarget(nearest);
                    continue;
                }

                // No tower found — check castle distance
                float dx = e->getPosition().x - castle.getPosition().x;
                float dy = e->getPosition().y - castle.getPosition().y;
                float dcastle = std::sqrt(dx*dx + dy*dy);
                if (dcastle <= enemySearchRadius) {
                    e->setAttackTarget(&castle);
                } else {
                    e->clearAttackTarget();
                }
            }

            // Update all enemies
            for (auto* enemy : enemies) {
                enemy->update(dt);
                enemy->resetColor();  // Reset hit color flash
            }

            // --- Update enemy projectiles (move, hit, remove) ---
            for (int pi = static_cast<int>(enemyProjectiles.size()) - 1; pi >= 0; --pi) {
                EnemyProjectile* p = enemyProjectiles[pi];
                if (!p) { enemyProjectiles.erase(enemyProjectiles.begin() + pi); continue; }

                p->update(dt);

                // If the projectile hit its target or target no longer exists, remove it
                if (p->shouldRemove()) {
                    delete p;
                    enemyProjectiles.erase(enemyProjectiles.begin() + pi);
                }
            }

            // Remove dead or finished enemies, award gold / damage castle / notify waves
            for (int i = static_cast<int>(enemies.size()) - 1; i >= 0; --i) {
                Enemy* e = enemies[i];

                if (e->isDead()) {
                    int goldGained = e->getGoldValue();
                    cout << "Enemy died! Gained " << goldGained << " gold" << endl;
                    ui.addGold(goldGained);

                    // increment enemy killed stat
                    enemiesEliminatedCount++;

                    // Only notify WaveManager once per enemy
                    if (!e->isCountedByWave()) {
                        waveManager.onEnemyKilled();
                        e->markCountedByWave();
                    }

                    delete e;
                    enemies.erase(enemies.begin() + i);
                }
                else if (e->hasReachedEnd()) {
                    // Enemy walked into castle area. Don't delete them immediately.
                    if (!e->isCountedByWave()) {
                        waveManager.onEnemyKilled();
                        e->markCountedByWave();
                        std::cout << "[Main] Enemy reached end: counted for wave (will now attack castle).\n";
                    }

                    if (e->getAttackTarget() == nullptr) {
                        e->setAttackTarget(&castle);
                    }

                    // let enemy continue attacking castle (do not delete)
                    continue;
                }
            }

            // Check for game over (castle destroyed)
            if (castle.isDestroyed()) {
                gameState = GameState::GAMEOVER;
                wavesSurvived = currentWaveLocal;
                std::cout << "[Main] GAME OVER. Waves survived: " << wavesSurvived << "\n";
                // stop spawning by not calling waveManager.update() while GAMEOVER
            }
        } // end PLAYING update

        // ===== RENDER =====
        window.clear(sf::Color(50, 150, 50));  // Grass green background

        if (gameState == GameState::MENU) {
            // Draw menu UI
            window.draw(titleText);
            window.draw(subtitleText);
            window.draw(buttonStartRect);
            window.draw(buttonStartText);
            ui.render();
        }
        else if (gameState == GameState::PLAYING) {
            // Draw game world as before
            for (const auto& marker : plotMarkers) window.draw(marker);
            for (const auto& marker : pathMarkers) window.draw(marker);

            castle.render(window);

            for (auto* enemy : enemies) enemy->render(window);
            for (auto* p : enemyProjectiles) p->render(window);

            for (auto* tower : towers) tower->render(window);

            ui.render();
        }
        else if (gameState == GameState::GAMEOVER) {
            // draw underlying game scene lightly (optional)
            for (const auto& marker : plotMarkers) window.draw(marker);
            for (const auto& marker : pathMarkers) window.draw(marker);
            castle.render(window);
            for (auto* enemy : enemies) enemy->render(window);
            for (auto* tower : towers) tower->render(window);

            // overlay
            sf::RectangleShape overlay(sf::Vector2f(1280.f, 720.f));
            overlay.setFillColor(sf::Color(0,0,0,160));
            window.draw(overlay);

            // Game over text
            sf::Text goTitle("GAME OVER", globalFont, 48);
            goTitle.setFillColor(sf::Color::Red);
            goTitle.setPosition(420.f, 80.f);
            window.draw(goTitle);

            // Stats
            std::string statsStr = "Waves survived: " + std::to_string(wavesSurvived) + "\n"
                                 + "Enemies eliminated: " + std::to_string(enemiesEliminatedCount) + "\n"
                                 + "Defenses placed: " + std::to_string(defensesPlacedCount);
            sf::Text statsText(statsStr, globalFont, 20);
            statsText.setFillColor(sf::Color::White);
            statsText.setPosition(420.f, 160.f);
            window.draw(statsText);

            // Restart button
            window.draw(buttonRestartRect);
            window.draw(buttonRestartText);
        }

        window.display();
    } // end while(window.isOpen())

    // ===== CLEANUP =====
    for (auto* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    for (auto* tower : towers) {
        delete tower;
    }
    towers.clear();

    for (auto* p : enemyProjectiles) {
        delete p;
    }
    enemyProjectiles.clear();

    cout << "Game closed. Goodbye!" << endl;
    return 0;
}
