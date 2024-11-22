#include "Actor.hpp"
#include <spdlog/spdlog.h>

namespace game {

Actor::Actor(float x, float y, std::unique_ptr<InputProvider> provider, float speed)
    : GameObject(x, y), inputProvider(std::move(provider)), speed(speed) {}

void Actor::update(float deltaTime) {
    if (inputProvider) {
        sf::Vector2f movement = inputProvider->getMovementVector();
        if (movement.x != 0.0f || movement.y != 0.0f) {
            move(movement.x, movement.y, deltaTime);
        }
    }
}

void Actor::move(float x, float y, float deltaTime) {
    sf::Vector2f oldPos = getPosition();
    sf::Vector2f newPos = oldPos + sf::Vector2f(x * speed * deltaTime, y * speed * deltaTime);
    setPosition(newPos);
}

} // namespace game
