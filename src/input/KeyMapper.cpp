#include "KeyMapper.hpp"

namespace game {

KeyMapper::KeyMapper() {
    // Initialize the key mappings
    nameToKeyMap = {
        {"A", sf::Keyboard::A}, {"B", sf::Keyboard::B}, {"C", sf::Keyboard::C},
        {"D", sf::Keyboard::D}, {"E", sf::Keyboard::E}, {"F", sf::Keyboard::F},
        {"G", sf::Keyboard::G}, {"H", sf::Keyboard::H}, {"I", sf::Keyboard::I},
        {"J", sf::Keyboard::J}, {"K", sf::Keyboard::K}, {"L", sf::Keyboard::L},
        {"M", sf::Keyboard::M}, {"N", sf::Keyboard::N}, {"O", sf::Keyboard::O},
        {"P", sf::Keyboard::P}, {"Q", sf::Keyboard::Q}, {"R", sf::Keyboard::R},
        {"S", sf::Keyboard::S}, {"T", sf::Keyboard::T}, {"U", sf::Keyboard::U},
        {"V", sf::Keyboard::V}, {"W", sf::Keyboard::W}, {"X", sf::Keyboard::X},
        {"Y", sf::Keyboard::Y}, {"Z", sf::Keyboard::Z},
        {"Num0", sf::Keyboard::Num0}, {"Num1", sf::Keyboard::Num1},
        {"Num2", sf::Keyboard::Num2}, {"Num3", sf::Keyboard::Num3},
        {"Num4", sf::Keyboard::Num4}, {"Num5", sf::Keyboard::Num5},
        {"Num6", sf::Keyboard::Num6}, {"Num7", sf::Keyboard::Num7},
        {"Num8", sf::Keyboard::Num8}, {"Num9", sf::Keyboard::Num9},
        {"Escape", sf::Keyboard::Escape}, {"Space", sf::Keyboard::Space},
        {"Return", sf::Keyboard::Return}, {"BackSpace", sf::Keyboard::BackSpace},
        {"Tab", sf::Keyboard::Tab}, {"Left", sf::Keyboard::Left},
        {"Right", sf::Keyboard::Right}, {"Up", sf::Keyboard::Up},
        {"Down", sf::Keyboard::Down}, {"LShift", sf::Keyboard::LShift},
        {"RShift", sf::Keyboard::RShift}, {"LControl", sf::Keyboard::LControl},
        {"RControl", sf::Keyboard::RControl}, {"LAlt", sf::Keyboard::LAlt},
        {"RAlt", sf::Keyboard::RAlt}
    };

    // Build reverse mapping
    for (const auto& [name, key] : nameToKeyMap) {
        keyToNameMap[key] = name;
    }
}

sf::Keyboard::Key KeyMapper::fromName(const std::string& keyName) {
    auto it = nameToKeyMap.find(keyName);
    return (it != nameToKeyMap.end()) ? it->second : sf::Keyboard::Unknown;
}

std::string KeyMapper::toName(sf::Keyboard::Key key) {
    auto it = keyToNameMap.find(key);
    return (it != keyToNameMap.end()) ? it->second : "Unknown";
}

} // namespace game
