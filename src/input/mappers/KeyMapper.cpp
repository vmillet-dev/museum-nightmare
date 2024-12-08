#include "KeyMapper.hpp"

namespace game {

KeyMapper::KeyMapper() {
    // Initialize the key mappings
    keyMap.insert(sf::Keyboard::A, "A"); keyMap.insert(sf::Keyboard::B, "B");
    keyMap.insert(sf::Keyboard::C, "C"); keyMap.insert(sf::Keyboard::D, "D");
    keyMap.insert(sf::Keyboard::E, "E"); keyMap.insert(sf::Keyboard::F, "F");
    keyMap.insert(sf::Keyboard::G, "G"); keyMap.insert(sf::Keyboard::H, "H");
    keyMap.insert(sf::Keyboard::I, "I"); keyMap.insert(sf::Keyboard::J, "J");
    keyMap.insert(sf::Keyboard::K, "K"); keyMap.insert(sf::Keyboard::L, "L");
    keyMap.insert(sf::Keyboard::M, "M"); keyMap.insert(sf::Keyboard::N, "N");
    keyMap.insert(sf::Keyboard::O, "O"); keyMap.insert(sf::Keyboard::P, "P");
    keyMap.insert(sf::Keyboard::Q, "Q"); keyMap.insert(sf::Keyboard::R, "R");
    keyMap.insert(sf::Keyboard::S, "S"); keyMap.insert(sf::Keyboard::T, "T");
    keyMap.insert(sf::Keyboard::U, "U"); keyMap.insert(sf::Keyboard::V, "V");
    keyMap.insert(sf::Keyboard::W, "W"); keyMap.insert(sf::Keyboard::X, "X");
    keyMap.insert(sf::Keyboard::Y, "Y"); keyMap.insert(sf::Keyboard::Z, "Z");

    keyMap.insert(sf::Keyboard::Num0, "Num0"); keyMap.insert(sf::Keyboard::Num1, "Num1");
    keyMap.insert(sf::Keyboard::Num2, "Num2"); keyMap.insert(sf::Keyboard::Num3, "Num3");
    keyMap.insert(sf::Keyboard::Num4, "Num4"); keyMap.insert(sf::Keyboard::Num5, "Num5");
    keyMap.insert(sf::Keyboard::Num6, "Num6"); keyMap.insert(sf::Keyboard::Num7, "Num7");
    keyMap.insert(sf::Keyboard::Num8, "Num8"); keyMap.insert(sf::Keyboard::Num9, "Num9");

    keyMap.insert(sf::Keyboard::Escape, "Escape");
    keyMap.insert(sf::Keyboard::Space, "Space");
    keyMap.insert(sf::Keyboard::Return, "Return");
    keyMap.insert(sf::Keyboard::BackSpace, "BackSpace");
    keyMap.insert(sf::Keyboard::Tab, "Tab");
    keyMap.insert(sf::Keyboard::Left, "Left");
    keyMap.insert(sf::Keyboard::Right, "Right");
    keyMap.insert(sf::Keyboard::Up, "Up");
    keyMap.insert(sf::Keyboard::Down, "Down");
    keyMap.insert(sf::Keyboard::LShift, "LShift");
    keyMap.insert(sf::Keyboard::RShift, "RShift");
    keyMap.insert(sf::Keyboard::LControl, "LControl");
    keyMap.insert(sf::Keyboard::RControl, "RControl");
    keyMap.insert(sf::Keyboard::LAlt, "LAlt");
    keyMap.insert(sf::Keyboard::RAlt, "RAlt");
}

sf::Keyboard::Key KeyMapper::fromName(const std::string& keyName) {
    try {
        return keyMap.get_right(keyName);
    } catch (const std::out_of_range&) {
        return sf::Keyboard::Unknown;
    }
}

std::string KeyMapper::toName(sf::Keyboard::Key key) {
    try {
        return keyMap.get_left(key);
    } catch (const std::out_of_range&) {
        return "Unknown";
    }
}

} // namespace game
