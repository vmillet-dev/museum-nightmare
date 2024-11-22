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

    // Load controller settings
    setDeadzone(config.getControllerDeadzone());
    setSensitivity(config.getControllerSensitivity());
}

void ControllerDevice::update() {
    wasConnected = connected;

    // Check if current controller is still connected
    if (connected && !sf::Joystick::isConnected(controllerId)) {
        connected = false;
        spdlog::info("Controller {} disconnected", controllerId);

        // Try to find another connected controller
        for (int i = 0; i < sf::Joystick::Count; ++i) {
            if (sf::Joystick::isConnected(i)) {
                connected = true;
                controllerId = i;
                spdlog::info("Switched to controller {}", i);
                init();  // Reinitialize with new controller
                break;
            }
        }
    }
    // Check for new controller if none connected
    else if (!connected) {
        for (int i = 0; i < sf::Joystick::Count; ++i) {
            if (sf::Joystick::isConnected(i)) {
                connected = true;
                controllerId = i;
                spdlog::info("New controller connected on port {}", i);
                init();  // Initialize with new controller
                break;
            }
        }
    }
}

bool ControllerDevice::isActionPressed(Action action) {
    if (!connected) return false;

    // Check button bindings
    auto buttonIt = buttonBindings.find(action);
    if (buttonIt != buttonBindings.end()) {
        return sf::Joystick::isButtonPressed(controllerId, buttonIt->second);
    }

    // Check axis bindings
    auto axisIt = axisBindings.find(action);
    if (axisIt != axisBindings.end()) {
        float position = sf::Joystick::getAxisPosition(controllerId, axisIt->second);

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
        if (!connected) {
            controllerId = event.joystickConnect.joystickId;
            connected = true;
            spdlog::info("Controller {} connected", controllerId);
            init();
        }
    } else if (event.type == sf::Event::JoystickDisconnected) {
        if (event.joystickConnect.joystickId == controllerId) {
            connected = false;
            spdlog::info("Controller {} disconnected", controllerId);
        }
    }
}

void ControllerDevice::setButtonBinding(Action action, unsigned int button) {
    buttonBindings[action] = button;
}

void ControllerDevice::setAxisBinding(Action action, sf::Joystick::Axis axis) {
    axisBindings[action] = axis;
}

} // namespace game
