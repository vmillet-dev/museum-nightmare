#include "ControllerDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void ControllerDevice::init() {
    auto& config = ConfigManager::getInstance();

    // Check for any available controller
    connected = false;
    for (int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            connected = true;
            controllerId = i;
            spdlog::info("Controller connected on port {}", i);
            break;
        }
    }

    // Load button bindings from config
    buttonBindings[Action::Confirm] = config.getControllerButton("controller_confirm");
    buttonBindings[Action::Cancel] = config.getControllerButton("controller_cancel");
    buttonBindings[Action::Pause] = config.getControllerButton("controller_pause");

    // Load axis bindings from config
    axisBindings[Action::MoveUp] = sf::Joystick::Y;
    axisBindings[Action::MoveDown] = sf::Joystick::Y;
    axisBindings[Action::MoveLeft] = sf::Joystick::X;
    axisBindings[Action::MoveRight] = sf::Joystick::X;

    // Initialize states
    for (const auto& [action, button] : buttonBindings) {
        buttonStates[action] = false;
    }
    for (const auto& [action, axis] : axisBindings) {
        axisStates[action] = 0.0f;
    }

    // Load controller settings
    setDeadzone(config.getControllerDeadzone());
    setSensitivity(config.getControllerSensitivity());
}

void ControllerDevice::update() {
    if (!connected) {
        // Check for new controller connection
        for (int i = 0; i < sf::Joystick::Count; ++i) {
            if (sf::Joystick::isConnected(i)) {
                connected = true;
                controllerId = i;
                spdlog::info("Controller connected on port {}", i);
                init();
                break;
            }
        }
        return;
    }

    // Store previous button states
    previousButtonStates = buttonStates;

    // Update button states
    for (const auto& [action, button] : buttonBindings) {
        buttonStates[action] = sf::Joystick::isButtonPressed(controllerId, button);
    }

    // Update axis states
    for (const auto& [action, axis] : axisBindings) {
        float position = sf::Joystick::getAxisPosition(controllerId, axis);
        axisStates[action] = std::abs(position) < deadzone ? 0.0f : position / sensitivity;
    }
}

bool ControllerDevice::isActionPressed(Action action) {
    if (!connected) return false;

    // Check button states
    auto buttonIt = buttonBindings.find(action);
    if (buttonIt != buttonBindings.end()) {
        return buttonStates[action];
    }

    // Check axis states
    auto axisIt = axisBindings.find(action);
    if (axisIt != axisBindings.end()) {
        float position = axisStates[action];
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

bool ControllerDevice::isActionJustPressed(Action action) {
    if (!connected) return false;

    auto buttonIt = buttonBindings.find(action);
    if (buttonIt != buttonBindings.end()) {
        return buttonStates[action] && !previousButtonStates[action];
    }

    // For axis-based actions, we don't track "just pressed" state
    return false;
}

bool ControllerDevice::isActionReleased(Action action) {
    if (!connected) return false;

    auto buttonIt = buttonBindings.find(action);
    if (buttonIt != buttonBindings.end()) {
        return !buttonStates[action];
    }

    auto axisIt = axisBindings.find(action);
    if (axisIt != axisBindings.end()) {
        float position = axisStates[action];
        switch(action) {
            case Action::MoveUp:
            case Action::MoveDown:
                return std::abs(position) <= 0.5f;
            case Action::MoveLeft:
            case Action::MoveRight:
                return std::abs(position) <= 0.5f;
            default:
                return true;
        }
    }

    return true;
}

void ControllerDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::JoystickDisconnected &&
        event.joystickConnect.joystickId == controllerId) {
        connected = false;
        spdlog::info("Controller {} disconnected", controllerId);
    }
}

void ControllerDevice::setButtonBinding(Action action, unsigned int button) {
    buttonBindings[action] = button;
}

void ControllerDevice::setAxisBinding(Action action, sf::Joystick::Axis axis) {
    axisBindings[action] = axis;
}

} // namespace game
