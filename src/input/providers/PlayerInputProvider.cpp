#include "PlayerInputProvider.hpp"
#include <spdlog/spdlog.h>

namespace game {

PlayerInputProvider::PlayerInputProvider(InputManager& inputMgr) : inputManager(inputMgr) {}

sf::Vector2f PlayerInputProvider::getMovementVector() {
    sf::Vector2f movement(0.0f, 0.0f);

    auto upState = inputManager.getActionState(Action::MoveUp);
    auto downState = inputManager.getActionState(Action::MoveDown);
    auto leftState = inputManager.getActionState(Action::MoveLeft);
    auto rightState = inputManager.getActionState(Action::MoveRight);

    if (upState == InputDevice::ActionState::PRESSED || upState == InputDevice::ActionState::JUST_PRESSED) {
        movement.y = -1.0f;
    }
    if (downState == InputDevice::ActionState::PRESSED || downState == InputDevice::ActionState::JUST_PRESSED) {
        movement.y = 1.0f;
    }
    if (leftState == InputDevice::ActionState::PRESSED || leftState == InputDevice::ActionState::JUST_PRESSED) {
        movement.x = -1.0f;
    }
    if (rightState == InputDevice::ActionState::PRESSED || rightState == InputDevice::ActionState::JUST_PRESSED) {
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
    auto state = inputManager.getActionState(action);
    return state == InputDevice::ActionState::PRESSED || state == InputDevice::ActionState::JUST_PRESSED;
}

} // namespace game
