#include "KeyboardDevice.hpp"
#include <spdlog/spdlog.h>

#include "../../config/ConfigManager.hpp"
#include "../mappers/KeyMapper.hpp"

namespace game {

KeyboardDevice::KeyboardDevice() {
    loadBinding();
}

void KeyboardDevice::loadBinding() {
    spdlog::debug("Loading KeyboardDevice binding");
    auto& config = ConfigManager::getInstance();
    auto mapper = KeyMapper();
    clearBindingsAndStatesMap();

    // Load key bindings from config
    for (const auto& [actionStr, action] : ActionUtil::getActionMap()) {
        auto* keys = config.getKeyboardBindingsFromAction(actionStr);
        if (!keys || keys->empty()) {
            continue;
        }

        for (const auto& key : *keys) {
            if (!key.is_string()) {
                spdlog::warn("Invalid key binding type for action: {} must be string type", actionStr);
                continue;
            }

            std::string keyName = key.value_or("");
            if (keyName.empty()) {
                continue;
            }

            sf::Keyboard::Key sfKey = mapper.stringToKey(keyName);
            if (sfKey == sf::Keyboard::Key::Unknown) {
                spdlog::warn("Unknown key name for action {}: {}", actionStr, keyName);
                continue;
            }

            setBinding(sfKey, action);
            spdlog::debug("Set keyboard binding: {} -> {}", keyName, ActionUtil::toString(action));
        }
    }
    spdlog::debug("KeyboardDevice binding loaded");
}

void KeyboardDevice::handleEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        setDigitalState(keyPressed->code, true);
    } else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {
        setDigitalState(keyReleased->code, false);
    }
}

} // namespace game
