#include "MouseDevice.hpp"
#include <spdlog/spdlog.h>
#include "../../config/ConfigManager.hpp"
#include "../mappers/MouseMapper.hpp"

namespace game {

MouseDevice::MouseDevice(sf::RenderWindow& window)
    : window(window)
    , lastMousePos(sf::Mouse::getPosition(window))
{}

void MouseDevice::init() {
    spdlog::debug("Initializing MouseDevice");
    auto& config = ConfigManager::getInstance();
    auto& mapper = MouseMapper::getInstance();

    // Load mouse bindings from config
    for (const auto& [actionStr, action] : ActionUtil::getActionMap()) {
        auto* buttons = config.getMouseBindingsForAction(actionStr);
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

            auto sfButton = mapper.stringToButton(buttonName);
            if (sfButton == sf::Mouse::Button::ButtonCount) {
                spdlog::warn("Unknown mouse button name for action {}: {}", actionStr, buttonName);
                continue;
            }

            setButtonBinding(sfButton, action);
            spdlog::debug("Set mouse binding: {} -> {}", buttonName, ActionUtil::toString(action));
        }
    }
    spdlog::debug("MouseDevice initialized");
}

void MouseDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
        setButtonState(event.mouseButton, event.type == sf::Event::MouseButtonPressed);
    }
}

sf::Vector2i MouseDevice::getMousePosition() const {
    return lastMousePos;
}

void MouseDevice::setButtonBinding(sf::Mouse::Button button, Action action) {
    setBinding(button, action);
}

void MouseDevice::setButtonState(sf::Event::MouseButtonEvent buttonEvent, bool pressed) {
    auto& mapper = MouseMapper::getInstance();
    spdlog::debug("Mouse Button {} {} at position ({}, {})",
                 mapper.buttonToString(buttonEvent.button),
                 pressed ? "Pressed" : "Released",
                 buttonEvent.x,
                 buttonEvent.y);
    setState(buttonEvent.button, pressed);
}

} // namespace game
