#pragma once
#include "IInputProvider.hpp"
#include <SFML/Window/Joystick.hpp>
#include <map>
#include <string>

namespace game {

enum class ControllerType {
    None,
    Xbox,
    PlayStation
};

class HumanInputProvider : public IInputProvider {
public:
    HumanInputProvider();

    sf::Vector2f getMovementInput() override;
    bool isActionTriggered(Action action) override;
    float getActionStrength(Action action) override;
    void update() override;
    void handleInputSourceChange() override;

private:
    ControllerType detectControllerType();
    void updateControllerState();
    float getAxisValue(sf::Joystick::Axis axis, unsigned int joystickId = 0);

    ControllerType currentControllerType;
    bool controllerConnected;
    unsigned int activeControllerId;
    float deadzone;

    std::map<Action, sf::Keyboard::Key> keyboardBindings;
    std::map<Action, unsigned int> controllerBindings;
};

} // namespace game
