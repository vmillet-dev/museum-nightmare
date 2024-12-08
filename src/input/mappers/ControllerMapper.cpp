#include "ControllerMapper.hpp"
#include <stdexcept>
#include <string>
#include <regex>
#include <unordered_map>

namespace game {

// Initialize static members
const Bimap<std::string, unsigned int> ControllerMapper::buttonMap = []() {
    Bimap<std::string, unsigned int> map;
    map.insert("A", 0);
    map.insert("B", 1);
    map.insert("X", 2);
    map.insert("Y", 3);
    map.insert("LB", 4);
    map.insert("RB", 5);
    map.insert("Back", 6);
    map.insert("Start", 7);
    map.insert("LeftStick", 8);
    map.insert("RightStick", 9);
    return map;
}();

const Bimap<std::string, unsigned int> ControllerMapper::axisMap = []() {
    Bimap<std::string, unsigned int> map;
    map.insert("LeftStickX", sf::Joystick::X);
    map.insert("LeftStickY", sf::Joystick::Y);
    map.insert("RightStickX", sf::Joystick::U);
    map.insert("RightStickY", sf::Joystick::V);
    map.insert("DPadX", sf::Joystick::PovX);
    map.insert("DPadY", sf::Joystick::PovY);
    map.insert("LeftTrigger", sf::Joystick::Z);
    map.insert("RightTrigger", sf::Joystick::R);
    return map;
}();

unsigned int ControllerMapper::mapButtonName(const std::string& name) {
    try {
        return buttonMap.get_right(name);
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Unknown controller button: " + name);
    }
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

    try {
        return axisMap.get_right(result);
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Unknown controller axis: " + name);
    }
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
    return buttonMap.contains_left(name);
}

} // namespace game
