#include <SFML/Window.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include "../mappers/ControllerMapper.hpp"

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
    std::unordered_map<sf::Mouse::Button, std::string> buttonToStringMap;
    std::unordered_map<std::string, sf::Mouse::Button> stringToButtonMap;

    MouseMapper();
};

}