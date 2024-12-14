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
    if ((event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased) &&
        event.joystickButton.joystickId == controllerId) {
        setState(event.joystickButton.button, event.type == sf::Event::JoystickButtonPressed);
    }

    if (event.type == sf::Event::JoystickMoved && event.joystickMove.joystickId == controllerId) {
        setAxisState(event.joystickMove.axis, event.joystickMove.position);
    }
}

void ControllerDevice::setAxisBinding(std::string axis, Action action) {
    axisBindings[axis] = action;
}

void ControllerDevice::setAxisState(unsigned int axisId, float position) {
    std::string axis = std::string(position > 0 ? "+" : "-") + std::to_string(axisId);

    auto absPosition = std::abs(position) > deadzone;
    if (axisBindings.count(axis) != 0 && axisStates[axis].current != absPosition) {
        spdlog::debug("Controller {}: Axis {} moved to position {} ({})", controllerId, axisId, position, axis);

        auto& state = axisStates[axis];
        state.previous = state.current;
        state.current = absPosition;
    }
}

} // namespace game
