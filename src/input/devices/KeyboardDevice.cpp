#include "KeyboardDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void KeyboardDevice::init() {
    auto& config = ConfigManager::getInstance();

    // Load key bindings from config
    keyBindings[config.getKeyBinding("move_up")] = Action::MoveUp;
    keyBindings[config.getKeyBinding("move_down")] = Action::MoveDown;
    keyBindings[config.getKeyBinding("move_left")] = Action::MoveLeft;
    keyBindings[config.getKeyBinding("move_right")] = Action::MoveRight;
    keyBindings[config.getKeyBinding("pause")] = Action::Pause;
    keyBindings[config.getKeyBinding("confirm")] = Action::Confirm;
    keyBindings[config.getKeyBinding("cancel")] = Action::Cancel;

    // Initialize action states
    for (const auto& [key, action] : keyBindings) {
        actionStates[action] = InputDevice::ActionState::NONE;
    }
}

InputDevice::ActionState KeyboardDevice::getActionState(Action action) const {
    for (const auto& [key, boundAction] : keyBindings) {
        if (boundAction == action) {
            auto stateIt = actionStates.find(action);
            return stateIt != actionStates.end() ? stateIt->second : InputDevice::ActionState::NONE;
        }
    }
    return InputDevice::ActionState::NONE;
}

void KeyboardDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        auto keyIt = keyBindings.find(event.key.code);
        if (keyIt != keyBindings.end()) {
            Action action = keyIt->second;
            if (event.type == sf::Event::KeyPressed) {
                actionStates[action] = (actionStates[action] == InputDevice::ActionState::NONE) ?
                    InputDevice::ActionState::JUST_PRESSED : InputDevice::ActionState::PRESSED;
                spdlog::debug("Key {} pressed for action {}",
                    static_cast<int>(event.key.code), static_cast<int>(action));
            } else {
                actionStates[action] = InputDevice::ActionState::RELEASED;
            }
        }
    }

    // Reset JUST_PRESSED and RELEASED states after they've been consumed
    for (auto& [action, state] : actionStates) {
        if (state == InputDevice::ActionState::JUST_PRESSED) {
            state = InputDevice::ActionState::PRESSED;
        } else if (state == InputDevice::ActionState::RELEASED) {
            state = InputDevice::ActionState::NONE;
        }
    }
}

void KeyboardDevice::setKeyBinding(Action action, sf::Keyboard::Key key) {
    keyBindings[key] = action;
}

void KeyboardDevice::setKeyState(sf::Keyboard::Key key, bool pressed) {
    auto keyIt = keyBindings.find(key);
    if (keyIt != keyBindings.end()) {
        Action action = keyIt->second;
        actionStates[action] = pressed ?
            (actionStates[action] == InputDevice::ActionState::NONE ?
             InputDevice::ActionState::JUST_PRESSED : InputDevice::ActionState::PRESSED) :
            InputDevice::ActionState::RELEASED;
    }
}

} // namespace game
