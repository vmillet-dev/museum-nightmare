#include "HumanInputProvider.hpp"
#include <spdlog/spdlog.h>

namespace game {

HumanInputProvider::HumanInputProvider()
    : currentControllerType(ControllerType::None)
    , controllerConnected(false)
    , activeControllerId(0)
    , deadzone(20.0f) {

    // Default keyboard bindings
    keyboardBindings = {
        {Action::MoveLeft, sf::Keyboard::A},
        {Action::MoveRight, sf::Keyboard::D},
        {Action::MoveUp, sf::Keyboard::W},
        {Action::MoveDown, sf::Keyboard::S},
        {Action::Jump, sf::Keyboard::Space},
        {Action::Attack, sf::Keyboard::E},
        {Action::Interact, sf::Keyboard::F},
        {Action::Pause, sf::Keyboard::Escape}
    };

    // Default controller bindings (Xbox layout)
    controllerBindings = {
        {Action::Jump, 0},      // A button
        {Action::Attack, 2},    // X button
        {Action::Interact, 3},  // Y button
        {Action::Pause, 7}      // Start button
    };

    updateControllerState();
}

sf::Vector2f HumanInputProvider::getMovementInput() {
    sf::Vector2f movement(0.f, 0.f);

    // Keyboard input
    if (sf::Keyboard::isKeyPressed(keyboardBindings[Action::MoveLeft]))  movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(keyboardBindings[Action::MoveRight])) movement.x += 1.f;
    if (sf::Keyboard::isKeyPressed(keyboardBindings[Action::MoveUp]))    movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(keyboardBindings[Action::MoveDown]))  movement.y += 1.f;

    // Controller input (if connected)
    if (controllerConnected) {
        float axisX = getAxisValue(sf::Joystick::X);
        float axisY = getAxisValue(sf::Joystick::Y);

        // Only use controller input if it exceeds deadzone
        if (std::abs(axisX) > deadzone/100.f) movement.x = axisX;
        if (std::abs(axisY) > deadzone/100.f) movement.y = axisY;
    }

    // Normalize diagonal movement
    if (movement.x != 0.f && movement.y != 0.f) {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement.x /= length;
        movement.y /= length;
    }

    return movement;
}

bool HumanInputProvider::isActionTriggered(Action action) {
    // Check keyboard
    if (keyboardBindings.count(action) > 0) {
        if (sf::Keyboard::isKeyPressed(keyboardBindings[action])) {
            return true;
        }
    }

    // Check controller
    if (controllerConnected && controllerBindings.count(action) > 0) {
        return sf::Joystick::isButtonPressed(activeControllerId, controllerBindings[action]);
    }

    return false;
}

float HumanInputProvider::getActionStrength(Action action) {
    // For digital inputs (keyboard/buttons), return 1.0f if pressed
    if (isActionTriggered(action)) return 1.0f;
    return 0.0f;
}

void HumanInputProvider::update() {
    updateControllerState();
}

void HumanInputProvider::handleInputSourceChange() {
    updateControllerState();
}

ControllerType HumanInputProvider::detectControllerType() {
    if (!controllerConnected) return ControllerType::None;

    // Try to detect controller type based on button count and axes
    unsigned int buttonCount = sf::Joystick::getButtonCount(activeControllerId);
    bool hasAxis = sf::Joystick::hasAxis(activeControllerId, sf::Joystick::PovX);

    // Basic detection logic - can be expanded
    if (buttonCount >= 10 && hasAxis) {
        // Could add more sophisticated detection here
        return ControllerType::Xbox; // Default to Xbox for now
    }

    return ControllerType::None;
}

void HumanInputProvider::updateControllerState() {
    sf::Joystick::update();

    // Check for any connected controller
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            if (!controllerConnected || i != activeControllerId) {
                activeControllerId = i;
                controllerConnected = true;
                currentControllerType = detectControllerType();
                spdlog::info("Controller connected: ID {}", i);
                return;
            }
            return;
        }
    }

    if (controllerConnected) {
        controllerConnected = false;
        currentControllerType = ControllerType::None;
        spdlog::info("Controller disconnected");
    }
}

float HumanInputProvider::getAxisValue(sf::Joystick::Axis axis, unsigned int joystickId) {
    if (!controllerConnected) return 0.f;

    float value = sf::Joystick::getAxisPosition(joystickId, axis) / 100.f;
    return std::abs(value) > deadzone/100.f ? value : 0.f;
}

} // namespace game
