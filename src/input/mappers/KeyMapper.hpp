#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include <mutex>
#include "../../core/containers/bimap.hpp"

namespace game {

class KeyMapper {
public:
    static sf::Keyboard::Key fromName(const std::string& keyName);
    static std::string toName(sf::Keyboard::Key key);

private:
    KeyMapper() = default;
    KeyMapper(const KeyMapper&) = delete;
    KeyMapper& operator=(const KeyMapper&) = delete;

    static void ensureInitialized();
    static void initializeMap();
    static std::once_flag initFlag_;
    static Bimap<sf::Keyboard::Key, std::string> keyMap;
};

} // namespace game
