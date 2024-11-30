#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class KeyboardDevice : public InputDevice {
public:
    void init() override;
    void update() override;
    bool isActionPressed(Action action) override;
    bool isActionJustPressed(Action action) override;
    bool isActionReleased(Action action) override;
    void handleEvent(const sf::Event& event) override;

private:
    std::unordered_map<sf::Keyboard::Key, Action> keyBindings;
    std::unordered_map<sf::Keyboard::Key, ActionState> keyStates;

    void setKeyBinding(sf::Keyboard::Key key, Action action);
    void setKeyState(sf::Keyboard::Key key, bool pressed);
};

} // namespace game
