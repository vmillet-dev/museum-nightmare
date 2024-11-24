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

    // Initialize action states
    for (const auto& [action, _] : buttonBindings) {
        actionStates[action] = InputDevice::ActionState::NONE;
    }
    for (const auto& [action, _] : axisBindings) {
        actionStates[action] = InputDevice::ActionState::NONE;
    }

    // Load controller settings
    setDeadzone(config.getControllerDeadzone());
    setSensitivity(config.getControllerSensitivity());
}

InputDevice::ActionState ControllerDevice::getActionState(Action action) const {
    if (!connected) return InputDevice::ActionState::NONE;

    auto stateIt = actionStates.find(action);
    return stateIt != actionStates.end() ? stateIt->second : InputDevice::ActionState::NONE;
}

void ControllerDevice::handleEvent(const sf::Event& event) {
    // Handle controller connection/disconnection
    if (event.type == sf::Event::JoystickConnected) {
        if (!connected) {
            connected = true;
            controllerId = event.joystickConnect.joystickId;
            spdlog::info("Controller {} connected", controllerId);
            init();
        }
    } else if (event.type == sf::Event::JoystickDisconnected &&
               event.joystickConnect.joystickId == controllerId) {
        connected = false;
        spdlog::info("Controller {} disconnected", controllerId);
        return;
    }

    if (!connected) return;

    // Handle button events
    if (event.type == sf::Event::JoystickButtonPressed ||
        event.type == sf::Event::JoystickButtonReleased) {
        for (const auto& [action, button] : buttonBindings) {
            if (event.joystickButton.button == button) {
                InputDevice::ActionState newState;
                if (event.type == sf::Event::JoystickButtonPressed) {
                    newState = actionStates[action] == InputDevice::ActionState::NONE ?
                              InputDevice::ActionState::JUST_PRESSED : InputDevice::ActionState::PRESSED;
                    spdlog::debug("Controller button {} pressed for action {}",
                                button, static_cast<int>(action));
                } else {
                    newState = InputDevice::ActionState::RELEASED;
                }
                actionStates[action] = newState;
            }
        }
    }

    // Handle axis movement
    if (event.type == sf::Event::JoystickMoved) {
        float position = event.joystickMove.position;
        // Apply deadzone with smoother transition
        if (std::abs(position) < deadzone) {
            position = 0.0f;
        } else {
            float normalizedPosition = (std::abs(position) - deadzone) / (100.0f - deadzone);
            position = (position > 0 ? 1 : -1) * normalizedPosition * sensitivity;
        }

        for (const auto& [action, axis] : axisBindings) {
            if (event.joystickMove.axis == axis) {
                InputDevice::ActionState newState = InputDevice::ActionState::NONE;
                bool isActive = false;

                switch(action) {
                    case Action::MoveUp:
                        isActive = position < -0.3f;  // More sensitive threshold
                        break;
                    case Action::MoveDown:
                        isActive = position > 0.3f;   // More sensitive threshold
                        break;
                    case Action::MoveLeft:
                        isActive = position < -0.3f;  // More sensitive threshold
                        break;
                    case Action::MoveRight:
                        isActive = position > 0.3f;   // More sensitive threshold
                        break;
                    default:
                        break;
                }

                if (isActive) {
                    auto currentState = actionStates[action];
                    newState = (currentState == InputDevice::ActionState::NONE ||
                              currentState == InputDevice::ActionState::RELEASED) ?
                              InputDevice::ActionState::JUST_PRESSED : InputDevice::ActionState::PRESSED;
                    spdlog::debug("Controller axis {} value: {} for action {}",
                                static_cast<int>(axis), position, static_cast<int>(action));
                } else {
                    newState = actionStates[action] != InputDevice::ActionState::NONE ?
                              InputDevice::ActionState::RELEASED : InputDevice::ActionState::NONE;
                }

                actionStates[action] = newState;
            }
        }
    }

    // Reset JUST_PRESSED and RELEASED states after they've been consumed
    for (auto& [action, state] : actionStates) {
        if (state == InputDevice::ActionState::JUST_PRESSED) {
            state = InputDevice::ActionState::PRESSED;
        } else if (state == InputDevice::ActionState::RELEASED) {
            state = InputDevice::ActionState::NONE;
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
