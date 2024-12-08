#pragma once
#include <SFML/Window.hpp>
#include <string>
#include <mutex>
#include "../../core/containers/bimap.hpp"

namespace game {

class MouseMapper {
public:
    static sf::Mouse::Button stringToButton(const std::string& buttonStr);
    static std::string buttonToString(sf::Mouse::Button button);

private:
    MouseMapper() = default;
    MouseMapper(const MouseMapper&) = delete;
    MouseMapper& operator=(const MouseMapper&) = delete;

    static void ensureInitialized();
    static void initializeMap();
    static bool initialized;
    static std::mutex initMutex;
    static Bimap<sf::Mouse::Button, std::string> buttonMap;
};

}
