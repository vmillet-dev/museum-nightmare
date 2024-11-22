#include "ControllerDevice.hpp"
#include <spdlog/spdlog.h>

namespace game {

void ControllerDevice::init() {
    // Check for connected controllers
    connected = sf::Joystick::isConnected(0);
    spdlog::debug("Controller connected: {}", connected);

    // Default button bindings
    buttonBindings[Action::Confirm] = 0;  // A button
    buttonBindings[Action::Cancel] = 1;   // B button
    buttonBindings[Action::Pause] = 7;    // Start button

    // Default axis bindings
    axisBindings[Action::MoveUp] = sf::Joystick::Y;
    axisBindings[Action::MoveDown] = sf::Joystick::Y;
    axisBindings[Action::MoveLeft] = sf::Joystick::X;
    axisBindings[Action::MoveRight] = sf::Joystick::X;
}

void ControllerDevice::update() {
    // Check controller connection status
    bool wasConnected = connected;
    connected = sf::Joystick::isConnected(0);

    if (connected != wasConnected) {
        spdlog::info("Controller {} - Status changed", connected ? "connected" : "disconnected");
        if (connected) {
            init();  // Reinitialize on reconnection
        }
    }
}

bool ControllerDevice::isActionPressed(Action action) {
    if (!connected) return false;

    // Check button bindings
    auto buttonIt = buttonBindings.find(action);
    if (buttonIt != buttonBindings.end()) {
        return sf::Joystick::isButtonPressed(0, buttonIt->second);
    }

    // Check axis bindings
    auto axisIt = axisBindings.find(action);
    if (axisIt != axisBindings.end()) {
        float position = sf::Joystick::getAxisPosition(0, axisIt->second);

        // Apply deadzone and sensitivity
        if (std::abs(position) < deadzone) return false;

        position = (position / sensitivity);

        switch(action) {
            case Action::MoveUp:
                return position < -0.5f;
            case Action::MoveDown:
                return position > 0.5f;
            case Action::MoveLeft:
                return position < -0.5f;
            case Action::MoveRight:
                return position > 0.5f;
            default:
                return false;
        }
    }

    return false;
}

void ControllerDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::JoystickConnected) {
        connected = true;
        init();
    } else if (event.type == sf::Event::JoystickDisconnected) {
        connected = false;
    }
}

void ControllerDevice::setButtonBinding(Action action, unsigned int button) {
    buttonBindings[action] = button;
}

void ControllerDevice::setAxisBinding(Action action, sf::Joystick::Axis axis) {
    axisBindings[action] = axis;
}

} // namespace game
