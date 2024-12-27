#include "ControllerDevice.hpp"
#include <spdlog/spdlog.h>
#include <string>

#include "../../config/ConfigManager.hpp"
#include "../mappers/ControllerMapper.hpp"

namespace game {

ControllerDevice::ControllerDevice(unsigned int controllerId): controllerId(controllerId){
    loadBinding();
}

void ControllerDevice::loadBinding() {
    spdlog::debug("Loading ControllerDevice binding for controller {}", controllerId);
    auto& config = ConfigManager::getInstance();
    auto mapper = ControllerMapper();
    clearBindingsAndStatesMap();

    // Load controller settings
    deadzone = config.getControllerDeadzone();
    sensitivity = config.getControllerSensitivity();

    // Load controller bindings from config
    for (const auto& [actionStr, action] : ActionUtil::getActionMap()) {
        auto controls = config.getControllerBindingsFromAction(actionStr);

        for (const auto& control : *controls) {
            std::string controlStr = control.value_or("");

            if (mapper.isAxis(controlStr)) {
                std::string axisKey = mapper.stringToAxisKey(controlStr);
                setAxisBinding(axisKey, action);
                spdlog::debug("Set controller axis binding: {} -> {}", controlStr, ActionUtil::toString(action));
            }
            else if (mapper.isButton(controlStr)) {
                unsigned int buttonId = mapper.stringToButtonId(controlStr);
                setBinding(buttonId, action);
                spdlog::debug("Set controller button binding: {} -> {}", controlStr, ActionUtil::toString(action));
            }
        }
    }
    spdlog::debug("ControllerDevice binding loaded");
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
    if (const auto* buttonPressed = event.getIf<sf::Event::JoystickButtonPressed>()) {
        if (buttonPressed->joystickId == controllerId) {
            setState(buttonPressed->button, true);
        }
    } else if (const auto* buttonReleased = event.getIf<sf::Event::JoystickButtonReleased>()) {
        if (buttonReleased->joystickId == controllerId) {
            setState(buttonReleased->button, false);
        }
    } else if (const auto* axisMoved = event.getIf<sf::Event::JoystickMoved>()) {
        if (axisMoved->joystickId == controllerId) {
            setAxisState(axisMoved->axis, axisMoved->position);
        }
    }
}

void ControllerDevice::setAxisBinding(std::string axis, Action action) {
    axisBindings[axis] = action;
}

void ControllerDevice::setAxisState(sf::Joystick::Axis axis, float position) {
    std::string axisStr = std::string(position > 0 ? "+" : "-") + std::to_string(static_cast<int>(axis));

    auto absPosition = std::abs(position) > deadzone;
    if (axisBindings.count(axisStr) != 0 && axisStates[axisStr].current != absPosition) {
        spdlog::debug("Controller {}: Axis {} moved to position {}", controllerId, static_cast<int>(axis), position);

        auto& state = axisStates[axisStr];
        state.previous = state.current;
        state.current = absPosition;

        if (axisBindings.count(axisStr) != 0) {
            // Convert axis to a unique key space separate from button IDs
            // Use high bits to ensure no overlap with button IDs
            unsigned int axisKey = 0x10000 | static_cast<unsigned int>(axis);
            setState(axisKey, absPosition);
        }
    }
}

} // namespace game
