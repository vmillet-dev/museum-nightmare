#include "InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void InputManager::init() {
    spdlog::info("Initializing InputManager");
    auto& config = ConfigManager::getInstance();

    // Set up default key bindings
    keyBindings[config.getKeyBinding("move_up")] = Action::MoveUp;
    keyBindings[config.getKeyBinding("move_down")] = Action::MoveDown;
    keyBindings[config.getKeyBinding("move_left")] = Action::MoveLeft;
    keyBindings[config.getKeyBinding("move_right")] = Action::MoveRight;
    keyBindings[config.getKeyBinding("pause")] = Action::Pause;
    keyBindings[config.getKeyBinding("confirm")] = Action::Confirm;
    keyBindings[config.getKeyBinding("cancel")] = Action::Cancel;
    spdlog::debug("KB: ZQSD={}{}{}{}", config.getKeyBinding("move_up"), config.getKeyBinding("move_down"), config.getKeyBinding("move_left"), config.getKeyBinding("move_right"));

    // Set up controller button bindings
    controllerButtonBindings[Action::MoveUp] = config.getControllerButton("controller_move_up");
    controllerButtonBindings[Action::MoveDown] = config.getControllerButton("controller_move_down");
    controllerButtonBindings[Action::MoveLeft] = config.getControllerButton("controller_move_left");
    controllerButtonBindings[Action::MoveRight] = config.getControllerButton("controller_move_right");
    controllerButtonBindings[Action::Pause] = config.getControllerButton("controller_pause");
    controllerButtonBindings[Action::Confirm] = config.getControllerButton("controller_confirm");
    controllerButtonBindings[Action::Cancel] = config.getControllerButton("controller_cancel");
    spdlog::debug("Ctrl bind: U{} D{} L{} R{} P{} OK{} C{}",
                 controllerButtonBindings[Action::MoveUp], controllerButtonBindings[Action::MoveDown],
                 controllerButtonBindings[Action::MoveLeft], controllerButtonBindings[Action::MoveRight],
                 controllerButtonBindings[Action::Pause], controllerButtonBindings[Action::Confirm],
                 controllerButtonBindings[Action::Cancel]);

    // Initialize controller settings
    hasController = config.isControllerEnabled() && sf::Joystick::isConnected(0);
    controllerDeadzone = config.getControllerDeadzone();
    controllerSensitivity = config.getControllerSensitivity();

    // Initialize axis bindings
    controllerAxisBindings[Action::MoveUp] = sf::Joystick::Y;
    controllerAxisBindings[Action::MoveDown] = sf::Joystick::Y;
    controllerAxisBindings[Action::MoveLeft] = sf::Joystick::X;
    controllerAxisBindings[Action::MoveRight] = sf::Joystick::X;

    spdlog::info("Input: KB(ZQSD), Ctrl(en:{},conn:{},dz:{:.1f},sens:{:.1f})",
                 config.isControllerEnabled(), sf::Joystick::isConnected(0),
                 controllerDeadzone, controllerSensitivity);

    if (hasController) {
        spdlog::info("Controller: {} buttons, X:{}, Y:{}, Sens:{:.1f}",
                    sf::Joystick::getButtonCount(0),
                    sf::Joystick::hasAxis(0, sf::Joystick::X),
                    sf::Joystick::hasAxis(0, sf::Joystick::Y),
                    controllerSensitivity);
    }

    // Initialize all key states to false
    for (const auto& binding : keyBindings) {
        keyStates[binding.first] = false;
    }
}

void InputManager::bindAction(Action action, std::function<void()> callback) {
    actionCallbacks[action] = callback;
    spdlog::debug("Bound callback to action {}", static_cast<int>(action));
}

