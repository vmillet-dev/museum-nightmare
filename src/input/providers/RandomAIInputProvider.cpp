#include "RandomAIInputProvider.hpp"

namespace game {

RandomAIInputProvider::RandomAIInputProvider()
    : rng(std::random_device{}())
    , dist(-1.0f, 1.0f)
    , lastChangeTime(std::chrono::steady_clock::now())
    , currentMovement(0.0f, 0.0f)
    , movementDuration(2.0f) {
}

sf::Vector2f RandomAIInputProvider::getMovementVector() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        now - lastChangeTime).count();

    if (elapsed >= movementDuration) {
        updateMovement();
        lastChangeTime = now;
    }

    return currentMovement;
}

void RandomAIInputProvider::updateMovement() {
    currentMovement.x = dist(rng);
    currentMovement.y = dist(rng);

    // Normalize the movement vector
    float length = std::sqrt(currentMovement.x * currentMovement.x +
                           currentMovement.y * currentMovement.y);
    if (length > 0) {
        currentMovement.x /= length;
        currentMovement.y /= length;
    }
}

bool RandomAIInputProvider::isActionActive(Action action) {
    return false; // AI doesn't use actions for now
}

} // namespace game
