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
    // This lambda will be used by WaveManager to create enemies into 'enemies' vector.
    // It alternates between upper and lower paths for variety.
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
    // Tell the UI when waves start and progress
    waveManager.onWaveStart = [&ui](int w){ ui.setWave(w); };
    waveManager.onWaveProgress = [&ui](int rem, int total){ ui.setWaveProgress(rem, total); };

    // Start first wave
    waveManager.startWave(1);

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
        // --- event loop (replacement) ---
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
                                // OPTIONAL: give towers a lifetime (seconds). 0 = permanent.
                                // You can tune this value per tower type if you want.
                                newTower->setLifetime(30.0f); // tower will expire after 30s

                                towers.push_back(newTower);
                                cout << "✓ TOWER ACTUALLY PLACED at (" << plot.x << ", " << plot.y << ")" << endl;
                            } else {
                                cout << "Failed to create tower object (null)." << endl;
                            }
                            break;
                        }
                    }
                }
            }

            // Press SPACE to spawn more enemies (use spawnFunc so wave accounting is correct)
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                static bool useUpperPath = true;

                if (spawnFunc) {
                    // spawn two enemies (these will be counted in the wave system)
                    spawnFunc("Grunt");
                    spawnFunc("Scout");
                    cout << "Spawned debug enemies via spawnFunc (counts in waves)." << endl;
                } else {
                    // fallback: if spawnFunc doesn't exist, keep your old behavior (not recommended)
                    // if (useUpperPath) {
                    //     enemies.push_back(new Grunt(upperPath));
                    //     enemies.push_back(new Scout(upperPath));
                    //     cout << "Spawned enemies on UPPER path (manual)!" << endl;
                    // } else {
                    //     enemies.push_back(new Brute(lowerPath));
                    //     enemies.push_back(new Grunt(lowerPath));
                    //     cout << "Spawned enemies on LOWER path (manual)!" << endl;
                    // }
                    // DEBUG ONLY: spawn 1 grunt instantly when pressing SPACE
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                    {
                        spawnFunc("Grunt");
                        cout << "[DEBUG] Spawned manual Grunt via SPACE\n";
                    }

                    useUpperPath = !useUpperPath;
                    cout << "Total enemies: " << enemies.size() << endl;
                }
            }

            // Press R to reset/clear all enemies & projectiles
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                for (auto* enemy : enemies) {
                    delete enemy;
                }
                enemies.clear();

                // Clear enemy projectiles (if you added the container)
                for (auto* p : enemyProjectiles) {
                    delete p;
                }
                enemyProjectiles.clear();

                cout << "All enemies & enemy projectiles cleared!" << endl;
            }
        } // end event loop

        // ===== UPDATE =====
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
                    // Use getAttackTarget() if available; otherwise, use pointer comparison via getter.
                    Entity* atk = nullptr;
                    // Try to call getAttackTarget safely if compiled in your Enemy class:
                    #ifdef ENABLE_GETATTACKTARGET
                    atk = e->getAttackTarget();
                    #else
                    // Fallback: we can't query the enemy's target; instead, ensure enemies reacquire targets next frame.
                    #endif

                    if (atk == t) {
                        e->clearAttackTarget();
                    }
                }

                // clear attackTarget from enemies
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


        // // Update enemy projectiles (before enemies so hits apply in same frame)
        // for (int i = static_cast<int>(enemyProjectiles.size()) - 1; i >= 0; --i) {
        //     enemyProjectiles[i]->update(dt);
        //     if (enemyProjectiles[i]->shouldRemove()) {
        //         delete enemyProjectiles[i];
        //         enemyProjectiles.erase(enemyProjectiles.begin() + i);
        //     }
        // }


        // --- Enemy target acquisition (prioritize nearest tower, else castle) ---
        const float enemySearchRadius = 220.f; // tune this (in pixels) per your game balance

        for (Enemy* e : enemies) {
            if (!e) continue;
            if (e->isDead() || e->hasReachedEnd()) {
                e->clearAttackTarget();
                continue;
            }

            // If enemy already has a valid target and it's alive, keep it.
            Entity* currentTarget = nullptr;
            #ifdef ENABLE_GETATTACKTARGET
            currentTarget = e->getAttackTarget();
            #endif
            if (currentTarget) {
                // If target is still alive and within a reasonable distance, keep it.
                if (currentTarget->isAlive()) {
                    // skip re-acquire
                    continue;
                } else {
                    e->clearAttackTarget();
                }
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
                // 1) Count them for the wave progress once (so waves advance correctly)
                if (!e->isCountedByWave()) {
                    waveManager.onEnemyKilled();
                    e->markCountedByWave();
                    std::cout << "[Main] Enemy reached end: counted for wave (will now attack castle).\n";
                }

                // 2) Ensure they target the castle so their attack logic (projectiles) runs.
                // Only set if they don't already have a target.
                if (e->getAttackTarget() == nullptr) {
                    e->setAttackTarget(&castle);
                }

                // 3) Do NOT delete them. Let Enemy::update() + projectiles handle castle damage over time.
                // Move on to next enemy (we purposely don't erase now).
                continue;
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

        // Draw Castle
        castle.render(window);

        // Draw all enemies
        for (auto* enemy : enemies) {
            enemy->render(window);
        }

        // Draw enemy projectiles
        for (auto* p : enemyProjectiles) {
            p->render(window);
        }

        // Draw all towers
        for (auto* tower : towers) {
            tower->render(window);
        }

        // Draw UI (gold, wave, HP, tower buttons)
        ui.render();

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
