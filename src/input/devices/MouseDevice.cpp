#include "MouseDevice.hpp"
#include <spdlog/spdlog.h>
#include "../../config/ConfigManager.hpp"

namespace game {

MouseDevice::MouseDevice(sf::RenderWindow& window)
    : window(window)
    , leftButtonPressed(false)
    , rightButtonPressed(false)
    , leftButtonJustPressed(false)
    , rightButtonJustPressed(false)
    , lastMousePos(sf::Mouse::getPosition(window))
{}

void MouseDevice::init() {
    auto& config = ConfigManager::getInstance();

    setButtonBinding(config.getMouseButton("fire"), Action::Fire);
}

void MouseDevice::update() {
    for (const auto& binding : buttonBindings) {
        if (buttonStates[binding.first].current != buttonStates[binding.first].previous) {
            buttonStates[binding.first].previous = buttonStates[binding.first].current;
        }
    }
}

bool MouseDevice::isActionPressed(Action action) {
    for (const auto& binding : buttonBindings) {
        if (binding.second == action && buttonStates[binding.first].current) {
            return true;
        }
    }
    return false;
}

bool MouseDevice::isActionJustPressed(Action action) {
    for (const auto& binding : buttonBindings) {
        const auto& state = buttonStates[binding.first];
        if (binding.second == action && state.current != state.previous && state.current) {
            return true;
        }
    }
    return false;
}

bool MouseDevice::isActionReleased(Action action) {
    for (const auto& binding : buttonBindings) {
        const auto& state = buttonStates[binding.first];
        if (binding.second == action && state.current != state.previous && !state.current) {
            return true;
        }
    }
    return false;
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
    buttonBindings[button] = action;
}

void MouseDevice::setButtonState(sf::Event::MouseButtonEvent buttonEvent, bool pressed) {
    std::string action = pressed ? "Pressed" : "Released";
    spdlog::debug("Mouse Button {} {} at position ({}, {})", MouseMapper::getInstance().buttonToString(buttonEvent.button), action, buttonEvent.x, buttonEvent.y);

    auto& state = buttonStates[buttonEvent.button];
    state.previous = state.current;
    state.current = pressed;
}

} // namespace game