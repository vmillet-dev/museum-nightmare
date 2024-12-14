#pragma once
#include "InputDevice.hpp"
#include "GenericInputDevice.hpp"
#include <unordered_map>

namespace game {

class ControllerDevice : public GenericInputDevice<unsigned int> {
public:
    explicit ControllerDevice(unsigned int controllerId);
    void update() override;
    bool isActionPressed(Action action) override;
    bool isActionJustPressed(Action action) override;
    bool isActionReleased(Action action) override;
    void handleEvent(const sf::Event& event) override;
    void loadBinding() override;

    unsigned int getControllerId() const { return controllerId; }

private:
    std::unordered_map<std::string, Action> axisBindings;
    std::unordered_map<std::string, ActionState> axisStates;

    float deadzone = 20.0f;
    float sensitivity = 100.0f;
    unsigned int controllerId = 0;

    void setAxisBinding(std::string axis, Action action);
    void setAxisState(unsigned int axis, float position);
};

} // namespace game
