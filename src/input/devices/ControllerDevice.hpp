#pragma once
#include "IInputDevice.hpp"
#include <unordered_map>

namespace game {

class ControllerDevice : public IInputDevice {
public:
    explicit ControllerDevice(unsigned int id);
    ~ControllerDevice() override = default;

    // IInputDevice interface implementation
    bool isActionPressed(Action action) const override;
    void update() override;
    void handleEvent(const sf::Event& event) override;
    bool isConnected() const override;
    std::string getDeviceId() const override;
    void loadConfig() override;

private:
    unsigned int controllerId;
    std::unordered_map<Action, unsigned int> buttonBindings;
    std::unordered_map<Action, sf::Joystick::Axis> axisBindings;
    float deadzone;
    float sensitivity;
    bool checkAxisInput(Action action) const;
};

} // namespace game
