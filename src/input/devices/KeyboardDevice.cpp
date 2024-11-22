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
}

void KeyboardDevice::update() {
    // Update key states for continuous input
    for (const auto& binding : keyBindings) {
        keyStates[binding.first] = sf::Keyboard::isKeyPressed(binding.first);
    }
}

bool KeyboardDevice::isActionPressed(Action action) {
    for (const auto& binding : keyBindings) {
        if (binding.second == action && keyStates[binding.first]) {
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

void KeyboardDevice::setKeyBinding(Action action, sf::Keyboard::Key key) {
    keyBindings[key] = action;
}

void KeyboardDevice::setKeyState(sf::Keyboard::Key key, bool pressed) {
    keyStates[key] = pressed;
}

} // namespace game
