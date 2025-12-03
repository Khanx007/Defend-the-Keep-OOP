#include "WaveManager.hpp"
#include "Enemy.hpp"
#include <algorithm>
#include <iostream>

static constexpr bool WAVEMANAGER_DEBUG = false; // set true during debugging

WaveManager::WaveManager(std::function<Enemy*(const std::string& type)> spawnFunc)
: spawnEnemyFunc(spawnFunc),
  currentWave(0),
  enemiesToSpawn(0),
  spawnedCount(0),
  killedCount(0),
  nextSpawnTimer(0.f),
  active(false),
  interWaveDelay(5.f),
  interWaveTimer(0.f)
{
}

void WaveManager::startWave(int waveNum) {
    currentWave = waveNum;
    currentConfig.waveNumber = waveNum;
    currentConfig.count = 6 + waveNum * 2;
    currentConfig.spawnInterval = std::max(0.25f, 1.0f - (waveNum * 0.03f));
    currentConfig.enemySpeedMultiplier = 1.0f + waveNum * 0.05f;

    enemiesToSpawn = currentConfig.count;
    spawnedCount = 0;
    killedCount = 0;
    nextSpawnTimer = 0.f;
    active = true;
    interWaveTimer = 0.f;

    if (onWaveStart) onWaveStart(currentWave);
    if (onWaveProgress) onWaveProgress(std::max(0, enemiesToSpawn - killedCount), enemiesToSpawn);

    if (WAVEMANAGER_DEBUG) {
        std::cout << "[WaveManager] Started wave " << currentWave << ", count=" << enemiesToSpawn << "\n";
    }
}

void WaveManager::update(float dt) {
    if (!active) {
        if (interWaveTimer > 0.f) {
            interWaveTimer -= dt;
            if (interWaveTimer <= 0.f) {
                startWave(currentWave + 1);
            }
        }
        return;
    }

    if (spawnedCount < enemiesToSpawn) {
        nextSpawnTimer -= dt;
        if (nextSpawnTimer <= 0.f) {
            std::string type = "Grunt";
            int idx = spawnedCount % 8;
            if (idx == 3) type = "Scout";
            else if (idx == 6) type = "Brute";

            Enemy* e = nullptr;
            if (spawnEnemyFunc) e = spawnEnemyFunc(type);

            if (e) {
                e->multiplySpeed(currentConfig.enemySpeedMultiplier);
            } else {
                if (WAVEMANAGER_DEBUG) std::cerr << "[WaveManager] spawn returned null for type=" << type << "\n";
            }

            ++spawnedCount;
            if (spawnedCount > enemiesToSpawn) spawnedCount = enemiesToSpawn;
            nextSpawnTimer = currentConfig.spawnInterval;

            if (onWaveProgress) onWaveProgress(std::max(0, enemiesToSpawn - killedCount), enemiesToSpawn);
        }
    } else {
        // all spawned: end when killedCount >= enemiesToSpawn
        if (killedCount >= enemiesToSpawn) {
            active = false;
            interWaveTimer = interWaveDelay;
            if (onWaveProgress) onWaveProgress(0, enemiesToSpawn);
            if (WAVEMANAGER_DEBUG) {
                std::cout << "[WaveManager] Wave " << currentWave << " complete. Next in " << interWaveDelay << "s\n";
            }
        }
    }

    if (WAVEMANAGER_DEBUG) {
        std::cout << "[WaveDebug] wave="<<currentWave
                  << " spawned="<<spawnedCount<<"/"<<enemiesToSpawn
                  << " killed="<<killedCount
                  << " nextSpawnTimer="<<nextSpawnTimer
                  << " active="<<active << std::endl;
    }
}

void WaveManager::onEnemyKilled() {
    ++killedCount;
    if (killedCount > enemiesToSpawn) killedCount = enemiesToSpawn;

    int remaining = std::max(0, enemiesToSpawn - killedCount);
    if (onWaveProgress) onWaveProgress(remaining, enemiesToSpawn);

    if (WAVEMANAGER_DEBUG) {
        std::cout << "[WaveManager] onEnemyKilled: killed=" << killedCount
                  << " remaining=" << remaining << " total=" << enemiesToSpawn << "\n";
    }
}

// Accessors
bool WaveManager::isWaveActive() const { return active; }
int WaveManager::getCurrentWave() const { return currentWave; }
int WaveManager::getEnemiesToSpawn() const { return enemiesToSpawn; }
int WaveManager::getSpawnedCount() const { return spawnedCount; }
int WaveManager::getKilledCount() const { return killedCount; }
int WaveManager::getRemaining() const { return std::max(0, enemiesToSpawn - killedCount); }

void WaveManager::setInterWaveDelay(float seconds) {
    if (seconds < 0.f) seconds = 0.f;
    interWaveDelay = seconds;
}
