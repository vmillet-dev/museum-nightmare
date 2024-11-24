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
        actionStates[action] = ActionState::NONE;
    }
    for (const auto& [action, _] : axisBindings) {
        actionStates[action] = ActionState::NONE;
    }

    // Load controller settings
    setDeadzone(config.getControllerDeadzone());
    setSensitivity(config.getControllerSensitivity());
}

InputDevice::ActionState ControllerDevice::getActionState(Action action) const {
    if (!connected) return ActionState::NONE;

    auto stateIt = actionStates.find(action);
    return stateIt != actionStates.end() ? stateIt->second : ActionState::NONE;
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
                ActionState newState;
                if (event.type == sf::Event::JoystickButtonPressed) {
                    newState = actionStates[action] == ActionState::NONE ?
                              ActionState::JUST_PRESSED : ActionState::PRESSED;
                    spdlog::debug("Controller button {} pressed for action {}",
                                button, static_cast<int>(action));
                } else {
                    newState = ActionState::RELEASED;
                }
                actionStates[action] = newState;
            }
        }
    }

    // Handle axis movement
    if (event.type == sf::Event::JoystickMoved) {
        float position = event.joystickMove.position;
        spdlog::debug("Raw joystick position: {}", position);

        // Apply deadzone with smoother transition
        if (std::abs(position) < deadzone) {
            position = 0.0f;
        } else {
            float normalizedPosition = (std::abs(position) - deadzone) / (100.0f - deadzone);
            position = (position > 0 ? 1 : -1) * normalizedPosition * sensitivity;
            spdlog::debug("Normalized position after deadzone: {}", position);
        }

        for (const auto& [action, axis] : axisBindings) {
            if (event.joystickMove.axis == axis) {
                ActionState newState = ActionState::NONE;
                bool isActive = false;

                // More forgiving thresholds for menu navigation
                const float MENU_THRESHOLD = 0.5f;

                switch(action) {
                    case Action::MoveUp:
                        isActive = position < -MENU_THRESHOLD;
                        break;
                    case Action::MoveDown:
                        isActive = position > MENU_THRESHOLD;
                        break;
                    case Action::MoveLeft:
                        isActive = position < -MENU_THRESHOLD;
                        break;
                    case Action::MoveRight:
                        isActive = position > MENU_THRESHOLD;
                        break;
                    default:
                        break;
                }

                if (isActive) {
                    auto currentState = actionStates[action];
                    newState = (currentState == ActionState::NONE ||
                              currentState == ActionState::RELEASED) ?
                              ActionState::JUST_PRESSED : ActionState::PRESSED;
                    spdlog::debug("Controller axis {} activated: position={}, action={}, newState={}",
                                static_cast<int>(axis), position, static_cast<int>(action),
                                static_cast<int>(newState));
                } else {
                    newState = actionStates[action] != ActionState::NONE ?
                              ActionState::RELEASED : ActionState::NONE;
                }

                actionStates[action] = newState;
            }
        }
    }

    // Reset JUST_PRESSED and RELEASED states after they've been consumed
    for (auto& [action, state] : actionStates) {
        if (state == ActionState::JUST_PRESSED) {
            state = ActionState::PRESSED;
        } else if (state == ActionState::RELEASED) {
            state = ActionState::NONE;
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
