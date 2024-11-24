#pragma once
#include <SFML/Graphics.hpp>
#include "InputDevice.hpp"

namespace game {

class MouseDevice : public InputDevice {
public:
    explicit MouseDevice(sf::RenderWindow& window);

    void init() override {}  // No initialization needed for mouse
    void update() override;
    bool isActionPressed(Action action) override;
    bool isActionJustPressed(Action action) override;
    bool isActionReleased(Action action) override;
    void handleEvent(const sf::Event& event) override { /* No mouse events to handle */ }

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
