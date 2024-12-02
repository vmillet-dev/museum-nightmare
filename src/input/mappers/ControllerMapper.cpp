#include "ControllerMapper.hpp"
#include <stdexcept>

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
    {"RightStick", 9},
    {"LT", 6},    // Left Trigger as button
    {"RT", 7}     // Right Trigger as button
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

unsigned int ControllerMapper::mapAxisId(const std::string& name) {
    std::string baseName = name;
    if (name.find("Up") != std::string::npos || name.find("Down") != std::string::npos) {
        baseName = name.substr(0, name.find("Up"));
        if (baseName.empty()) {
            baseName = name.substr(0, name.find("Down"));
        }
        baseName += "Y";
    } else if (name.find("Left") != std::string::npos || name.find("Right") != std::string::npos) {
        baseName = name.substr(0, name.find("Left"));
        if (baseName.empty()) {
            baseName = name.substr(0, name.find("Right"));
        }
        baseName += "X";
    }

    auto it = axisMap.find(baseName);
    if (it != axisMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown controller axis: " + name);
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
