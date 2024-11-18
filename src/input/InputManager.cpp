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
}

void InputManager::bindAction(Action action, std::function<void()> callback) {
    actionCallbacks[action] = callback;
    spdlog::debug("Bound callback to action {}", static_cast<int>(action));
}

void InputManager::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        auto it = keyBindings.find(event.key.code);
        if (it != keyBindings.end()) {
            auto callbackIt = actionCallbacks.find(it->second);
            if (callbackIt != actionCallbacks.end()) {
                callbackIt->second();
            }
        }
    }
}

bool InputManager::isActionPressed(Action action) {
    switch (action) {
        case Action::MoveUp:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
        case Action::MoveDown:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        case Action::MoveLeft:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
        case Action::MoveRight:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        default:
            return false;
    }
}

void InputManager::update() {
    // Update any continuous input states here if needed
    spdlog::trace("InputManager updated");
}

} // namespace game
