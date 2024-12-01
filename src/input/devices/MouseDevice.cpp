#include "MouseDevice.hpp"
#include <spdlog/spdlog.h>
#include "../../config/ConfigManager.hpp"

namespace game {

MouseDevice::MouseDevice(sf::RenderWindow& window)
    : window(window)
    , lastMousePos(sf::Mouse::getPosition(window))
{}

void MouseDevice::init() {
    // Load mouse bindings from config for each action
    const std::vector<std::string> actions = {"MoveUp", "MoveDown", "MoveLeft", "MoveRight", "Pause", "Confirm", "Cancel", "Fire"};

    for (const auto& actionStr : actions) {
        Action action = config.getActionFromString(actionStr);
        const auto& buttons = config.getMouseBindingsForAction(actionStr);
        for (const auto& button : buttons) {
            setButtonBinding(static_cast<sf::Mouse::Button>(button), action);
            spdlog::debug("Set mouse binding: {} -> {}", static_cast<int>(button), static_cast<int>(action));
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
    // Initialize state when binding is set
    if (states.find(button) == states.end()) {
        states[button] = ActionState{false, false};
    }
}

void MouseDevice::setButtonState(sf::Event::MouseButtonEvent buttonEvent, bool pressed) {
    std::string action = pressed ? "Pressed" : "Released";
    spdlog::debug("Mouse Button {} {} at position ({}, {})", MouseMapper::getInstance().buttonToString(buttonEvent.button), action, buttonEvent.x, buttonEvent.y);
    setState(buttonEvent.button, pressed);
}

} // namespace game
