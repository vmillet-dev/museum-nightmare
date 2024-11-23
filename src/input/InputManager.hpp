#pragma once
#include <SFML/Window.hpp>
#include <memory>
#include <vector>
#include "devices/InputDevice.hpp"
#include "devices/KeyboardDevice.hpp"
#include "devices/ControllerDevice.hpp"
#include "Action.hpp"

namespace game {

class InputManager {
public:
    InputManager() = default;
    void init();
    void update();
    void handleEvent(const sf::Event& event);
    bool isActionPressed(Action action) const;

private:
    void createControllerDevice();
    void removeControllerDevice();
    std::vector<std::unique_ptr<InputDevice>> devices;
};

} // namespace game
