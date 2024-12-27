#pragma once
#include <SFML/Window.hpp>
#include "../Action.hpp"

namespace game {

struct ActionState {
    float current = 0.0f;
    float previous = 0.0f;

    // Helper functions to maintain backward compatibility
    bool isPressed() const { return current > 0.5f; }
    bool wasPressed() const { return previous > 0.5f; }
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
