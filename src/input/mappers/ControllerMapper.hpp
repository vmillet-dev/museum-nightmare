#pragma once
#include <SFML/Window/Joystick.hpp>
#include <string>
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
    static ControllerMapper& getInstance() {
        static ControllerMapper instance;
        return instance;
    }

    ControllerMapper() { initializeMap(); }
    ControllerMapper(const ControllerMapper&) = delete;
    ControllerMapper& operator=(const ControllerMapper&) = delete;

    static Bimap<unsigned int, std::string>& getButtonMap() {
        static Bimap<unsigned int, std::string> buttonMap;
        return buttonMap;
    }

    static Bimap<unsigned int, std::string>& getAxisMap() {
        static Bimap<unsigned int, std::string> axisMap;
        return axisMap;
    }

    void initializeMap();
};

} // namespace game
