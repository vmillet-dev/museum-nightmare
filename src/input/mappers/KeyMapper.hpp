#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include "../../core/containers/bimap.hpp"

namespace game {

class KeyMapper {
public:
    static sf::Keyboard::Key fromName(const std::string& keyName);
    static std::string toName(sf::Keyboard::Key key);

private:
    static KeyMapper& getInstance() {
        static KeyMapper instance;
        return instance;
    }

    KeyMapper() { initializeMap(); }
    KeyMapper(const KeyMapper&) = delete;
    KeyMapper& operator=(const KeyMapper&) = delete;

    static Bimap<sf::Keyboard::Key, std::string>& getKeyMap() {
        static Bimap<sf::Keyboard::Key, std::string> keyMap;
        return keyMap;
    }

    void initializeMap();
};

} // namespace game
