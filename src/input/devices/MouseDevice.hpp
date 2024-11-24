#pragma once
#include <SFML/Graphics.hpp>
#include "InputDevice.hpp"
#include <unordered_map>

namespace game {

class MouseDevice : public InputDevice {
public:
    explicit MouseDevice(sf::RenderWindow& window);

    void init() override {}  // No initialization needed for mouse
    InputDevice::ActionState getActionState(Action action) const override;
    void handleEvent(const sf::Event& event) override;

    sf::Vector2i getMousePosition() const;
    bool isLeftButtonPressed() const;
    bool isRightButtonPressed() const;

private:
    sf::RenderWindow& window;
    std::unordered_map<Action, InputDevice::ActionState> actionStates;
    sf::Vector2i lastMousePos;
};

} // namespace game
