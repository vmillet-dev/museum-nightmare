#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <unordered_map>

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

    std::unordered_map<std::string, sf::Keyboard::Key> nameToKeyMap;
    std::unordered_map<sf::Keyboard::Key, std::string> keyToNameMap;
};

} // namespace game
