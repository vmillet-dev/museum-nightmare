#include "InputManager.hpp"
#include "devices/KeyboardDevice.hpp"
#include "devices/ControllerDevice.hpp"
#include <spdlog/spdlog.h>

namespace game {

void InputManager::init() {
    spdlog::info("Initializing InputManager with device-based system");

    // Always add keyboard device
    devices["keyboard"] = std::make_unique<KeyboardDevice>();
    lastActiveDevice = "keyboard";

    // Try to connect controller if available
    tryConnectController();
}

void InputManager::tryConnectController() {
    if (sf::Joystick::isConnected(0)) {
        devices["controller0"] = std::make_unique<ControllerDevice>(0);
        spdlog::info("Controller connected and initialized");
    }
}

void InputManager::updateDevices() {
    // Check for controller connection/disconnection
    bool wasControllerConnected = devices.find("controller0") != devices.end();
    bool isControllerConnected = sf::Joystick::isConnected(0);

    if (wasControllerConnected && !isControllerConnected) {
        devices.erase("controller0");
        spdlog::info("Controller disconnected");
        if (lastActiveDevice == "controller0") {
            lastActiveDevice = "keyboard";
        }
    } else if (!wasControllerConnected && isControllerConnected) {
        tryConnectController();
    }
}

void InputManager::update() {
    updateDevices();

    // Update all connected devices
    for (auto& [id, device] : devices) {
        device->update();
    }
}

void InputManager::handleInput(const sf::Event& event) {
    // Forward event to all devices
    for (auto& [id, device] : devices) {
        device->handleEvent(event);

        // Update last active device based on input
        if (event.type == sf::Event::KeyPressed && id == "keyboard") {
            lastActiveDevice = id;
        } else if ((event.type == sf::Event::JoystickButtonPressed ||
                   event.type == sf::Event::JoystickMoved) &&
                  id == "controller0") {
            lastActiveDevice = id;
        }
    }
}

bool InputManager::isActionPressed(Action action) const {
    // First check the last active device
    auto lastDeviceIt = devices.find(lastActiveDevice);
    if (lastDeviceIt != devices.end() && lastDeviceIt->second->isActionPressed(action)) {
        return true;
    }

    // Then check other devices
    for (const auto& [id, device] : devices) {
        if (id != lastActiveDevice && device->isActionPressed(action)) {
            return true;
        }
    }

    return false;
}

} // namespace game
