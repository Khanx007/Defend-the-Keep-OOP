#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity();
    virtual ~Entity();

    // Per-frame update / render
    virtual void update(float dt) {}
    virtual void render(sf::RenderWindow& window) = 0;

    // Basic interface used by towers/enemies/projectiles
    virtual void takeDamage(int amount);      // default: do nothing
    virtual bool isAlive() const;            // default: true unless overridden

    // Position helpers (non-virtual)
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    // Sprite / bounds helper
    virtual sf::FloatRect getBounds() const;



protected:
    sf::Sprite sprite;
    sf::Vector2f position{0.f, 0.f};
};
