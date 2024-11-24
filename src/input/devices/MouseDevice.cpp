#include "MouseDevice.hpp"

namespace game {

MouseDevice::MouseDevice(sf::RenderWindow& window)
    : window(window)
    , leftButtonPressed(false)
    , rightButtonPressed(false)
    , leftButtonJustPressed(false)
    , rightButtonJustPressed(false)
    , lastMousePos(sf::Mouse::getPosition(window))
{
}

void MouseDevice::update() {
    // Store previous state
    bool wasLeftPressed = leftButtonPressed;
    bool wasRightPressed = rightButtonPressed;

    // Update current state
    leftButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    rightButtonPressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    // Update "just pressed" states
    leftButtonJustPressed = leftButtonPressed && !wasLeftPressed;
    rightButtonJustPressed = rightButtonPressed && !wasRightPressed;

    // Update mouse position
    lastMousePos = sf::Mouse::getPosition(window);
}

bool MouseDevice::isActionPressed(Action action) {
    switch (action) {
        case Action::MouseLeft:
            return leftButtonPressed;
        case Action::MouseRight:
            return rightButtonPressed;
        default:
            return false;
    }
}

bool MouseDevice::isActionJustPressed(Action action) {
    switch (action) {
        case Action::MouseLeft:
            return leftButtonJustPressed;
        case Action::MouseRight:
            return rightButtonJustPressed;
        default:
            return false;
    }
}

bool MouseDevice::isActionReleased(Action action) {
    switch (action) {
        case Action::MouseLeft:
            return !leftButtonPressed;
        case Action::MouseRight:
            return !rightButtonPressed;
        default:
            return false;
    }
}

sf::Vector2i MouseDevice::getMousePosition() const {
    return lastMousePos;
}

bool MouseDevice::isLeftButtonPressed() const {
    return leftButtonPressed;
}

bool MouseDevice::isRightButtonPressed() const {
    return rightButtonPressed;
}

} // namespace game
