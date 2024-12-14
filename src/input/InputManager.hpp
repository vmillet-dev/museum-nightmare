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
    explicit InputManager(sf::RenderWindow& window);
    void update();
    void handleEvent(const sf::Event& event);
    bool isActionPressed(Action action);
    bool isActionJustPressed(Action action);
    bool isActionReleased(Action action);
    sf::RenderWindow& getWindow() { return window; }

private:
    void createControllerDevice(unsigned int controllerId);
    void removeControllerDevice(unsigned int controllerId);
    std::vector<std::unique_ptr<InputDevice>> devices;
    sf::RenderWindow& window;
};

} // namespace game
