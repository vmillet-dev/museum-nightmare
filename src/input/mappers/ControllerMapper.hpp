#pragma once
#include <SFML/Window/Joystick.hpp>
#include <string>
#include <mutex>
#include "../../core/containers/bimap.hpp"

namespace game {

class ControllerMapper {
public:
    static ControllerMapper& getInstance() {
        static ControllerMapper instance;
        return instance;
    }

    unsigned int mapButtonName(const std::string& name);
    unsigned int mapAxisId(const std::string& name);
    bool isAxisPositive(const std::string& name);
    bool isAxis(const std::string& name);
    bool isButton(const std::string& name);

private:
    ControllerMapper();
    ControllerMapper(const ControllerMapper&) = delete;
    ControllerMapper& operator=(const ControllerMapper&) = delete;

    Bimap<unsigned int, std::string> buttonMap;
    Bimap<unsigned int, std::string> axisMap;

    void initializeMap();
};

} // namespace game
