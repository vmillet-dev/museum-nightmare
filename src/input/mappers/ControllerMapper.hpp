#pragma once
#include <SFML/Window/Joystick.hpp>
#include <string>
#include <unordered_map>

namespace game {

class ControllerMapper {
public:
    static unsigned int mapButtonName(const std::string& name);
    static unsigned int mapAxisId(const std::string& name);
    static bool isAxisPositive(const std::string& name);
    static bool isAxis(const std::string& name);
    static bool isButton(const std::string& name);

private:
    static const std::unordered_map<std::string, unsigned int> buttonMap;
    static const std::unordered_map<std::string, unsigned int> axisMap;
};

} // namespace game
