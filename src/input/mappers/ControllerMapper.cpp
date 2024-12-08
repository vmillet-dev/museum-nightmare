#include "ControllerMapper.hpp"
#include <stdexcept>
#include <string>
#include <regex>
#include <unordered_map>

namespace game {

// Initialize static members
const Bimap<unsigned int, std::string> ControllerMapper::buttonMap = []() {
    Bimap<unsigned int, std::string> map;
    map.insert(0, "A");
    map.insert(1, "B");
    map.insert(2, "X");
    map.insert(3, "Y");
    map.insert(4, "LB");
    map.insert(5, "RB");
    map.insert(6, "Back");
    map.insert(7, "Start");
    map.insert(8, "LeftStick");
    map.insert(9, "RightStick");
    return map;
}();

const Bimap<unsigned int, std::string> ControllerMapper::axisMap = []() {
    Bimap<unsigned int, std::string> map;
    map.insert(sf::Joystick::X, "LeftStickX");
    map.insert(sf::Joystick::Y, "LeftStickY");
    map.insert(sf::Joystick::U, "RightStickX");
    map.insert(sf::Joystick::V, "RightStickY");
    map.insert(sf::Joystick::PovX, "DPadX");
    map.insert(sf::Joystick::PovY, "DPadY");
    map.insert(sf::Joystick::Z, "LeftTrigger");
    map.insert(sf::Joystick::R, "RightTrigger");
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
