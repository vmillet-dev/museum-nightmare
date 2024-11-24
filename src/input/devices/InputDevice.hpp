#pragma once
#include <SFML/Window.hpp>
#include "../Action.hpp"

namespace game {

class InputDevice {
public:
    virtual ~InputDevice() = default;
    virtual void init() = 0;
    virtual void update() = 0;
    virtual bool isActionPressed(Action action) = 0;
    virtual bool isActionJustPressed(Action action) = 0;
    virtual bool isActionReleased(Action action) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
};

} // namespace game
