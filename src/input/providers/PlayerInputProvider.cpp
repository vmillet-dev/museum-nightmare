#include "PlayerInputProvider.hpp"
#include <spdlog/spdlog.h>

namespace game {

PlayerInputProvider::PlayerInputProvider(InputManager& inputMgr) : inputManager(inputMgr) {}

sf::Vector2f PlayerInputProvider::getMovementVector() {
    sf::Vector2f movement(0.0f, 0.0f);

    if (inputManager.isActionPressed(Action::MoveUp)) {
        movement.y = -1.0f;
    }
    if (inputManager.isActionPressed(Action::MoveDown)) {
        movement.y = 1.0f;
    }
    if (inputManager.isActionPressed(Action::MoveLeft)) {
        movement.x = -1.0f;
    }
    if (inputManager.isActionPressed(Action::MoveRight)) {
        movement.x = 1.0f;
    }

    // Normalize movement vector if moving diagonally
    if (movement.x != 0.0f && movement.y != 0.0f) {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement.x /= length;
        movement.y /= length;
    }

    return movement;
}

bool PlayerInputProvider::isActionActive(Action action) {
    return inputManager.isActionPressed(action);
}

} // namespace game
