#pragma once
#include "IInputDevice.hpp"
#include <unordered_map>

namespace game {

class KeyboardDevice : public IInputDevice {
public:
    KeyboardDevice();
    ~KeyboardDevice() override = default;

    // IInputDevice interface implementation
    bool isActionPressed(Action action) const override;
    void update() override;
    void handleEvent(const sf::Event& event) override;
    bool isConnected() const override { return true; }
    std::string getDeviceId() const override { return "keyboard"; }
    void loadConfig() override;

private:
    std::unordered_map<sf::Keyboard::Key, Action> keyBindings;
    std::unordered_map<sf::Keyboard::Key, bool> keyStates;
};

} // namespace game
