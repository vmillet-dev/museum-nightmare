#include "KeyMapper.hpp"

namespace game {

KeyMapper::KeyMapper() {
    // Initialize the key mappings
    keyMap.insert("A", sf::Keyboard::A); keyMap.insert("B", sf::Keyboard::B);
    keyMap.insert("C", sf::Keyboard::C); keyMap.insert("D", sf::Keyboard::D);
    keyMap.insert("E", sf::Keyboard::E); keyMap.insert("F", sf::Keyboard::F);
    keyMap.insert("G", sf::Keyboard::G); keyMap.insert("H", sf::Keyboard::H);
    keyMap.insert("I", sf::Keyboard::I); keyMap.insert("J", sf::Keyboard::J);
    keyMap.insert("K", sf::Keyboard::K); keyMap.insert("L", sf::Keyboard::L);
    keyMap.insert("M", sf::Keyboard::M); keyMap.insert("N", sf::Keyboard::N);
    keyMap.insert("O", sf::Keyboard::O); keyMap.insert("P", sf::Keyboard::P);
    keyMap.insert("Q", sf::Keyboard::Q); keyMap.insert("R", sf::Keyboard::R);
    keyMap.insert("S", sf::Keyboard::S); keyMap.insert("T", sf::Keyboard::T);
    keyMap.insert("U", sf::Keyboard::U); keyMap.insert("V", sf::Keyboard::V);
    keyMap.insert("W", sf::Keyboard::W); keyMap.insert("X", sf::Keyboard::X);
    keyMap.insert("Y", sf::Keyboard::Y); keyMap.insert("Z", sf::Keyboard::Z);

    keyMap.insert("Num0", sf::Keyboard::Num0); keyMap.insert("Num1", sf::Keyboard::Num1);
    keyMap.insert("Num2", sf::Keyboard::Num2); keyMap.insert("Num3", sf::Keyboard::Num3);
    keyMap.insert("Num4", sf::Keyboard::Num4); keyMap.insert("Num5", sf::Keyboard::Num5);
    keyMap.insert("Num6", sf::Keyboard::Num6); keyMap.insert("Num7", sf::Keyboard::Num7);
    keyMap.insert("Num8", sf::Keyboard::Num8); keyMap.insert("Num9", sf::Keyboard::Num9);

    keyMap.insert("Escape", sf::Keyboard::Escape);
    keyMap.insert("Space", sf::Keyboard::Space);
    keyMap.insert("Return", sf::Keyboard::Return);
    keyMap.insert("BackSpace", sf::Keyboard::BackSpace);
    keyMap.insert("Tab", sf::Keyboard::Tab);
    keyMap.insert("Left", sf::Keyboard::Left);
    keyMap.insert("Right", sf::Keyboard::Right);
    keyMap.insert("Up", sf::Keyboard::Up);
    keyMap.insert("Down", sf::Keyboard::Down);
    keyMap.insert("LShift", sf::Keyboard::LShift);
    keyMap.insert("RShift", sf::Keyboard::RShift);
    keyMap.insert("LControl", sf::Keyboard::LControl);
    keyMap.insert("RControl", sf::Keyboard::RControl);
    keyMap.insert("LAlt", sf::Keyboard::LAlt);
    keyMap.insert("RAlt", sf::Keyboard::RAlt);
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