void InputManager::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
        auto it = keyBindings.find(event.key.code);
        if (it != keyBindings.end()) {
            // Update key state
            keyStates[event.key.code] = (event.type == sf::Event::KeyPressed);
            spdlog::debug("Key {} {}", static_cast<int>(event.key.code),
                         (event.type == sf::Event::KeyPressed) ? "pressed" : "released");

            if (event.type == sf::Event::KeyPressed) {
                auto callbackIt = actionCallbacks.find(it->second);
                if (callbackIt != actionCallbacks.end()) {
                    callbackIt->second();
                }
            }
        }
    }
}

void InputManager::setKeyState(sf::Keyboard::Key key, bool pressed) {
    keyStates[key] = pressed;
    spdlog::debug("Key state manually set - Key: {}, Pressed: {}", static_cast<int>(key), pressed);
}

bool InputManager::isActionPressed(Action action) {
    bool isPressed = false;
    bool keyboardPressed = false;
    bool joystickPressed = false;

    // Helper lambda to find key for action
    auto findKeyForAction = [this](Action action) -> sf::Keyboard::Key {
        for (const auto& binding : keyBindings) {
            if (binding.second == action) {
                return binding.first;
            }
        }
        return sf::Keyboard::Unknown;
    };

    // Check keyboard input
    sf::Keyboard::Key key = findKeyForAction(action);
    keyboardPressed = keyStates[key];

    // Check controller input if enabled
    if (hasController) {
        float xAxis = (sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100.0f) * controllerSensitivity;
        float yAxis = (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100.0f) * controllerSensitivity;

        switch (action) {
            case Action::MoveUp: {
                float yAxis = (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100.0f) * controllerSensitivity;
                joystickPressed = yAxis < -controllerDeadzone || sf::Joystick::isButtonPressed(0, controllerButtonBindings[action]);
            } break;
            case Action::MoveDown: {
                float yAxis = (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100.0f) * controllerSensitivity;
                joystickPressed = yAxis > controllerDeadzone || sf::Joystick::isButtonPressed(0, controllerButtonBindings[action]);
            } break;
            case Action::MoveLeft: {
                float xAxis = (sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100.0f) * controllerSensitivity;
                joystickPressed = xAxis < -controllerDeadzone || sf::Joystick::isButtonPressed(0, controllerButtonBindings[action]);
            } break;
            case Action::MoveRight: {
                float xAxis = (sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100.0f) * controllerSensitivity;
                joystickPressed = xAxis > controllerDeadzone || sf::Joystick::isButtonPressed(0, controllerButtonBindings[action]);
            } break;
            default:
                if (auto it = controllerButtonBindings.find(action); it != controllerButtonBindings.end()) {
                    joystickPressed = sf::Joystick::isButtonPressed(0, it->second);
                    if (joystickPressed) spdlog::debug("Button {} pressed for action {}", it->second, static_cast<int>(action));
                }
                break;
        }
    }

    isPressed = keyboardPressed || joystickPressed;
    if (isPressed && hasController) {
        spdlog::debug("Input[{}]: KB:{} Ctrl:{} XY({:.1f},{:.1f})",
            static_cast<int>(action), keyboardPressed, joystickPressed,
            (sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100.0f) * controllerSensitivity,
            (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100.0f) * controllerSensitivity);
    }
    return isPressed;
}

void InputManager::update() {
    // Check controller connection status
    bool wasControllerConnected = hasController;
    hasController = sf::Joystick::isConnected(0);

    if (wasControllerConnected != hasController) {
        spdlog::info("Controller {}: Btn:{} Axes(X:{},Y:{})",
            hasController?"connected":"disconnected",
            sf::Joystick::getButtonCount(0),
            sf::Joystick::hasAxis(0, sf::Joystick::X),
            sf::Joystick::hasAxis(0, sf::Joystick::Y));
    }

    // Log controller axis positions when connected
    if (hasController) {
        spdlog::debug("Axes: XY({:.1f},{:.1f})",
            (sf::Joystick::getAxisPosition(0, sf::Joystick::X) / 100.0f) * controllerSensitivity,
            (sf::Joystick::getAxisPosition(0, sf::Joystick::Y) / 100.0f) * controllerSensitivity);
    }
}

} // namespace game
