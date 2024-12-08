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
    auto& config = ConfigManager::getInstance();

    // Load mouse bindings from config
    for (const auto& [actionStr, action] : ActionUtil::getActionMap()) {
        auto buttons = config.getMouseBindingsForAction(actionStr);

        for (const auto& button : *buttons) {
            std::string buttonName = button.value_or("");
            auto sfButton = MouseMapper::getInstance().stringToButton(buttonName);
            if (sfButton != sf::Mouse::Button::ButtonCount) {

                setButtonBinding(sfButton, action);
                spdlog::debug("Set mouse binding: {} -> {}", buttonName, ActionUtil::toString(action));
            }
        }
    }
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
    std::string action = pressed ? "Pressed" : "Released";
    auto& mapper = MouseMapper::getInstance();
    spdlog::debug("Mouse Button {} {} at position ({}, {})",
                 mapper.buttonToString(buttonEvent.button),
                 action,
                 buttonEvent.x,
                 buttonEvent.y);
    setState(buttonEvent.button, pressed);
}

} // namespace game
