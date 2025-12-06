#pragma once

#include <functional>
#include <string>

// Forward
class Enemy;

struct WaveConfig {
    int waveNumber = 0;
    int count = 0;
    float spawnInterval = 1.0f;
    float enemySpeedMultiplier = 1.0f;
    float enemyHealthMultiplier = 1.0f;
    float enemyDamageMultiplier = 1.0f;
};

class WaveManager {
public:
    // spawnFunc should create an Enemy* (and push it into any container the game uses).
    // It may return nullptr if spawn fails.
    WaveManager(std::function<Enemy*(const std::string& type)> spawnFunc = nullptr);

    // Start a wave manually
    void startWave(int waveNum);

    // Call every frame
    void update(float dt);

    // Notify manager that an enemy was removed (killed or reached castle)
    void onEnemyKilled();

    // Callbacks UI can subscribe to
    std::function<void(int waveNumber)> onWaveStart;
    // (remaining, total)
    std::function<void(int remaining, int total)> onWaveProgress;

    // Accessors
    bool isWaveActive() const;
    int getCurrentWave() const;

    int getEnemiesToSpawn() const;
    int getSpawnedCount() const;
    int getKilledCount() const;
    int getRemaining() const;

    

    // WaveManager.hpp (add to public section)
    void extendInterWaveDelay(float extraSeconds); // add extra seconds to the current inter-wave delay
    void setInterWaveDelay(float seconds);         // set the inter-wave delay to an absolute value
    float getInterWaveDelay() const;               // read current inter-wave delay


private:
    std::function<Enemy*(const std::string& type)> spawnEnemyFunc;

    WaveConfig currentConfig;

    int currentWave = 0;
    int enemiesToSpawn = 0;
    int spawnedCount = 0;
    int killedCount = 0;

    float nextSpawnTimer = 0.f;

    bool active = false;
    float interWaveDelay = 5.f;
    float interWaveTimer = 0.f;
};
