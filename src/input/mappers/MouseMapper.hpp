#pragma once
#include <SFML/Window.hpp>
#include <string>
#include "../../../core/containers/bimap.hpp"

namespace game {

class MouseMapper {
public:

    static MouseMapper& getInstance() {
        static MouseMapper instance;
        return instance;
    }

    sf::Mouse::Button stringToButton(const std::string& buttonStr) const;
    std::string buttonToString(sf::Mouse::Button button) const;

private:
    MouseMapper();
    Bimap<sf::Mouse::Button, std::string> buttonMap;
};

}
