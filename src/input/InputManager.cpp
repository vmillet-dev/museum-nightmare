#include "InputManager.hpp"
#include "../config/ConfigManager.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>
#include <typeinfo>
#include "mappers/KeyMapper.hpp"
#include "mappers/MouseMapper.hpp"
#include "mappers/ControllerMapper.hpp"

namespace game {

InputManager::InputManager(sf::RenderWindow& window) : window(window) {
    spdlog::info("Initializing InputManager");

    // Initialize mappers before creating devices
    KeyMapper::getInstance();
    MouseMapper::getInstance();
    ControllerMapper::getInstance();

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

void InputManager::createControllerDevice() {
    auto controllerDevice = std::make_unique<ControllerDevice>();
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
            spdlog::debug("Action {} just pressed on {}", ActionUtil::toString(action), typeid(*device).name());
            return true;
        }
    }
    return false;
}

bool InputManager::isActionReleased(Action action) {
    for (auto& device : devices) {
        if (device->isActionReleased(action)) {
            spdlog::debug("Action {} released on {}", ActionUtil::toString(action), typeid(*device).name());
            return true;
        }
    }
    return false;
}

} // namespace game
