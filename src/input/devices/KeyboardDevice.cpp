#include "KeyboardDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

KeyboardDevice::KeyboardDevice() {
    loadConfig();
}

void KeyboardDevice::loadConfig() {
    spdlog::info("Loading keyboard configuration");
    auto& config = ConfigManager::getInstance();

    // Set up default key bindings
    keyBindings[config.getKeyBinding("move_up")] = Action::MoveUp;
    keyBindings[config.getKeyBinding("move_down")] = Action::MoveDown;
    keyBindings[config.getKeyBinding("move_left")] = Action::MoveLeft;
    keyBindings[config.getKeyBinding("move_right")] = Action::MoveRight;
    keyBindings[config.getKeyBinding("pause")] = Action::Pause;
    keyBindings[config.getKeyBinding("confirm")] = Action::Confirm;
    keyBindings[config.getKeyBinding("cancel")] = Action::Cancel;

    // Initialize all key states to false
    for (const auto& binding : keyBindings) {
        keyStates[binding.first] = false;
    }
}

void KeyboardDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        auto it = keyBindings.find(event.key.code);
        if (it != keyBindings.end()) {
            keyStates[event.key.code] = (event.type == sf::Event::KeyPressed);
            spdlog::debug("Keyboard key {} {}", static_cast<int>(event.key.code),
                         (event.type == sf::Event::KeyPressed) ? "pressed" : "released");
        }
    }
}

void KeyboardDevice::update() {
    // No continuous update needed for keyboard
}

bool KeyboardDevice::isActionPressed(Action action) const {
    for (const auto& [key, boundAction] : keyBindings) {
        if (boundAction == action && keyStates.at(key)) {
            return true;
        }
    }
    return false;
}

} // namespace game
