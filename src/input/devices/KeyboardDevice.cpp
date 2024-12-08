#include "KeyboardDevice.hpp"
#include <spdlog/spdlog.h>

#include "../../config/ConfigManager.hpp"
#include "../mappers/KeyMapper.hpp"

namespace game {

KeyboardDevice::KeyboardDevice() {
    spdlog::debug("Creating KeyboardDevice");
}

void KeyboardDevice::init() {
    spdlog::debug("Initializing KeyboardDevice");
    auto& config = ConfigManager::getInstance();
    auto& mapper = KeyMapper::getInstance();
    spdlog::debug("Got KeyMapper instance");

    // Load key bindings from config
    for (const auto& [actionStr, action] : ActionUtil::getActionMap()) {
        spdlog::debug("Loading bindings for action: {}", actionStr);
        auto* keys = config.getKeyboardBindingsForAction(actionStr);
        if (!keys) {
            spdlog::warn("No keyboard bindings found for action: {}", actionStr);
            continue;
        }

        for (const auto& key : *keys) {
            if (!key) {
                spdlog::warn("Invalid key binding for action: {}", actionStr);
                continue;
            }

            std::string keyName = key.value_or("");
            if (keyName.empty()) {
                spdlog::warn("Empty key binding for action: {}", actionStr);
                continue;
            }

            sf::Keyboard::Key sfKey = mapper.fromName(keyName);
            if (sfKey == sf::Keyboard::Unknown) {
                spdlog::warn("Unknown key name for action {}: {}", actionStr, keyName);
                continue;
            }

            setKeyBinding(sfKey, action);
            spdlog::debug("Set keyboard binding: {} -> {}", keyName, ActionUtil::toString(action));
        }
    }
    spdlog::debug("KeyboardDevice initialized");
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
