#include "MouseDevice.hpp"
#include <spdlog/spdlog.h>

namespace game {

MouseDevice::MouseDevice(sf::RenderWindow& window)
    : window(window)
    , lastMousePos(sf::Mouse::getPosition(window))
{
    actionStates[Action::MouseLeft] = InputDevice::ActionState::NONE;
    actionStates[Action::MouseRight] = InputDevice::ActionState::NONE;
}

InputDevice::ActionState MouseDevice::getActionState(Action action) const {
    auto stateIt = actionStates.find(action);
    return stateIt != actionStates.end() ? stateIt->second : InputDevice::ActionState::NONE;
}

void MouseDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed ||
        event.type == sf::Event::MouseButtonReleased) {
        Action action;
        if (event.mouseButton.button == sf::Mouse::Left) {
            action = Action::MouseLeft;
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            action = Action::MouseRight;
        } else {
            return;
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            actionStates[action] = (actionStates[action] == InputDevice::ActionState::NONE) ?
                InputDevice::ActionState::JUST_PRESSED : InputDevice::ActionState::PRESSED;
            spdlog::debug("Mouse button {} pressed at position ({}, {})",
                static_cast<int>(event.mouseButton.button),
                event.mouseButton.x, event.mouseButton.y);
        } else {
            actionStates[action] = InputDevice::ActionState::RELEASED;
        }
    }

    // Update mouse position
    lastMousePos = sf::Mouse::getPosition(window);

    // Reset JUST_PRESSED and RELEASED states after they've been consumed
    for (auto& [action, state] : actionStates) {
        if (state == InputDevice::ActionState::JUST_PRESSED) {
            state = InputDevice::ActionState::PRESSED;
        } else if (state == InputDevice::ActionState::RELEASED) {
            state = InputDevice::ActionState::NONE;
        }
    }
}

sf::Vector2i MouseDevice::getMousePosition() const {
    return lastMousePos;
}

bool MouseDevice::isLeftButtonPressed() const {
    return getActionState(Action::MouseLeft) == InputDevice::ActionState::PRESSED ||
           getActionState(Action::MouseLeft) == InputDevice::ActionState::JUST_PRESSED;
}

bool MouseDevice::isRightButtonPressed() const {
    return getActionState(Action::MouseRight) == InputDevice::ActionState::PRESSED ||
           getActionState(Action::MouseRight) == InputDevice::ActionState::JUST_PRESSED;
}

} // namespace game
