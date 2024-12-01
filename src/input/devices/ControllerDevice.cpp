#include "ControllerDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <string>

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

    if (!connected) {
        spdlog::warn("No controller connected");
        return;
    }

    // Load controller bindings from config for each action
    const std::vector<std::string> actions = {"MoveUp", "MoveDown", "MoveLeft", "MoveRight", "Pause", "Confirm", "Cancel", "Fire"};

    for (const auto& actionStr : actions) {
        Action action = config.getActionFromString(actionStr);
        auto controls = config.getControllerBindingsForAction(actionStr);

        for (const auto& control : controls) {
            if (control.find("Stick") != std::string::npos) {
                setAxisBinding(control, action);
                spdlog::debug("Set controller axis binding: {} -> {}", control, static_cast<int>(action));
            } else if (control.find("Trigger") != std::string::npos) {
                // Handle triggers as buttons
                setButtonBinding(7, action); // Right trigger is usually button 7
                spdlog::debug("Set controller trigger binding: {} -> {}", 7, static_cast<int>(action));
            } else if (control == "A") {
                setButtonBinding(0, action);
            } else if (control == "B") {
                setButtonBinding(1, action);
            } else if (control == "Start") {
                setButtonBinding(7, action);
            }
        }
    }
}

void ControllerDevice::update() {
    GenericInputDevice<unsigned int>::update();  // Update button states

    for (const auto& binding : axisBindings) {
        if (axisStates[binding.first].current != axisStates[binding.first].previous) {
            axisStates[binding.first].previous = axisStates[binding.first].current;
        }
    }
}

bool ControllerDevice::isActionPressed(Action action) {
    if (GenericInputDevice<unsigned int>::isActionPressed(action)) {
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
    if (GenericInputDevice<unsigned int>::isActionJustPressed(action)) {
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
    if (GenericInputDevice<unsigned int>::isActionReleased(action)) {
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
    setBinding(button, action);
}

void ControllerDevice::setButtonState(unsigned int button, bool pressed) {
    if (bindings.count(button)) {
        spdlog::debug("Controller {}: Button {} {}", controllerId, button, pressed ? "Pressed" : "Released");
        setState(button, pressed);
    }
}

void ControllerDevice::setAxisBinding(std::string axis, Action action) {
    axisBindings[axis] = action;
    // Initialize axis state when binding is set
    if (axisStates.find(axis) == axisStates.end()) {
        axisStates[axis] = ActionState{false, false};
    }
}

void ControllerDevice::setAxisState(unsigned int axisId, float position) {
    std::string axis = (position > 0 ? "+" : "-") + std::to_string(axisId);

    auto absPosition = std::abs(position) > deadzone;
    if (axisBindings.count(axis) != 0 && axisStates[axis].current != absPosition) {
        spdlog::debug("Controller {}: Axis {} moved to position {} ({})", controllerId, axisId, position, axis);

        auto& state = axisStates[axis];
        state.previous = state.current;
        state.current = absPosition;
    }
}

} // namespace game
