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
    explicit InputManager(sf::RenderWindow& window) : window(window) {}
    void init();
    void update();
    void handleEvent(const sf::Event& event);
    bool isActionPressed(Action action);
    sf::RenderWindow& getWindow() { return window; }

private:
    void createControllerDevice();
    void removeControllerDevice();
    std::vector<std::unique_ptr<InputDevice>> devices;
    sf::RenderWindow& window;
};

} // namespace game
