#include "InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>

namespace game {

void InputManager::createControllerDevice() {
    auto controllerDevice = std::make_unique<ControllerDevice>();
    auto& config = ConfigManager::getInstance();
    controllerDevice->setDeadzone(config.getControllerDeadzone());
    controllerDevice->setSensitivity(config.getControllerSensitivity());
    controllerDevice->init();
    devices.push_back(std::move(controllerDevice));
    spdlog::info("Controller connected and initialized");
}

void InputManager::removeControllerDevice() {
    devices.erase(
        std::remove_if(devices.begin(), devices.end(),
            [](const auto& device) { return dynamic_cast<ControllerDevice*>(device.get()) != nullptr; }
        ),
        devices.end()
    );
    spdlog::info("Controller disconnected and removed");
}

void InputManager::init() {
    spdlog::info("Initializing InputManager");

    // Add keyboard device by default
    auto keyboardDevice = std::make_unique<KeyboardDevice>();
    keyboardDevice->init();
    devices.push_back(std::move(keyboardDevice));

    // Add mouse device
    auto mouseDevice = std::make_unique<MouseDevice>(window);
    mouseDevice->init();
    devices.push_back(std::move(mouseDevice));

    // Check for already connected controllers
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            createControllerDevice();
        }
    }

    spdlog::info("Input devices initialized");
}

void InputManager::update() {
    for (auto& device : devices) {
        device->update();
    }
}

void InputManager::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::JoystickConnected) {
        createControllerDevice();
    }
    else if (event.type == sf::Event::JoystickDisconnected) {
        removeControllerDevice();
    }

    // Forward events to all devices
    for (auto& device : devices) {
        device->handleEvent(event);
    }
}

bool InputManager::isActionPressed(Action action) {
    for (auto& device : devices) {
        if (device->isActionPressed(action)) {
            return true;
        }
    }
    return false;
}

bool InputManager::isActionJustPressed(Action action) {
    for (auto& device : devices) {
        if (device->isActionJustPressed(action)) {
            return true;
        }
    }
    return false;
}

bool InputManager::isActionReleased(Action action) {
    for (auto& device : devices) {
        if (device->isActionReleased(action)) {
            return true;
        }
    }
    return false;
}

} // namespace game
