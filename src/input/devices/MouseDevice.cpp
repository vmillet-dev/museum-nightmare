#include "MouseDevice.hpp"
#include <spdlog/spdlog.h>

namespace game {

void MouseDevice::init() {
    // No initialization needed for mouse device
}

void MouseDevice::update() {
    // Update mouse button states
    leftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    rightMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Right);
}

bool MouseDevice::isActionPressed(Action action) const {
    if (action == Action::Confirm) {
        return leftMousePressed;
    }
    if (action == Action::Cancel) {
        return rightMousePressed;
    }
    return false;
}

void MouseDevice::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            leftMousePressed = true;
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            rightMousePressed = true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            leftMousePressed = false;
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            rightMousePressed = false;
        }
    }
}

} // namespace game
