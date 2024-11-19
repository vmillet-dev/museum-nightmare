#include "ControllerDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

ControllerDevice::ControllerDevice(unsigned int id) : controllerId(id) {
    loadConfig();
}

void ControllerDevice::loadConfig() {
    auto& config = ConfigManager::getInstance();

    // Load controller settings
    deadzone = config.getControllerDeadzone();
    sensitivity = config.getControllerSensitivity();

    // Set up button bindings
    buttonBindings[Action::MoveUp] = config.getControllerButton("controller_move_up");
    buttonBindings[Action::MoveDown] = config.getControllerButton("controller_move_down");
    buttonBindings[Action::MoveLeft] = config.getControllerButton("controller_move_left");
    buttonBindings[Action::MoveRight] = config.getControllerButton("controller_move_right");
    buttonBindings[Action::Pause] = config.getControllerButton("controller_pause");
    buttonBindings[Action::Confirm] = config.getControllerButton("controller_confirm");
    buttonBindings[Action::Cancel] = config.getControllerButton("controller_cancel");

    // Set up axis bindings
    axisBindings[Action::MoveUp] = sf::Joystick::Y;
    axisBindings[Action::MoveDown] = sf::Joystick::Y;
    axisBindings[Action::MoveLeft] = sf::Joystick::X;
    axisBindings[Action::MoveRight] = sf::Joystick::X;

    spdlog::info("Controller {} configured: deadzone={}, sensitivity={}",
                 controllerId, deadzone, sensitivity);
}

bool ControllerDevice::isConnected() const {
    return sf::Joystick::isConnected(controllerId);
}

std::string ControllerDevice::getDeviceId() const {
    return "controller" + std::to_string(controllerId);
}

void ControllerDevice::handleEvent(const sf::Event& event) {
    // Controller events are handled in update()
}

void ControllerDevice::update() {
    if (!isConnected()) {
        return;
    }

    // Controller state is polled directly in isActionPressed
    // No need for additional update logic
}

bool ControllerDevice::checkAxisInput(Action action) const {
    auto it = axisBindings.find(action);
    if (it == axisBindings.end()) {
        return false;
    }

    float axisValue = (sf::Joystick::getAxisPosition(controllerId, it->second) / 100.0f) * sensitivity;

    switch (action) {
        case Action::MoveUp:
            return axisValue < -deadzone;
        case Action::MoveDown:
            return axisValue > deadzone;
        case Action::MoveLeft:
            return axisValue < -deadzone;
        case Action::MoveRight:
            return axisValue > deadzone;
        default:
            return false;
    }
}

bool ControllerDevice::isActionPressed(Action action) const {
    if (!isConnected()) {
        return false;
    }

    // Check button binding
    auto buttonIt = buttonBindings.find(action);
    if (buttonIt != buttonBindings.end() &&
        sf::Joystick::isButtonPressed(controllerId, buttonIt->second)) {
        return true;
    }

    // Check axis binding
    return checkAxisInput(action);
}

} // namespace game
