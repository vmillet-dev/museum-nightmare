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

    // Add methods to check controller status
    bool isConnected() const { return connected; }
    int getControllerId() const { return controllerId; }

private:
    std::unordered_map<unsigned int, Action> buttonBindings;
    std::unordered_map<unsigned int, ActionState> buttonStates;

    std::unordered_map<std::string, Action> axisBindings;
    std::unordered_map<std::string, ActionState> axisStates;

    bool connected = false;
    float deadzone = 20.0f;
    float sensitivity = 100.0f;
    int controllerId = 0;

    void setButtonBinding(unsigned int button, Action action);
    void setButtonState(unsigned int button, bool pressed);

    void setAxisBinding(std::string axis, Action action);
    void setAxisState(unsigned int axis, float position);

    void setDeadzone(float value) { deadzone = value; }
    void setSensitivity(float value) { sensitivity = value; }
};

} // namespace game
