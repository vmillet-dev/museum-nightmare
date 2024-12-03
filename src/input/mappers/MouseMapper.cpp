#include "MouseMapper.hpp"

namespace game {

sf::Mouse::Button MouseMapper::stringToButton(const std::string& buttonStr) const {
    auto it = stringToButtonMap.find(buttonStr);
    if (it != stringToButtonMap.end()) {
        return it->second;
    }
    else {
        std::cerr << "Error: Button \"" << buttonStr << "\" not found!" << std::endl;
        return sf::Mouse::ButtonCount;  // Return an invalid button if not found
    }
}

std::string MouseMapper::buttonToString(sf::Mouse::Button button) const {
    auto it = buttonToStringMap.find(button);
    if (it != buttonToStringMap.end()) {
        return it->second;
    }
    else {
        return "unknown";  // Return "unknown" if button is not found
    }
}

MouseMapper::MouseMapper() {
    // Initialize the mapping between strings and sf::Mouse::Button values
    buttonToStringMap = {
        {sf::Mouse::Left, "LeftButton"},
        {sf::Mouse::Right, "RightButton"},
        {sf::Mouse::Middle, "MiddleButton"},
        {sf::Mouse::XButton1, "x1"},
        {sf::Mouse::XButton2, "x2"}
    };

    // Build reverse mapping
    for (const auto& [name, key] : buttonToStringMap) {
        stringToButtonMap[key] = name;
    }
}

}