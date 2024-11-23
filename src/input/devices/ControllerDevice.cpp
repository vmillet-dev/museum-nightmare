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
    buttonBindings[config.getControllerButton("controller_confirm")] = Action::Confirm;
    buttonBindings[config.getControllerButton("controller_cancel")] = Action::Cancel;
    buttonBindings[config.getControllerButton("controller_pause")] = Action::Pause;

    // Load axis bindings from config
    axisBindings[sf::Joystick::Y] = Action::MoveUp;
    axisBindings[sf::Joystick::X] = Action::MoveLeft;

    // Initialize states
    for (const auto& [button, action] : buttonBindings) {
        buttonStates[button] = false;
    }
    for (const auto& [axis, action] : axisBindings) {
        axisStates[axis] = 0.0f;
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

    // Update button states
    for (const auto& [button, action] : buttonBindings) {
        buttonStates[button] = sf::Joystick::isButtonPressed(controllerId, button);
    }

    // Update axis states
    for (const auto& [axis, action] : axisBindings) {
        float position = sf::Joystick::getAxisPosition(controllerId, axis);
        axisStates[axis] = std::abs(position) < deadzone ? 0.0f : position / sensitivity;
    }
}

bool ControllerDevice::isActionPressed(Action action) {
    if (!connected) return false;

    // Check button states
    for (const auto& [button, boundAction] : buttonBindings) {
        if (boundAction == action && buttonStates[button]) {
            return true;
        }
    }

    // Check axis states
    for (const auto& [axis, boundAction] : axisBindings) {
        if (boundAction == action) {
            float position = axisStates[axis];
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
                    break;
            }
        }
    }

    return false;
}

void ControllerDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::JoystickDisconnected &&
        event.joystickConnect.joystickId == controllerId) {
        connected = false;
        spdlog::info("Controller {} disconnected", controllerId);
    }
}

void ControllerDevice::setButtonBinding(Action action, unsigned int button) {
    buttonBindings[button] = action;
}

void ControllerDevice::setAxisBinding(Action action, sf::Joystick::Axis axis) {
    axisBindings[axis] = action;
}

} // namespace game
