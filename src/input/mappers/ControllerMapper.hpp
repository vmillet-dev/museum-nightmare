#pragma once
#include <SFML/Window/Joystick.hpp>
#include <string>
#include <mutex>
#include "../../core/containers/bimap.hpp"

namespace game {

class ControllerMapper {
public:
    static unsigned int mapButtonName(const std::string& name);
    static unsigned int mapAxisId(const std::string& name);
    static bool isAxisPositive(const std::string& name);
    static bool isAxis(const std::string& name);
    static bool isButton(const std::string& name);

private:
    ControllerMapper() = default;
    ControllerMapper(const ControllerMapper&) = delete;
    ControllerMapper& operator=(const ControllerMapper&) = delete;

    static void ensureInitialized();
    static void initializeMap();
    static std::once_flag initFlag_;
    static Bimap<unsigned int, std::string> buttonMap;
    static Bimap<unsigned int, std::string> axisMap;
};

} // namespace game
