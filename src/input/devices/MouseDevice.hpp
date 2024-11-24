#pragma once
#include <SFML/Graphics.hpp>
#include "InputDevice.hpp"

namespace game {

class MouseDevice : public InputDevice {
public:
    explicit MouseDevice(sf::RenderWindow& window);

    void update() override;
    bool isActionPressed(Action action) const override;
    bool isActionJustPressed(Action action) const override;
    bool isActionReleased(Action action) const override;

    sf::Vector2i getMousePosition() const;
    bool isLeftButtonPressed() const;
    bool isRightButtonPressed() const;

private:
    sf::RenderWindow& window;
    bool leftButtonPressed;
    bool rightButtonPressed;
    bool leftButtonJustPressed;
    bool rightButtonJustPressed;
    sf::Vector2i lastMousePos;
};

} // namespace game
