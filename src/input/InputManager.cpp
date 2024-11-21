#include "InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void InputManager::init() {
    // Create and add default devices
    auto keyboard = std::make_unique<KeyboardDevice>();
    auto controller = std::make_unique<ControllerDevice>(0); // Primary controller

    addDevice(std::move(keyboard));
    addDevice(std::move(controller));

    spdlog::info("Input Manager initialized with default devices");
}

void InputManager::update() {
    for (auto& device : devices) {
        device->update();
    }
}

void InputManager::handleInput(const sf::Event& event) {
    for (auto& device : devices) {
        device->handleEvent(event);
    }
}

void InputManager::bindAction(Action action, std::function<void()> callback) {
    actionCallbacks[action] = callback;
    spdlog::debug("Action binding added");
}

bool InputManager::isActionPressed(Action action) {
    for (const auto& device : devices) {
        if (device->isConnected() && device->isActionPressed(action)) {
            return true;
        }
    }
    return false;
}

void InputManager::addDevice(std::unique_ptr<InputDevice> device) {
    spdlog::info("Adding input device: {}", device->getDeviceName());
    devices.push_back(std::move(device));
}

void InputManager::removeDevice(const std::string& deviceName) {
    auto it = std::find_if(devices.begin(), devices.end(),
        [&deviceName](const auto& device) {
            return device->getDeviceName() == deviceName;
        });

    if (it != devices.end()) {
        spdlog::info("Removing input device: {}", deviceName);
        devices.erase(it);
    }
}

const std::function<void()>& InputManager::getActionCallback(Action action) const {
    static const std::function<void()> emptyCallback;
    auto it = actionCallbacks.find(action);
    return (it != actionCallbacks.end()) ? it->second : emptyCallback;
}

} // namespace game
