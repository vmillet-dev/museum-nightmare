#include "ControllerDevice.hpp"
#include "../../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

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
    setButtonBinding(config.getControllerButton("controller_confirm"), Action::Confirm);
    setButtonBinding(config.getControllerButton("controller_cancel"), Action::Cancel);
    setButtonBinding(config.getControllerButton("controller_pause"), Action::Pause);

    // Load axis bindings from config
    setAxisBinding(config.getControllerAxis("controller_move_up"), Action::MoveUp);
    setAxisBinding(config.getControllerAxis("controller_move_down"), Action::MoveDown);
    setAxisBinding(config.getControllerAxis("controller_move_left"), Action::MoveLeft);
    setAxisBinding(config.getControllerAxis("controller_move_right"), Action::MoveRight);

    // Load controller settings
    setDeadzone(config.getControllerDeadzone());
    setSensitivity(config.getControllerSensitivity());
}

void ControllerDevice::update() {
    // Update button states
    for (const auto& binding : buttonBindings) {
        auto& state = buttonStates[binding.first];
        if (state.current != state.previous) {
            spdlog::debug("Button state updated - Action: {}, Current: {}, Previous: {}",
                         static_cast<int>(binding.second), state.current, state.previous);
            state.previous = state.current;
        }
    }

    // Update axis states and check for continuous input
    for (const auto& binding : axisBindings) {
        auto& state = axisStates[binding.first];
        if (state.current != state.previous) {
            spdlog::debug("Axis state updated - Action: {}, Current: {}, Previous: {}",
                         static_cast<int>(binding.second), state.current, state.previous);
            state.previous = state.current;
        }
    }
}

bool ControllerDevice::isActionPressed(Action action) {

    for (const auto& binding : buttonBindings) {
        if (binding.second == action && buttonStates[binding.first].current) {
            return true;
        }
    }

    for (const auto& binding : axisBindings) {
        if (binding.second == action && axisStates[binding.first].current) {
            return true;
        }
    }

    return false;
}

bool ControllerDevice::isActionJustPressed(Action action) {
    for (const auto& binding : buttonBindings) {
        const auto& state = buttonStates[binding.first];
        if (binding.second == action && state.current != state.previous && state.current) {
            return true;
        }
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
    for (const auto& binding : buttonBindings) {
        const auto& state = buttonStates[binding.first];
        if (binding.second == action && state.current != state.previous && !state.current) {
            return true;
        }
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

    if ((event.type == sf::Event::JoystickButtonPressed || event.type == sf::Event::JoystickButtonReleased) && event.joystickButton.joystickId == controllerId) {
        setButtonState(event.joystickButton.button, event.type == sf::Event::JoystickButtonPressed);
    }

    if (event.type == sf::Event::JoystickMoved && event.joystickMove.joystickId == controllerId) {
        setAxisState(event.joystickMove.axis, event.joystickMove.position);
    }
}

void ControllerDevice::setButtonBinding(unsigned int button, Action action) {
    buttonBindings[button] = action;
}

void ControllerDevice::setButtonState(unsigned int button, bool pressed) {

    if (buttonBindings.count(button)) {
        spdlog::debug("Controller {}: Button {} {}", controllerId, button, pressed ? "Pressed" : "Released");

        auto& state = buttonStates[button];
        state.previous = state.current;
        state.current = pressed;
    }
}

void ControllerDevice::setAxisBinding(std::string axis, Action action) {
    axisBindings[axis] = action;
}

void ControllerDevice::setAxisState(unsigned int axisId, float position) {
    // Normalize position from [-100, 100] to [-1, 1]
    float normalizedPos = position / 100.0f;

    // Check both positive and negative directions
    std::string posAxis = fmt::format("+{}", axisId);
    std::string negAxis = fmt::format("-{}", axisId);

    bool exceedsDeadzone = std::abs(normalizedPos) > deadzone;
    float sensitivity = this->sensitivity;

    spdlog::debug("Axis {}: raw={:.2f}, normalized={:.2f}, exceeds_deadzone={}, sensitivity={:.2f}",
                 axisId, position, normalizedPos, exceedsDeadzone, sensitivity);

    // Update positive direction state
    if (axisBindings.count(posAxis)) {
        auto& state = axisStates[posAxis];
        bool newState = exceedsDeadzone && normalizedPos > 0;
        if (state.current != newState) {
            state.previous = state.current;
            state.current = newState;
            spdlog::debug("Axis {} positive state changed: {} -> {}", axisId, state.previous, state.current);
        }
    }

    // Update negative direction state
    if (axisBindings.count(negAxis)) {
        auto& state = axisStates[negAxis];
        bool newState = exceedsDeadzone && normalizedPos < 0;
        if (state.current != newState) {
            state.previous = state.current;
            state.current = newState;
            spdlog::debug("Axis {} negative state changed: {} -> {}", axisId, state.previous, state.current);
        }
    }
}

} // namespace game
