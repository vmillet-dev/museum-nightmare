#include "KeyboardDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void KeyboardDevice::init() {
    auto& config = ConfigManager::getInstance();

    // Load key bindings from config
    setKeyBinding(config.getKeyBinding("move_up"), Action::MoveUp);
    setKeyBinding(config.getKeyBinding("move_down"), Action::MoveDown);
    setKeyBinding(config.getKeyBinding("move_left"), Action::MoveLeft);
    setKeyBinding(config.getKeyBinding("move_right"), Action::MoveRight);
    setKeyBinding(config.getKeyBinding("pause"), Action::Pause);
    setKeyBinding(config.getKeyBinding("confirm"), Action::Confirm);
    setKeyBinding(config.getKeyBinding("cancel"), Action::Cancel);
}

void KeyboardDevice::update() {
    for (const auto& binding : keyBindings) {
        if (keyStates[binding.first].current != keyStates[binding.first].previous) {
            keyStates[binding.first].previous = keyStates[binding.first].current;
        }
    }
}

bool KeyboardDevice::isActionPressed(Action action) {
    for (const auto& binding : keyBindings) {
        if (binding.second == action && keyStates[binding.first].current) {
            return true;
        }
    }
    return false;
}

bool KeyboardDevice::isActionJustPressed(Action action) {
    for (const auto& binding : keyBindings) {
        const auto& state = keyStates[binding.first];
        if (binding.second == action && state.current != state.previous && state.current) {
            return true;
        }
    }
    return false;
}

bool KeyboardDevice::isActionReleased(Action action) {
    for (const auto& binding : keyBindings) {
        const auto& state = keyStates[binding.first];
        if (binding.second == action && state.current != state.previous && !state.current) {
            return true;
        }
    }
    return false;
}

void KeyboardDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        setKeyState(event.key.code, event.type == sf::Event::KeyPressed);
    }
}

void KeyboardDevice::setKeyBinding(sf::Keyboard::Key key, Action action) {
    keyBindings[key] = action;
}

void KeyboardDevice::setKeyState(sf::Keyboard::Key key, bool pressed) {
    if (keyBindings.count(key) != 0) {
        auto& state = keyStates[key];
        state.previous = state.current;
        state.current = pressed;
    }
}

} // namespace game
