#pragma once
#include "GenericInputDevice.hpp"

namespace game {

class KeyboardDevice : public GenericInputDevice<sf::Keyboard::Key> {
public:
    KeyboardDevice();
    void handleEvent(const sf::Event& event) override;
    void loadBinding() override;
};

} // namespace game
