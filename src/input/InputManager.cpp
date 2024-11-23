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

    // Check for already connected controllers
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            try {
                auto controllerDevice = std::make_unique<ControllerDevice>();
                auto& config = ConfigManager::getInstance();
                controllerDevice->setDeadzone(config.getControllerDeadzone());
                controllerDevice->setSensitivity(config.getControllerSensitivity());
                controllerDevice->init();
                devices.push_back(std::move(controllerDevice));
                spdlog::info("Found and initialized existing controller at index {}", i);
            } catch (const std::exception& e) {
                spdlog::error("Failed to initialize existing controller at index {}: {}", i, e.what());
            }
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
    try {
        switch (event.type) {
            case sf::Event::JoystickConnected: {
                auto controllerDevice = std::make_unique<ControllerDevice>();
                auto& config = ConfigManager::getInstance();
                controllerDevice->setDeadzone(config.getControllerDeadzone());
                controllerDevice->setSensitivity(config.getControllerSensitivity());
                controllerDevice->init();
                devices.push_back(std::move(controllerDevice));
                spdlog::info("Controller connected and initialized");
                break;
            }
            case sf::Event::JoystickDisconnected: {
                auto it = std::find_if(devices.begin(), devices.end(),
                    [](const auto& device) { return dynamic_cast<ControllerDevice*>(device.get()) != nullptr; }
                );
                if (it != devices.end()) {
                    try {
                        devices.erase(it);
                        spdlog::info("Controller disconnected and removed");
                    } catch (const std::exception& e) {
                        spdlog::error("Error removing controller: {}", e.what());
                    }
                }
                break;
            }
            default:
                break;
        }
    } catch (const std::exception& e) {
        spdlog::error("Error handling controller event: {}", e.what());
    }

    // Forward events to all devices
    for (auto& device : devices) {
        try {
            device->handleEvent(event);
        } catch (const std::exception& e) {
            spdlog::error("Error in device event handling: {}", e.what());
        }
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
