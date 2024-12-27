#include "MouseMapper.hpp"

namespace game {

MouseMapper::MouseMapper() {
    initializeButtonMap();
}

void MouseMapper::initializeButtonMap() {
    buttonMap.insert(sf::Mouse::Button::Left, "LeftButton");
    buttonMap.insert(sf::Mouse::Button::Right, "RightButton");
    buttonMap.insert(sf::Mouse::Button::Middle, "MiddleButton");
    buttonMap.insert(sf::Mouse::Button::Extra1, "x1");
    buttonMap.insert(sf::Mouse::Button::Extra2, "x2");
}

std::optional<sf::Mouse::Button> MouseMapper::stringToButton(const std::string& buttonStr) {
    try {
        return buttonMap.get_right(buttonStr);
    } catch (const std::out_of_range&) {
        return std::nullopt;  // Return nullopt for invalid buttons
    }
}

std::string MouseMapper::buttonToString(sf::Mouse::Button button) {
    try {
        return buttonMap.get_left(button);
    } catch (const std::out_of_range&) {
        return "unknown";  // Return "unknown" if button is not found
    }
}

} // namespace game
