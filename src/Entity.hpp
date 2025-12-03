#pragma once
#include <SFML/Graphics.hpp>

/*
 * Lightweight base Entity class
 *
 * Notes:
 * - Concrete entities (Enemy, Tower, Castle) should override takeDamage() and isAlive()
 *   if they maintain their own HP/health logic.
 * - This header supplies sprite/position helpers used across your codebase.
 */

class Entity {
public:
    Entity();
    virtual ~Entity();

    // Per-frame logic (optional override)
    virtual void update(float dt) {}

    // Pure-render interface: must be implemented by concrete subclasses
    virtual void render(sf::RenderWindow& window) = 0;

    // Damage API: default is no-op. Override in classes that have HP.
    virtual void takeDamage(int amount);

    // Query whether this entity is alive for removal / targeting.
    // Default returns true (override to use real HP).
    virtual bool isAlive() const;

    // Position / sprite helpers used by other modules
    virtual sf::Vector2f getPosition() const;
    virtual void setPosition(const sf::Vector2f& pos);
    virtual sf::FloatRect getBounds() const;

    // Direct access to the sprite when subclass wants to use it
    sf::Sprite& getSprite() { return sprite; }
    const sf::Sprite& getSprite() const { return sprite; }

    

protected:
    sf::Sprite sprite;
    sf::Vector2f position{0.f, 0.f};
};
