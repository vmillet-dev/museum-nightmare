#include "ControllerMapper.hpp"
#include <stdexcept>
#include <string>
#include <regex>

namespace game {

const std::unordered_map<std::string, unsigned int> ControllerMapper::buttonMap = {
    {"A", 0},
    {"B", 1},
    {"X", 2},
    {"Y", 3},
    {"LB", 4},
    {"RB", 5},
    {"Back", 6},
    {"Start", 7},
    {"LeftStick", 8},
    {"RightStick", 9}
};

const std::unordered_map<std::string, unsigned int> ControllerMapper::axisMap = {
    {"LeftStickX", sf::Joystick::X},
    {"LeftStickY", sf::Joystick::Y},
    {"RightStickX", sf::Joystick::U},
    {"RightStickY", sf::Joystick::V},
    {"DPadX", sf::Joystick::PovX},
    {"DPadY", sf::Joystick::PovY},
    {"LeftTrigger", sf::Joystick::Z},
    {"RightTrigger", sf::Joystick::R}
};

unsigned int ControllerMapper::mapButtonName(const std::string& name) {
    auto it = buttonMap.find(name);
    if (it != buttonMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown controller button: " + name);
}

unsigned int ControllerMapper::mapAxisId(const std::string& input) {
    std::string result = input;

    // Check and replace the suffixes directly
    if (input.size() > 2 && input.compare(input.size() - 2, 2, "Up") == 0) {
        result.replace(input.size() - 2, 2, "Y");
    }
    else if (input.size() > 4 && input.compare(input.size() - 4, 4, "Down") == 0) {
        result.replace(input.size() - 4, 4, "Y");
    }
    else if (input.size() > 4 && input.compare(input.size() - 4, 4, "Left") == 0) {
        result.replace(input.size() - 4, 4, "X");
    }
    else if (input.size() > 5 && input.compare(input.size() - 5, 5, "Right") == 0) {
        result.replace(input.size() - 5, 5, "X");
    }

    auto it = axisMap.find(result);
    if (it != axisMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown controller axis: " + input);
}

bool ControllerMapper::isAxisPositive(const std::string& name) {
    return name.find("Down") != std::string::npos ||
           name.find("Right") != std::string::npos;
}

bool ControllerMapper::isAxis(const std::string& name) {
    return name.find("Stick") != std::string::npos ||
           name.find("DPad") != std::string::npos ||
           name.find("Trigger") != std::string::npos;
}

bool ControllerMapper::isButton(const std::string& name) {
    return buttonMap.find(name) != buttonMap.end();
}

} // namespace game
