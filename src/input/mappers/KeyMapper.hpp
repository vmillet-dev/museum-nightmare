#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <mutex>
#include "../../core/containers/bimap.hpp"

namespace game {

class KeyMapper {
public:
    static KeyMapper& getInstance() {
        static KeyMapper instance;
        return instance;
    }

    sf::Keyboard::Key stringToKey(const std::string& keyName);
    std::string keyToString(sf::Keyboard::Key key);

private:
    KeyMapper();
    KeyMapper(const KeyMapper&) = delete;
    KeyMapper& operator=(const KeyMapper&) = delete;

    Bimap<sf::Keyboard::Key, std::string> keyMap;

    void initializeKeyMap();
};

} // namespace game
