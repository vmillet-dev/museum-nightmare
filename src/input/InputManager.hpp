#pragma once
#include <SFML/Window.hpp>
#include <memory>
#include "devices/InputDevice.hpp"
#include "devices/KeyboardDevice.hpp"
#include "devices/ControllerDevice.hpp"
#include "Action.hpp"

namespace game {

class InputManager {
public:
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }

    void init();
    void update();
    void handleEvent(const sf::Event& event);
    bool isActionPressed(Action action);

private:
    InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unique_ptr<KeyboardDevice> keyboardDevice;
    std::unique_ptr<ControllerDevice> controllerDevice;
};

} // namespace game
