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

    sf::Keyboard::Key fromName(const std::string& keyName);
    std::string toName(sf::Keyboard::Key key);

private:
    KeyMapper() {}  // initializeMap will be called by getKeyMap
    KeyMapper(const KeyMapper&) = delete;
    KeyMapper& operator=(const KeyMapper&) = delete;

    static Bimap<sf::Keyboard::Key, std::string>& getKeyMap() {
        static std::once_flag initFlag;
        static Bimap<sf::Keyboard::Key, std::string> keyMap;
        std::call_once(initFlag, [&]() { getInstance().initializeMap(); });
        return keyMap;
    }

    void initializeMap();
};

} // namespace game
