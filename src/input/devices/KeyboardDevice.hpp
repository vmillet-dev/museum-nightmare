#pragma once
#include "GenericInputDevice.hpp"
#include <SFML/Window.hpp>

namespace game {

class KeyboardDevice : public GenericInputDevice<sf::Keyboard::Key> {
public:
    void init() override;
    void handleEvent(const sf::Event& event) override;

private:
    void setKeyBinding(sf::Keyboard::Key key, Action action);
    void setKeyState(sf::Keyboard::Key key, bool pressed);
};

} // namespace game
