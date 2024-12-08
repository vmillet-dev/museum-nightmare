#pragma once
#include <SFML/Window.hpp>
#include <string>
#include <mutex>
#include "../../core/containers/bimap.hpp"

namespace game {

class MouseMapper {
public:
    static MouseMapper& getInstance() {
        static MouseMapper instance;
        return instance;
    }

    sf::Mouse::Button stringToButton(const std::string& buttonStr);
    std::string buttonToString(sf::Mouse::Button button);

private:
    MouseMapper() { initializeMap(); }
    MouseMapper(const MouseMapper&) = delete;
    MouseMapper& operator=(const MouseMapper&) = delete;

    static Bimap<sf::Mouse::Button, std::string>& getButtonMap() {
        static Bimap<sf::Mouse::Button, std::string> buttonMap;
        return buttonMap;
    }

    void initializeMap();
};

} // namespace game
