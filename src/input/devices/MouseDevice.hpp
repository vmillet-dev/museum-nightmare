#pragma once
#include "InputDevice.hpp"
#include <SFML/Window.hpp>
#include <map>

namespace game {

class MouseDevice : public InputDevice {
public:
    void init() override;
    void update() override;
    bool isActionPressed(Action action) const override;
    void handleEvent(const sf::Event& event) override;

private:
    bool leftMousePressed = false;
    bool rightMousePressed = false;
};

} // namespace game
