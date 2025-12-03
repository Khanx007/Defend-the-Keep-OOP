#include "Entity.hpp"

// default constructor / destructor
Entity::Entity() = default;
Entity::~Entity() = default;

// default do-nothing takeDamage (override in towers/enemies)
void Entity::takeDamage(int amount) {
    (void)amount; // unused by default
}

// default isAlive: check that sprite / entity is present.
// Derived classes should override if they track hp.
bool Entity::isAlive() const {
    return true;
}

// position accessors
sf::Vector2f Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const sf::Vector2f& pos) {
    position = pos;
    sprite.setPosition(position);
}

// Default bounds uses sprite global bounds
sf::FloatRect Entity::getBounds() const {
    return sprite.getGlobalBounds();
}
