#include "KeyboardDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

KeyboardDevice::KeyboardDevice() {
    auto& config = ConfigManager::getInstance();

    // Set up default key bindings from config
    bindKey(config.getKeyBinding("move_up"), Action::MoveUp);
    bindKey(config.getKeyBinding("move_down"), Action::MoveDown);
    bindKey(config.getKeyBinding("move_left"), Action::MoveLeft);
    bindKey(config.getKeyBinding("move_right"), Action::MoveRight);
    bindKey(config.getKeyBinding("pause"), Action::Pause);
    bindKey(config.getKeyBinding("confirm"), Action::Confirm);
    bindKey(config.getKeyBinding("cancel"), Action::Cancel);

    // Initialize all key states to false
    for (const auto& binding : keyBindings) {
        keyStates[binding.first] = false;
    }
}

void KeyboardDevice::update() {
    // Nothing to update for keyboard
}

void KeyboardDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        auto it = keyBindings.find(event.key.code);
        if (it != keyBindings.end()) {
            keyStates[event.key.code] = (event.type == sf::Event::KeyPressed);

            if (event.type == sf::Event::KeyPressed) {
                notifyActionTriggered(it->second);
            }
        }
    }
}

bool KeyboardDevice::isActionPressed(Action action) {
    sf::Keyboard::Key key = findKeyForAction(action);
    return (key != sf::Keyboard::Unknown) && keyStates[key];
}

void KeyboardDevice::bindKey(sf::Keyboard::Key key, Action action) {
    keyBindings[key] = action;
    keyStates[key] = false;
}

void KeyboardDevice::setKeyState(sf::Keyboard::Key key, bool pressed) {
    keyStates[key] = pressed;
}

sf::Keyboard::Key KeyboardDevice::findKeyForAction(Action action) const {
    for (const auto& binding : keyBindings) {
        if (binding.second == action) {
            return binding.first;
        }
    }
    return sf::Keyboard::Unknown;
}

} // namespace game
