#pragma once
#include <SFML/Window.hpp>
#include "../Action.hpp"

namespace game {

class InputDevice {
public:
    virtual ~InputDevice() = default;
    virtual void init() = 0;

    // New method to check action state
    enum class ActionState {
        NONE,       // Action is not active
        PRESSED,    // Action is currently active
        JUST_PRESSED, // Action just became active this frame
        RELEASED    // Action was just released this frame
    };

    virtual ActionState getActionState(Action action) const = 0;
    virtual void handleEvent(const sf::Event& event) = 0;
};

} // namespace game
