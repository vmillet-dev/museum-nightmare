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

unsigned int ControllerMapper::mapAxisId(const std::string& name) {
    std::string result;

    static const std::unordered_map<std::string, std::string> suffixMap = {
        {"Up", "Y"},
        {"Down", "Y"},
        {"Left", "X"},
        {"Right", "X"}
    };

    for (const auto& [suffix, replacement] : suffixMap) {
        if (name.size() >= suffix.size() && name.compare(name.size() - suffix.size(), suffix.size(), suffix) == 0) {
            result = name.substr(0, name.size() - suffix.size()) + replacement;
            break;
        }
    }

    auto it = axisMap.find(result);
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
