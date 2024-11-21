#pragma once
#include <SFML/Window.hpp>
#include "../InputManager.hpp"

namespace game {

class InputDevice {
public:
    virtual ~InputDevice() = default;

    // Core interface
    virtual void update() = 0;
    virtual bool isActionPressed(Action action) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;

    // Device status
    virtual bool isConnected() const = 0;
    virtual std::string getDeviceName() const = 0;

protected:
    // Utility methods for derived classes
    virtual void notifyActionTriggered(Action action) {
        if (auto callback = InputManager::getInstance().getActionCallback(action)) {
            callback();
        }
    }
};

} // namespace game
