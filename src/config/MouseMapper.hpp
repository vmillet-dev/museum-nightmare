#pragma once
#include <SFML/Window.hpp>
#include <unordered_map>
#include <string>
#include <iostream>

namespace game {

class MouseMapper {
public:

    static MouseMapper& getInstance() {
        static MouseMapper instance;
        return instance;
    }

    // Convert a string to the corresponding sf::Mouse::Button
    sf::Mouse::Button stringToButton(const std::string& buttonStr) const {
        auto it = stringToButtonMap.find(buttonStr);
        if (it != stringToButtonMap.end()) {
            return it->second;
        }
        else {
            std::cerr << "Error: Button \"" << buttonStr << "\" not found!" << std::endl;
            return sf::Mouse::ButtonCount;  // Return an invalid button if not found
        }
    }

    // Convert a sf::Mouse::Button to the corresponding string
    std::string buttonToString(sf::Mouse::Button button) const {
        auto it = buttonToStringMap.find(button);
        if (it != buttonToStringMap.end()) {
            return it->second;
        }
        else {
            return "unknown";  // Return "unknown" if button is not found
        }
    }

private:
    std::unordered_map<sf::Mouse::Button, std::string> buttonToStringMap;
    std::unordered_map<std::string, sf::Mouse::Button> stringToButtonMap;

    MouseMapper() {
        // Initialize the mapping between strings and sf::Mouse::Button values
        buttonToStringMap = {
            {sf::Mouse::Left, "left"},
            {sf::Mouse::Right, "right"},
            {sf::Mouse::Middle, "middle"},
            {sf::Mouse::XButton1, "x1"},
            {sf::Mouse::XButton2, "x2"}
        };

        // Build reverse mapping
        for (const auto& [button, name] : buttonToStringMap) {
            stringToButtonMap[name] = button;
        }
    }
};

}
