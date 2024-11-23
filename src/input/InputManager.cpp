#include "InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>

namespace game {

void InputManager::init() {
    spdlog::info("Initializing InputManager");

    // Add keyboard device by default
    auto keyboardDevice = std::make_unique<KeyboardDevice>();
    keyboardDevice->init();
    devices.push_back(std::move(keyboardDevice));

    spdlog::info("Input devices initialized");
}

void InputManager::update() {
    for (auto& device : devices) {
        device->update();
    }
}

void InputManager::handleEvent(const sf::Event& event) {
    // Handle controller connection/disconnection
    if (event.type == sf::Event::JoystickConnected) {
        auto controllerDevice = std::make_unique<ControllerDevice>();
        auto& config = ConfigManager::getInstance();
        controllerDevice->setDeadzone(config.getControllerDeadzone());
        controllerDevice->setSensitivity(config.getControllerSensitivity());
        controllerDevice->init();
        devices.push_back(std::move(controllerDevice));
        spdlog::info("Controller connected and initialized");
    }
    else if (event.type == sf::Event::JoystickDisconnected) {
        devices.erase(
            std::remove_if(devices.begin(), devices.end(),
                [](const auto& device) {
                    return dynamic_cast<ControllerDevice*>(device.get()) != nullptr;
                }
            ),
            devices.end()
        );
        spdlog::info("Controller disconnected and removed");
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

} // namespace game
