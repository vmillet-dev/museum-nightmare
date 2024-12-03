#include "ControllerDevice.hpp"
#include <spdlog/spdlog.h>
#include <string>

#include "../../config/ConfigManager.hpp"
#include "../mappers/ControllerMapper.hpp"

namespace game {

void ControllerDevice::init() {
    auto& config = ConfigManager::getInstance();

    // Load controller settings
    deadzone = config.getControllerDeadzone();
    sensitivity = config.getControllerSensitivity();

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

    if (!connected) {
        spdlog::warn("No controller connected");
        return;
    }

    // Load controller bindings from config
    for (const auto& [actionStr, action] : ActionUtil::getActionMap()) {
        auto controls = config.getControllerBindingsForAction(actionStr);

        for (const auto& control : *controls) {
            std::string controlStr = control.value_or("");
            if (ControllerMapper::isAxis(controlStr)) {
                unsigned int axisId = ControllerMapper::mapAxisId(controlStr);
                std::string axisKey = (ControllerMapper::isAxisPositive(controlStr) ? "+" : "-") + std::to_string(axisId);
                setAxisBinding(axisKey, action);
                spdlog::debug("Set controller axis binding: {} -> {}", controlStr, ActionUtil::toString(action));
            }
            else if (ControllerMapper::isButton(controlStr)) {
                unsigned int buttonId = ControllerMapper::mapButtonName(controlStr);
                setButtonBinding(buttonId, action);
                spdlog::debug("Set controller button binding: {} -> {}", controlStr, ActionUtil::toString(action));

            }
        }
    }
}

void ControllerDevice::update() {
    GenericInputDevice<unsigned int>::update();  // Handle button states

    for (const auto& binding : axisBindings) {
        if (axisStates[binding.first].current != axisStates[binding.first].previous) {
            axisStates[binding.first].previous = axisStates[binding.first].current;
        }
    }
}

bool ControllerDevice::isActionPressed(Action action) {
    if (GenericInputDevice<unsigned int>::isActionPressed(action)) {  // Check button states
        return true;
    }

    for (const auto& binding : axisBindings) {
        if (binding.second == action && axisStates[binding.first].current) {
            return true;
        }
    }
    return false;
}

bool ControllerDevice::isActionJustPressed(Action action) {
    if (GenericInputDevice<unsigned int>::isActionJustPressed(action)) {  // Check button states
        return true;
    }

    for (const auto& binding : axisBindings) {
        const auto& state = axisStates[binding.first];
        if (binding.second == action && state.current != state.previous && state.current) {
            return true;
        }
    }
    return false;
}

bool ControllerDevice::isActionReleased(Action action) {
    if (GenericInputDevice<unsigned int>::isActionReleased(action)) {  // Check button states
        return true;
    }

    for (const auto& binding : axisBindings) {
        const auto& state = axisStates[binding.first];
        if (binding.second == action && state.current != state.previous && !state.current) {
            return true;
        }
    }
    return false;
}

void ControllerDevice::handleEvent(const sf::Event& event) {
    if ((event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased) &&
        event.joystickButton.joystickId == controllerId) {
        setButtonState(event.joystickButton.button, event.type == sf::Event::JoystickButtonPressed);
    }

    if (event.type == sf::Event::JoystickMoved && event.joystickMove.joystickId == controllerId) {
        setAxisState(event.joystickMove.axis, event.joystickMove.position);
    }
}

void ControllerDevice::setButtonBinding(unsigned int button, Action action) {
    GenericInputDevice<unsigned int>::setBinding(button, action);
}

void ControllerDevice::setButtonState(unsigned int button, bool pressed) {
    if (GenericInputDevice<unsigned int>::hasBinding(button)) {
        spdlog::debug("Controller {}: Button {} {}", controllerId, button, pressed ? "Pressed" : "Released");
        GenericInputDevice<unsigned int>::setState(button, pressed);
    }
}

void ControllerDevice::setAxisBinding(std::string axis, Action action) {
    axisBindings[axis] = action;
}

void ControllerDevice::setAxisState(unsigned int axisId, float position) {
    std::string axis = std::string(position > 0 ? "+" : "-") + std::to_string(axisId);

    auto absPosition = std::abs(position) > (deadzone / 100.0f * 100.0f); // Convert from percentage to SFML's -100 to 100 range
    if (axisBindings.count(axis) != 0 && axisStates[axis].current != absPosition) {
        spdlog::debug("Controller {}: Axis {} moved to position {} ({})", controllerId, axisId, position, axis);

        auto& state = axisStates[axis];
        state.previous = state.current;
        state.current = absPosition;
    }
}

} // namespace game
