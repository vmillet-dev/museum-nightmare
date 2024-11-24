#pragma once
#include <SFML/Window.hpp>
#include <memory>
#include <vector>
#include "devices/InputDevice.hpp"
#include "devices/KeyboardDevice.hpp"
#include "devices/ControllerDevice.hpp"
#include "devices/MouseDevice.hpp"
#include "Action.hpp"

namespace game {

class InputManager {
public:
    using ActionState = InputDevice::ActionState;  // Add this line

    explicit InputManager(sf::RenderWindow& window) : window(window) {}
    void init();
    void update();
    void handleEvent(const sf::Event& event);
    ActionState getActionState(Action action) const;
    sf::RenderWindow& getWindow() { return window; }

private:
    void createControllerDevice();
    void removeControllerDevice();
    std::vector<std::unique_ptr<InputDevice>> devices;
    sf::RenderWindow& window;
};

} // namespace game
