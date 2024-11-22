#include "KeyboardDevice.hpp"
#include <spdlog/spdlog.h>

namespace game {

void KeyboardDevice::init() {
    // Default key bindings
    keyBindings[sf::Keyboard::Z] = Action::MoveUp;
    keyBindings[sf::Keyboard::S] = Action::MoveDown;
    keyBindings[sf::Keyboard::Q] = Action::MoveLeft;
    keyBindings[sf::Keyboard::D] = Action::MoveRight;
    keyBindings[sf::Keyboard::Escape] = Action::Pause;
    keyBindings[sf::Keyboard::Enter] = Action::Confirm;
    keyBindings[sf::Keyboard::BackSpace] = Action::Cancel;
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
