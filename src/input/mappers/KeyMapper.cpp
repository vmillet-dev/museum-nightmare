#include "KeyMapper.hpp"

namespace game {

KeyMapper::KeyMapper() {
    initializeKeyMap();
}

void KeyMapper::initializeKeyMap() {
    keyMap.insert(sf::Keyboard::Key::A, "A"); keyMap.insert(sf::Keyboard::Key::B, "B");
    keyMap.insert(sf::Keyboard::Key::C, "C"); keyMap.insert(sf::Keyboard::Key::D, "D");
    keyMap.insert(sf::Keyboard::Key::E, "E"); keyMap.insert(sf::Keyboard::Key::F, "F");
    keyMap.insert(sf::Keyboard::Key::G, "G"); keyMap.insert(sf::Keyboard::Key::H, "H");
    keyMap.insert(sf::Keyboard::Key::I, "I"); keyMap.insert(sf::Keyboard::Key::J, "J");
    keyMap.insert(sf::Keyboard::Key::K, "K"); keyMap.insert(sf::Keyboard::Key::L, "L");
    keyMap.insert(sf::Keyboard::Key::M, "M"); keyMap.insert(sf::Keyboard::Key::N, "N");
    keyMap.insert(sf::Keyboard::Key::O, "O"); keyMap.insert(sf::Keyboard::Key::P, "P");
    keyMap.insert(sf::Keyboard::Key::Q, "Q"); keyMap.insert(sf::Keyboard::Key::R, "R");
    keyMap.insert(sf::Keyboard::Key::S, "S"); keyMap.insert(sf::Keyboard::Key::T, "T");
    keyMap.insert(sf::Keyboard::Key::U, "U"); keyMap.insert(sf::Keyboard::Key::V, "V");
    keyMap.insert(sf::Keyboard::Key::W, "W"); keyMap.insert(sf::Keyboard::Key::X, "X");
    keyMap.insert(sf::Keyboard::Key::Y, "Y"); keyMap.insert(sf::Keyboard::Key::Z, "Z");

    keyMap.insert(sf::Keyboard::Key::Num0, "Num0"); keyMap.insert(sf::Keyboard::Key::Num1, "Num1");
    keyMap.insert(sf::Keyboard::Key::Num2, "Num2"); keyMap.insert(sf::Keyboard::Key::Num3, "Num3");
    keyMap.insert(sf::Keyboard::Key::Num4, "Num4"); keyMap.insert(sf::Keyboard::Key::Num5, "Num5");
    keyMap.insert(sf::Keyboard::Key::Num6, "Num6"); keyMap.insert(sf::Keyboard::Key::Num7, "Num7");
    keyMap.insert(sf::Keyboard::Key::Num8, "Num8"); keyMap.insert(sf::Keyboard::Key::Num9, "Num9");

    keyMap.insert(sf::Keyboard::Key::Escape, "Escape");
    keyMap.insert(sf::Keyboard::Key::Space, "Space");
    keyMap.insert(sf::Keyboard::Key::Enter, "Return");
    keyMap.insert(sf::Keyboard::Key::Backspace, "BackSpace");
    keyMap.insert(sf::Keyboard::Key::Tab, "Tab");
    keyMap.insert(sf::Keyboard::Key::Left, "Left");
    keyMap.insert(sf::Keyboard::Key::Right, "Right");
    keyMap.insert(sf::Keyboard::Key::Up, "Up");
    keyMap.insert(sf::Keyboard::Key::Down, "Down");
    keyMap.insert(sf::Keyboard::Key::LShift, "LShift");
    keyMap.insert(sf::Keyboard::Key::RShift, "RShift");
    keyMap.insert(sf::Keyboard::Key::LControl, "LControl");
    keyMap.insert(sf::Keyboard::Key::RControl, "RControl");
    keyMap.insert(sf::Keyboard::Key::LAlt, "LAlt");
    keyMap.insert(sf::Keyboard::Key::RAlt, "RAlt");
}

sf::Keyboard::Key KeyMapper::stringToKey(const std::string& keyName) {
    try {
        return keyMap.get_right(keyName);
    } catch (const std::out_of_range&) {
        return sf::Keyboard::Key::Unknown;
    }
}

std::string KeyMapper::keyToString(sf::Keyboard::Key key) {
    try {
        return keyMap.get_left(key);
    } catch (const std::out_of_range&) {
        return "Unknown";
    }
}

} // namespace game
