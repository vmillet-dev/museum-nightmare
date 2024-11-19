#pragma once
#include <SFML/Window.hpp>
#include <functional>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include <memory>
#include "devices/IInputDevice.hpp"
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
    void bindAction(Action action, std::function<void()> callback);
    void handleInput(const sf::Event& event);
    bool isActionPressed(Action action) const;
    void setKeyState(sf::Keyboard::Key key, bool pressed); // New method

private:
    InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    void updateDevices();
    void tryConnectController();

    std::unordered_map<std::string, std::unique_ptr<IInputDevice>> devices;
    std::unordered_map<Action, std::function<void()>> actionCallbacks;
    std::string lastActiveDevice;
};

} // namespace game
