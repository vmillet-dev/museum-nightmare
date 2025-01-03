#pragma once
#include <SFML/Window.hpp>
#include "../Action.hpp"

namespace game {

struct ActionState {
    bool current;
    bool previous;
};

class InputDevice {
public:
    virtual ~InputDevice() = default;
    virtual void update() = 0;
    virtual bool isActionPressed(Action action) = 0;
    virtual bool isActionJustPressed(Action action) = 0;
    virtual bool isActionReleased(Action action) = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void loadBinding() = 0;
};

} // namespace game
