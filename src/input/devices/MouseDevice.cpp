#include "MouseDevice.hpp"
#include <spdlog/spdlog.h>
#include "../../config/ConfigManager.hpp"
#include "../mappers/MouseMapper.hpp"

namespace game {

MouseDevice::MouseDevice(sf::RenderWindow& window) : window(window), lastMousePos(sf::Mouse::getPosition(window)){
    loadBinding();
}

void MouseDevice::loadBinding() {
    spdlog::debug("loading MouseDevice binding");
    auto& config = ConfigManager::getInstance();
    auto mapper = MouseMapper();
    clearBindingsAndStatesMap();

    // Load mouse bindings from config
    for (const auto& [actionStr, action] : ActionUtil::getActionMap()) {
        auto* buttons = config.getMouseBindingsFromAction(actionStr);
        if (!buttons || buttons->empty()) {
            continue;
        }

        for (const auto& button : *buttons) {
            if (!button.is_string()) {
                spdlog::warn("Invalid mouse button binding type for action: {} must be a string type", actionStr);
                continue;
            }

            std::string buttonName = button.value_or("");
            if (buttonName.empty()) {
                continue;
            }

            auto sfButtonOpt = mapper.stringToButton(buttonName);
            if (!sfButtonOpt.has_value()) {
                spdlog::warn("Unknown mouse button name for action {}: {}", actionStr, buttonName);
                continue;
            }

            setBinding(sfButtonOpt.value(), action);
            spdlog::debug("Set mouse binding: {} -> {}", buttonName, ActionUtil::toString(action));
        }
    }
    spdlog::debug("MouseDevice binding loaded");
}

void MouseDevice::handleEvent(const sf::Event& event) {
    if (const auto* buttonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        setButtonState(buttonPressed->button, true);
    } else if (const auto* buttonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        setButtonState(buttonReleased->button, false);
    }
}

sf::Vector2i MouseDevice::getMousePosition() const {
    return lastMousePos;
}

void MouseDevice::setButtonState(const sf::Mouse::Button& button, bool pressed) {
    auto mousePos = sf::Mouse::getPosition(window);
    spdlog::debug("Mouse Button {} at position ({}, {})", pressed ? "Pressed" : "Released", mousePos.x, mousePos.y);
    setDigitalState(button, pressed);
}

} // namespace game
