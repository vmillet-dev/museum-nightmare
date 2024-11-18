#pragma once
#include <SFML/Window.hpp>
#include <functional>
#include <unordered_map>
#include <spdlog/spdlog.h>

namespace game {

enum class Action {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Pause,
    Confirm,
    Cancel
};

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

private:
    InputManager() = default;
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;

    std::unordered_map<sf::Keyboard::Key, Action> keyBindings;
    std::unordered_map<Action, std::function<void()>> actionCallbacks;
};

} // namespace game
