#include "ControllerMapper.hpp"
#include <stdexcept>
#include <string>
#include <regex>

namespace game {

ControllerMapper::ControllerMapper() {
    initializeMap();
}

void ControllerMapper::initializeMap() {

    // Initialize button mappings
    buttonMap.insert(0, "A");
    buttonMap.insert(1, "B");
    buttonMap.insert(2, "X");
    buttonMap.insert(3, "Y");
    buttonMap.insert(4, "LB");
    buttonMap.insert(5, "RB");
    buttonMap.insert(6, "Back");
    buttonMap.insert(7, "Start");
    buttonMap.insert(8, "LeftStick");
    buttonMap.insert(9, "RightStick");

    // Initialize axis mappings
    axisMap.insert(sf::Joystick::X, "LeftStickX");
    axisMap.insert(sf::Joystick::Y, "LeftStickY");
    axisMap.insert(sf::Joystick::U, "RightStickX");
    axisMap.insert(sf::Joystick::V, "RightStickY");
    axisMap.insert(sf::Joystick::PovX, "DPadX");
    axisMap.insert(sf::Joystick::PovY, "DPadY");
    axisMap.insert(sf::Joystick::Z, "LeftTrigger");
    axisMap.insert(sf::Joystick::R, "RightTrigger");
}

unsigned int ControllerMapper::stringToButtonId(const std::string& name) {
    try {
        return buttonMap.get_right(name);
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Unknown controller button: " + name);
    }
}

std::string ControllerMapper::stringToAxisKey(const std::string& name)
{
    unsigned int axisId = mapAxisId(name);
    return (isAxisPositive(name) ? "+" : "-") + std::to_string(axisId);
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
    return buttonMap.contains_right(name);
}

} // namespace game
