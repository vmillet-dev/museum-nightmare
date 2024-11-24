#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class ControllerDevice : public InputDevice {
public:
    void init() override;
    void update() override;
    bool isActionPressed(Action action) override;
    bool isActionJustPressed(Action action) override;
    bool isActionReleased(Action action) override;
    void handleEvent(const sf::Event& event) override;

    void setButtonBinding(Action action, unsigned int button);
    void setAxisBinding(Action action, sf::Joystick::Axis axis);
    void setDeadzone(float value) { deadzone = value; }
    void setSensitivity(float value) { sensitivity = value; }

    // Add methods to check controller status
    bool isConnected() const { return connected; }
    int getControllerId() const { return controllerId; }

private:
    std::unordered_map<Action, unsigned int> buttonBindings;
    std::unordered_map<Action, sf::Joystick::Axis> axisBindings;
    std::unordered_map<Action, bool> buttonStates;
    std::unordered_map<Action, bool> previousButtonStates;  // For just pressed detection
    std::unordered_map<Action, float> axisStates;
    std::unordered_map<Action, bool> previousAxisStates;  // For just pressed detection on axes
    bool connected = false;
    float deadzone = 20.0f;
    float sensitivity = 100.0f;
    int controllerId = 0;
};

} // namespace game
