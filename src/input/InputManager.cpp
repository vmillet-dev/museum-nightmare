#include "InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>
#include <typeinfo>

namespace game {

InputManager::InputManager(sf::RenderWindow& window) : window(window) {
    spdlog::info("Initializing InputManager");

    // Add keyboard and mouse devices by default
    auto keyboardDevice = std::make_unique<KeyboardDevice>();
    devices.push_back(std::move(keyboardDevice));

    auto mouseDevice = std::make_unique<MouseDevice>(window);
    devices.push_back(std::move(mouseDevice));

    // Check for already connected controllers
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            createControllerDevice(i);
        }
    }

    spdlog::info("Input devices initialized");
}

void InputManager::update() {
    for (auto& device : devices) {
        device->update();
    }
}

void InputManager::createControllerDevice(unsigned int controllerId) {
    spdlog::info("Controller {} connected", controllerId);
    auto controllerDevice = std::make_unique<ControllerDevice>(controllerId);
    devices.push_back(std::move(controllerDevice));
    spdlog::info("Controller {} connected and initialized", controllerId);
}

void InputManager::removeControllerDevice(unsigned int controllerId) {
    devices.erase(
        std::remove_if(devices.begin(), devices.end(),
            [controllerId](const auto& device) {
                auto* controllerDevice = dynamic_cast<ControllerDevice*>(device.get());
                return controllerDevice && controllerDevice->getControllerId() == controllerId;
            }
        ),
        devices.end()
    );
    spdlog::info("Controller disconnected and removed");
}

void InputManager::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::JoystickConnected) {
        createControllerDevice(event.joystickConnect.joystickId);
    }
    else if (event.type == sf::Event::JoystickDisconnected) {
        removeControllerDevice(event.joystickConnect.joystickId);
    }

    // Forward events to all devices
    for (auto& device : devices) {
        device->handleEvent(event);
    }
}

bool InputManager::isActionPressed(Action action) const {
    for (const auto& device : devices) {
        if (device->isActionPressed(action)) {
            return true;
        }
    }
    return false;
}

bool InputManager::isActionJustPressed(Action action) const {
    for (const auto& device : devices) {
        if (device->isActionJustPressed(action)) {
            spdlog::debug("Action {} just pressed on {}", ActionUtil::toString(action), typeid(*device).name());
            return true;
        }
    }
    return false;
}

bool InputManager::isActionReleased(Action action) const {
    for (const auto& device : devices) {
        if (device->isActionReleased(action)) {
            spdlog::debug("Action {} released on {}", ActionUtil::toString(action), typeid(*device).name());
            return true;
        }
    }
    return false;
}

} // namespace game
