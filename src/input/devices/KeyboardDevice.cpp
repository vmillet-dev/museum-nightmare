#include "KeyboardDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void KeyboardDevice::init() {
    auto& config = ConfigManager::getInstance();

    // Load key bindings from config for each action
    const std::vector<std::string> actions = {"MoveUp", "MoveDown", "MoveLeft", "MoveRight", "Pause", "Confirm", "Cancel", "Fire"};

    for (const auto& actionStr : actions) {
        Action action = config.getActionFromString(actionStr);
        auto keys = config.getKeyboardBindingsForAction(actionStr);
        for (const auto& key : keys) {
            setKeyBinding(key, action);
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
