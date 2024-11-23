#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class ControllerDevice : public InputDevice {
public:
    void init() override;
    void update() override;
    bool isActionPressed(Action action) override;
    void handleEvent(const sf::Event& event) override;

    void setButtonBinding(Action action, unsigned int button);
    void setAxisBinding(Action action, sf::Joystick::Axis axis);
    void setDeadzone(float value) { deadzone = value; }
    void setSensitivity(float value) { sensitivity = value; }

    // Add methods to check controller status
    bool isConnected() const { return connected; }
    int getControllerId() const { return controllerId; }

private:
    std::unordered_map<unsigned int, Action> buttonBindings;
    std::unordered_map<sf::Joystick::Axis, Action> axisBindings;
    std::unordered_map<unsigned int, bool> buttonStates;
    std::unordered_map<sf::Joystick::Axis, float> axisStates;
    bool connected = false;
    float deadzone = 20.0f;
    float sensitivity = 100.0f;
    int controllerId = 0;
};

} // namespace game
