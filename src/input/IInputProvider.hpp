#pragma once
#include <SFML/System/Vector2.hpp>

namespace game {

enum class Action {
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    Jump,
    Attack,
    Interact,
    Pause
};

class IInputProvider {
public:
    virtual ~IInputProvider() = default;

    // Get normalized movement vector (-1 to 1 for each axis)
    virtual sf::Vector2f getMovementInput() = 0;

    // Check if an action is currently being performed
    virtual bool isActionTriggered(Action action) = 0;

    // Get the strength of an action (0 to 1, useful for analog inputs)
    virtual float getActionStrength(Action action) = 0;

    // Update input state (called each frame)
    virtual void update() = 0;

    // Called when input source changes (e.g., controller connected/disconnected)
    virtual void handleInputSourceChange() = 0;
};

} // namespace game
