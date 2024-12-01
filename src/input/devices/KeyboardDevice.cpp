#include "KeyboardDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void KeyboardDevice::init() {
    // Load key bindings from config for each action
    const std::vector<std::string> actions = {"MoveUp", "MoveDown", "MoveLeft", "MoveRight", "Pause", "Confirm", "Cancel", "Fire"};

    for (const auto& actionStr : actions) {
        Action action = ConfigManager::getInstance().getActionFromString(actionStr);
        std::vector<sf::Keyboard::Key> keys = ConfigManager::getInstance().getKeyboardBindingsForAction(actionStr);  // Explicit type
        for (const sf::Keyboard::Key& key : keys) {  // Explicit type
            setKeyBinding(key, action);  // No need for static_cast since type is already correct
            spdlog::debug("Set keyboard binding: {} -> {}", static_cast<int>(key), static_cast<int>(action));
        }
    }
}

void KeyboardDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        setKeyState(event.key.code, event.type == sf::Event::KeyPressed);
    }
}

void KeyboardDevice::setKeyBinding(sf::Keyboard::Key key, Action action) {
    setBinding(key, action);
}

void KeyboardDevice::setKeyState(sf::Keyboard::Key key, bool pressed) {
    setState(key, pressed);
}

} // namespace game
