#include "GameObject.hpp"

namespace game {

GameObject::GameObject(float x, float y) : position(x, y) {}

const sf::Vector2f& GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
}

} // namespace game
