#pragma once
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class ControllerDevice : public InputDevice {
public:
    ControllerDevice(unsigned int controllerIndex = 0);
    ~ControllerDevice() override = default;

    void update() override;
    bool isActionPressed(Action action) override;
    void handleEvent(const sf::Event& event) override;
    bool isConnected() const override;
    std::string getDeviceName() const override;

    void bindButton(unsigned int button, Action action);
    void bindAxis(sf::Joystick::Axis axis, Action positiveAction, Action negativeAction);

    void setDeadzone(float value) { deadzone = value; }
    void setSensitivity(float value) { sensitivity = value; }

private:
    unsigned int controllerIndex;
    float deadzone;
    float sensitivity;
    bool connected;

    std::unordered_map<unsigned int, Action> buttonBindings;
    struct AxisBinding {
        Action positiveAction;
        Action negativeAction;
    };
    std::unordered_map<sf::Joystick::Axis, AxisBinding> axisBindings;

    void checkConnection();
    float getAxisValue(sf::Joystick::Axis axis) const;
};

} // namespace game
