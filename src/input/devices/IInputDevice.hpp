#pragma once
#include <SFML/Window.hpp>
#include <string>
#include "../Action.hpp"

namespace game {

class IInputDevice {
public:
    virtual ~IInputDevice() = default;

    // Core input functionality
    virtual bool isActionPressed(Action action) const = 0;
    virtual void update() = 0;
    virtual void handleEvent(const sf::Event& event) = 0;

    // Device management
    virtual bool isConnected() const = 0;
    virtual std::string getDeviceId() const = 0;

    // Configuration
    virtual void loadConfig() = 0;
};

} // namespace game
