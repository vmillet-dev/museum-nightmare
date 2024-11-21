#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class KeyboardDevice : public InputDevice {
public:
    KeyboardDevice();
    ~KeyboardDevice() override = default;

    void update() override;
    bool isActionPressed(Action action) override;
    void handleEvent(const sf::Event& event) override;
    bool isConnected() const override { return true; }  // Keyboard is always connected
    std::string getDeviceName() const override { return "Keyboard"; }

    void bindKey(sf::Keyboard::Key key, Action action);
    void setKeyState(sf::Keyboard::Key key, bool pressed);

private:
    std::unordered_map<sf::Keyboard::Key, Action> keyBindings;
    std::unordered_map<sf::Keyboard::Key, bool> keyStates;
    sf::Keyboard::Key findKeyForAction(Action action) const;
};

} // namespace game
