#include "ControllerDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

ControllerDevice::ControllerDevice(unsigned int index)
    : controllerIndex(index)
    , deadzone(20.0f)
    , sensitivity(100.0f)
    , connected(false) {

    auto& config = ConfigManager::getInstance();

    // Load controller settings from config
    deadzone = config.getControllerDeadzone();
    sensitivity = config.getControllerSensitivity();

    // Set up button bindings
    bindButton(config.getControllerButton("controller_move_up"), Action::MoveUp);
    bindButton(config.getControllerButton("controller_move_down"), Action::MoveDown);
    bindButton(config.getControllerButton("controller_move_left"), Action::MoveLeft);
    bindButton(config.getControllerButton("controller_move_right"), Action::MoveRight);
    bindButton(config.getControllerButton("controller_pause"), Action::Pause);
    bindButton(config.getControllerButton("controller_confirm"), Action::Confirm);
    bindButton(config.getControllerButton("controller_cancel"), Action::Cancel);

    // Set up axis bindings
    bindAxis(sf::Joystick::X, Action::MoveRight, Action::MoveLeft);
    bindAxis(sf::Joystick::Y, Action::MoveDown, Action::MoveUp);

    checkConnection();
}

void ControllerDevice::update() {
    checkConnection();

    if (connected) {
        // Check axis values and trigger actions
        for (const auto& [axis, binding] : axisBindings) {
            float value = getAxisValue(axis);
            if (std::abs(value) > deadzone) {
                Action action = (value > 0) ? binding.positiveAction : binding.negativeAction;
                notifyActionTriggered(action);
            }
        }
    }
}

bool ControllerDevice::isActionPressed(Action action) {
    if (!connected) return false;

    // Check button bindings
    for (const auto& [button, boundAction] : buttonBindings) {
        if (boundAction == action && sf::Joystick::isButtonPressed(controllerIndex, button)) {
            return true;
        }
    }

    // Check axis bindings
    for (const auto& [axis, binding] : axisBindings) {
        float value = getAxisValue(axis);
        if (std::abs(value) > deadzone) {
            if (value > 0 && binding.positiveAction == action) return true;
            if (value < 0 && binding.negativeAction == action) return true;
        }
    }

    return false;
}

void ControllerDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::JoystickConnected &&
        event.joystickConnect.joystickId == controllerIndex) {
        checkConnection();
    }
    else if (event.type == sf::Event::JoystickDisconnected &&
             event.joystickConnect.joystickId == controllerIndex) {
        checkConnection();
    }
    else if (event.type == sf::Event::JoystickButtonPressed &&
             event.joystickButton.joystickId == controllerIndex) {
        auto it = buttonBindings.find(event.joystickButton.button);
        if (it != buttonBindings.end()) {
            notifyActionTriggered(it->second);
        }
    }
}

bool ControllerDevice::isConnected() const {
    return connected;
}

std::string ControllerDevice::getDeviceName() const {
    return "Controller " + std::to_string(controllerIndex);
}

void ControllerDevice::bindButton(unsigned int button, Action action) {
    buttonBindings[button] = action;
}

void ControllerDevice::bindAxis(sf::Joystick::Axis axis, Action positiveAction, Action negativeAction) {
    axisBindings[axis] = {positiveAction, negativeAction};
}

void ControllerDevice::checkConnection() {
    bool wasConnected = connected;
    connected = sf::Joystick::isConnected(controllerIndex);

    if (wasConnected != connected) {
        if (connected) {
            spdlog::info("Controller {} connected: {} buttons, X:{}, Y:{}",
                controllerIndex,
                sf::Joystick::getButtonCount(controllerIndex),
                sf::Joystick::hasAxis(controllerIndex, sf::Joystick::X),
                sf::Joystick::hasAxis(controllerIndex, sf::Joystick::Y));
        } else {
            spdlog::info("Controller {} disconnected", controllerIndex);
        }
    }
}

float ControllerDevice::getAxisValue(sf::Joystick::Axis axis) const {
    if (!sf::Joystick::hasAxis(controllerIndex, axis)) return 0.0f;
    return (sf::Joystick::getAxisPosition(controllerIndex, axis) / 100.0f) * sensitivity;
}

} // namespace game
