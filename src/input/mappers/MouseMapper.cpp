#include "MouseMapper.hpp"
#include <mutex>

namespace game {

bool MouseMapper::initialized = false;
std::mutex MouseMapper::initMutex;
Bimap<sf::Mouse::Button, std::string> MouseMapper::buttonMap;

void MouseMapper::ensureInitialized() {
    std::lock_guard<std::mutex> lock(initMutex);
    if (!initialized) {
        initializeMap();
        initialized = true;
    }
}

void MouseMapper::initializeMap() {
    buttonMap.insert(sf::Mouse::Left, "LeftButton");
    buttonMap.insert(sf::Mouse::Right, "RightButton");
    buttonMap.insert(sf::Mouse::Middle, "MiddleButton");
    buttonMap.insert(sf::Mouse::XButton1, "x1");
    buttonMap.insert(sf::Mouse::XButton2, "x2");
}

sf::Mouse::Button MouseMapper::stringToButton(const std::string& buttonStr) {
    ensureInitialized();
    try {
        return buttonMap.get_right(buttonStr);
    } catch (const std::out_of_range&) {
        return sf::Mouse::ButtonCount;  // Return an invalid button if not found
    }
}

std::string MouseMapper::buttonToString(sf::Mouse::Button button) {
    ensureInitialized();
    try {
        return buttonMap.get_left(button);
    } catch (const std::out_of_range&) {
        return "unknown";  // Return "unknown" if button is not found
    }
}

}
