// WaveManager.cpp
#include "WaveManager.hpp"
#include "Enemy.hpp"
#include <algorithm>
#include <iostream>
#include <cmath>

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

    // --- Wave scaling policy (tweak these numbers for balance) ---
    // number of enemies
    currentConfig.waveNumber = waveNum;
    // start small, grow faster: formula -> base + wave * growth
    currentConfig.count = 5 + waveNum * 3; 

    // spawn interval: higher = slower spawns. clamp minimum so it's never too crazy.
    // Early waves have a comfortable interval, later waves ramp down.
    currentConfig.spawnInterval = std::max(0.35f, 1.2f - (waveNum * 0.035f));

    // speed: small linear bump already present
    currentConfig.enemySpeedMultiplier = 1.0f + (waveNum - 1) * 0.02f; // tiny increase per wave

    // HEALTH: exponential growth is common (so later waves scale faster)
    currentConfig.enemyHealthMultiplier = std::pow(1.12f, static_cast<float>(waveNum - 1)); 
    // - wave 1 => 1.0, wave 2 => ~1.12, wave 3 => ~1.25, wave 10 => ~3.10

    // DAMAGE: linear growth (keeps player summersault manageable)
    currentConfig.enemyDamageMultiplier = 1.0f + static_cast<float>(waveNum - 1) * 0.06f; 
    // - wave1 => 1.0, wave2 => 1.06, wave3 => 1.12, w10 => 1.54

    enemiesToSpawn = currentConfig.count;
    spawnedCount = 0;
    killedCount = 0;
    nextSpawnTimer = 0.f;
    active = true;
    interWaveTimer = 0.f;

    if (onWaveStart) onWaveStart(currentWave);
    if (onWaveProgress) onWaveProgress(std::max(0, enemiesToSpawn - killedCount), enemiesToSpawn);

    if (WAVEMANAGER_DEBUG) {
        std::cout << "[WaveManager] Started wave " << currentWave << ", count=" << enemiesToSpawn
                  << ", spawnInterval=" << currentConfig.spawnInterval
                  << ", speedMult=" << currentConfig.enemySpeedMultiplier << "\n";
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
                if (currentConfig.enemySpeedMultiplier != 1.0f) e->multiplySpeed(currentConfig.enemySpeedMultiplier);

                // apply the new multipliers
                if (currentConfig.enemyHealthMultiplier != 1.0f) e->multiplyHealth(currentConfig.enemyHealthMultiplier);
                if (currentConfig.enemyDamageMultiplier != 1.0f) e->multiplyDamage(currentConfig.enemyDamageMultiplier);

                // (You can set initial position / other setup here if needed)
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

// WaveManager helper: extend inter-wave delay
void WaveManager::extendInterWaveDelay(float extraSeconds) {
    if (extraSeconds <= 0.f) return;
    interWaveDelay += extraSeconds;
    std::cout << "[WaveManager] Extended inter-wave delay by " << extraSeconds
              << "s. Now: " << interWaveDelay << "s\n";
}

float WaveManager::getInterWaveDelay() const {
    return interWaveDelay;
}
