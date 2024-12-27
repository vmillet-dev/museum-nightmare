#pragma once
#include <SFML/Window.hpp>
#include <string>
#include <mutex>
#include <optional>
#include "../../core/containers/bimap.hpp"

namespace game {

class MouseMapper {
public:
    MouseMapper();
    std::optional<sf::Mouse::Button> stringToButton(const std::string& buttonStr);
    std::string buttonToString(sf::Mouse::Button button);

private:
    MouseMapper(const MouseMapper&) = delete;
    MouseMapper& operator=(const MouseMapper&) = delete;

    Bimap<sf::Mouse::Button, std::string> buttonMap;

    void initializeButtonMap();
};

} // namespace game
