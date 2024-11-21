#pragma once
#include <SFML/Window.hpp>
#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>
#include <spdlog/spdlog.h>
#include "Actions.hpp"
#include "devices/InputDevice.hpp"
#include "devices/KeyboardDevice.hpp"
#include "devices/ControllerDevice.hpp"

namespace game {

class InputManager {
public:
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }

    void init();
    void update();
    void bindAction(Action action, std::function<void()> callback);
    void handleInput(const sf::Event& event);
    bool isActionPressed(Action action);

    // New methods for device management
    void addDevice(std::unique_ptr<InputDevice> device);
    void removeDevice(const std::string& deviceName);
    const std::function<void()>& getActionCallback(Action action) const;

private:
    InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::vector<std::unique_ptr<InputDevice>> devices;
    std::unordered_map<Action, std::function<void()>> actionCallbacks;
};

} // namespace game
