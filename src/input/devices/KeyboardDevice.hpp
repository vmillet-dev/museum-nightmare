#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class KeyboardDevice : public InputDevice {
public:
    void init() override;
    InputDevice::ActionState getActionState(Action action) const override;
    void handleEvent(const sf::Event& event) override;
    void setKeyBinding(Action action, sf::Keyboard::Key key);
    void setKeyState(sf::Keyboard::Key key, bool pressed);

private:
    std::unordered_map<sf::Keyboard::Key, Action> keyBindings;
    std::unordered_map<Action, InputDevice::ActionState> actionStates;
};

} // namespace game
