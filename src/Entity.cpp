#include "Entity.hpp"

// Default constructor / destructor
Entity::Entity() = default;
Entity::~Entity() = default;

// Default damage handler: no-op. Subclasses should override to apply HP reduction.
void Entity::takeDamage(int /*amount*/) {
    // intentionally empty; override in subclasses that have health
}

// Default isAlive: returns true. Subclasses should override to reflect HP status.
bool Entity::isAlive() const {
    return true;
}

// Position accessors - subclasses often rely on these
sf::Vector2f Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    // keep sprite in sync if used
    sprite.setPosition(position);
}

sf::FloatRect Entity::getBounds() const {
    return sprite.getGlobalBounds();
}
