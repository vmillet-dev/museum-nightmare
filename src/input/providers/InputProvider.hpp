#pragma once
#include <SFML/System/Vector2.hpp>
#include "../../input/InputManager.hpp"

namespace game {

/**
 * Abstract base class for input providers.
 * Provides a common interface for different input sources (player, AI, etc.)
 */
class InputProvider {
public:
    virtual ~InputProvider() = default;

    /**
     * Get the current movement vector based on input
     * @return Vector2f with x/y components in range [-1,1]
     */
    virtual sf::Vector2f getMovementVector() = 0;

    /**
     * Check if a specific action is currently active
     * @param action The action to check
     * @return true if the action is active
     */
    virtual bool isActionActive(Action action) = 0;
};

} // namespace game
