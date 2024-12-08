#pragma once
#include <SFML/Window/Joystick.hpp>
#include <string>
#include "../../../core/containers/bimap.hpp"

namespace game {

class ControllerMapper {
public:
    static unsigned int mapButtonName(const std::string& name);
    static unsigned int mapAxisId(const std::string& name);
    static bool isAxisPositive(const std::string& name);
    static bool isAxis(const std::string& name);
    static bool isButton(const std::string& name);

private:
    static const Bimap<std::string, unsigned int> buttonMap;
    static const Bimap<std::string, unsigned int> axisMap;
};

} // namespace game
