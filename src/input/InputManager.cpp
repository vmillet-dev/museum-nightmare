#include "InputManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void InputManager::init() {
    spdlog::info("Initializing InputManager");

    // Set up default key bindings
    keyBindings[sf::Keyboard::Z] = Action::MoveUp;
    keyBindings[sf::Keyboard::S] = Action::MoveDown;
    keyBindings[sf::Keyboard::Q] = Action::MoveLeft;
    keyBindings[sf::Keyboard::D] = Action::MoveRight;
    keyBindings[sf::Keyboard::Escape] = Action::Pause;
    keyBindings[sf::Keyboard::Return] = Action::Confirm;
    keyBindings[sf::Keyboard::BackSpace] = Action::Cancel;

    // Initialize all key states to false
    for (const auto& binding : keyBindings) {
        keyStates[binding.first] = false;
    }
}

void InputManager::bindAction(Action action, std::function<void()> callback) {
    actionCallbacks[action] = callback;
    spdlog::debug("Bound callback to action {}", static_cast<int>(action));
}

void InputManager::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        auto it = keyBindings.find(event.key.code);
        if (it != keyBindings.end()) {
            // Update key state
            keyStates[event.key.code] = (event.type == sf::Event::KeyPressed);
            spdlog::debug("Key {} {}", static_cast<int>(event.key.code),
                         (event.type == sf::Event::KeyPressed) ? "pressed" : "released");

            if (event.type == sf::Event::KeyPressed) {
                auto callbackIt = actionCallbacks.find(it->second);
                if (callbackIt != actionCallbacks.end()) {
                    callbackIt->second();
                }
            }
        }
    }
}

void InputManager::setKeyState(sf::Keyboard::Key key, bool pressed) {
    keyStates[key] = pressed;
    spdlog::debug("Key state manually set - Key: {}, Pressed: {}", static_cast<int>(key), pressed);
}

bool InputManager::isActionPressed(Action action) {
    bool isPressed = false;
    bool keyboardPressed = false;
    bool joystickPressed = false;

    // Helper lambda to find key for action
    auto findKeyForAction = [this](Action action) -> sf::Keyboard::Key {
        for (const auto& binding : keyBindings) {
            if (binding.second == action) {
                return binding.first;
            }
        }
        return sf::Keyboard::Unknown;
    };

    switch (action) {
        case Action::MoveUp: {
            sf::Keyboard::Key key = findKeyForAction(Action::MoveUp);
            keyboardPressed = keyStates[key];
            joystickPressed = hasController && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) < -20.0f;
            break;
        }
        case Action::MoveDown: {
            sf::Keyboard::Key key = findKeyForAction(Action::MoveDown);
            keyboardPressed = keyStates[key];
            joystickPressed = hasController && sf::Joystick::getAxisPosition(0, sf::Joystick::Y) > 20.0f;
            break;
        }
        case Action::MoveLeft: {
            sf::Keyboard::Key key = findKeyForAction(Action::MoveLeft);
            keyboardPressed = keyStates[key];
            joystickPressed = hasController && sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -20.0f;
            break;
        }
        case Action::MoveRight: {
            sf::Keyboard::Key key = findKeyForAction(Action::MoveRight);
            keyboardPressed = keyStates[key];
            joystickPressed = hasController && sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 20.0f;
            break;
        }
        default:
            break;
    }

    isPressed = keyboardPressed || joystickPressed;
    if (isPressed) {
        spdlog::debug("InputManager: Action {} detected - Keyboard: {}, Joystick: {}",
                     static_cast<int>(action), keyboardPressed, joystickPressed);
    }
    return isPressed;
}

void InputManager::update() {
    // Check controller connection status
    bool wasControllerConnected = hasController;
    hasController = sf::Joystick::isConnected(0);

    if (wasControllerConnected != hasController) {
        if (hasController) {
            spdlog::info("Controller connected");
        } else {
            spdlog::info("Controller disconnected");
        }
    }
}

} // namespace game
