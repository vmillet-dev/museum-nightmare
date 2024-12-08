#include "MouseMapper.hpp"

namespace game {

sf::Mouse::Button MouseMapper::stringToButton(const std::string& buttonStr) const {
    try {
        return buttonMap.get_right(buttonStr);
    } catch (const std::out_of_range&) {
        return sf::Mouse::ButtonCount;  // Return an invalid button if not found
    }
}

std::string MouseMapper::buttonToString(sf::Mouse::Button button) const {
    try {
        return buttonMap.get_left(button);
    } catch (const std::out_of_range&) {
        return "unknown";  // Return "unknown" if button is not found
    }
}

MouseMapper::MouseMapper() {
    // Initialize the mapping between buttons and strings
    buttonMap.insert(sf::Mouse::Left, "LeftButton");
    buttonMap.insert(sf::Mouse::Right, "RightButton");
    buttonMap.insert(sf::Mouse::Middle, "MiddleButton");
    buttonMap.insert(sf::Mouse::XButton1, "x1");
    buttonMap.insert(sf::Mouse::XButton2, "x2");
}

}
