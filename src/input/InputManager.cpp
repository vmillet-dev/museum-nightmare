#include "InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void InputManager::init() {
    spdlog::info("Initializing InputManager");

    keyboardDevice = std::make_unique<KeyboardDevice>();
    controllerDevice = std::make_unique<ControllerDevice>();

    auto& config = ConfigManager::getInstance();

    // Initialize keyboard device with config bindings
    keyboardDevice->init();
    keyboardDevice->setKeyBinding(Action::MoveUp, config.getKeyBinding("move_up"));
    keyboardDevice->setKeyBinding(Action::MoveDown, config.getKeyBinding("move_down"));
    keyboardDevice->setKeyBinding(Action::MoveLeft, config.getKeyBinding("move_left"));
    keyboardDevice->setKeyBinding(Action::MoveRight, config.getKeyBinding("move_right"));
    keyboardDevice->setKeyBinding(Action::Pause, config.getKeyBinding("pause"));
    keyboardDevice->setKeyBinding(Action::Confirm, config.getKeyBinding("confirm"));
    keyboardDevice->setKeyBinding(Action::Cancel, config.getKeyBinding("cancel"));

    // Initialize controller device with config settings
    controllerDevice->setDeadzone(config.getControllerDeadzone());
    controllerDevice->setSensitivity(config.getControllerSensitivity());
    controllerDevice->init();
    controllerDevice->setButtonBinding(Action::MoveUp, config.getControllerButton("controller_move_up"));
    controllerDevice->setButtonBinding(Action::MoveDown, config.getControllerButton("controller_move_down"));
    controllerDevice->setButtonBinding(Action::MoveLeft, config.getControllerButton("controller_move_left"));
    controllerDevice->setButtonBinding(Action::MoveRight, config.getControllerButton("controller_move_right"));
    controllerDevice->setButtonBinding(Action::Pause, config.getControllerButton("controller_pause"));
    controllerDevice->setButtonBinding(Action::Confirm, config.getControllerButton("controller_confirm"));
    controllerDevice->setButtonBinding(Action::Cancel, config.getControllerButton("controller_cancel"));

    // Set up axis bindings
    controllerDevice->setAxisBinding(Action::MoveUp, sf::Joystick::Y);
    controllerDevice->setAxisBinding(Action::MoveDown, sf::Joystick::Y);
    controllerDevice->setAxisBinding(Action::MoveLeft, sf::Joystick::X);
    controllerDevice->setAxisBinding(Action::MoveRight, sf::Joystick::X);

    spdlog::info("Input devices initialized with configuration settings");
}

void InputManager::update() {
    keyboardDevice->update();
    controllerDevice->update();
}

void InputManager::handleEvent(const sf::Event& event) {
    keyboardDevice->handleEvent(event);
    controllerDevice->handleEvent(event);
}

bool InputManager::isActionPressed(Action action) {
    return keyboardDevice->isActionPressed(action) || controllerDevice->isActionPressed(action);
}

} // namespace game
