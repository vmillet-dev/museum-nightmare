#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include "../../core/containers/bimap.hpp"

namespace game {

class KeyMapper {
public:
    static KeyMapper& getInstance() {
        static KeyMapper instance;
        return instance;
    }

    // Convert key name to SFML key code
    sf::Keyboard::Key fromName(const std::string& keyName);

    // Convert SFML key code to key name
    std::string toName(sf::Keyboard::Key key);

private:
    KeyMapper();
    KeyMapper(const KeyMapper&) = delete;
    KeyMapper& operator=(const KeyMapper&) = delete;

    Bimap<std::string, sf::Keyboard::Key> keyMap;
};

} // namespace game
