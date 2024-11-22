#include "AIInputProvider.hpp"
#include <cmath>
#include <random>
#include <spdlog/spdlog.h>

namespace game {

AIInputProvider::AIInputProvider(AIBehaviorType behavior)
    : currentBehavior(behavior)
    , targetPosition(0.f, 0.f)
    , currentMovement(0.f, 0.f)
    , wanderAngle(0.f) {
}

sf::Vector2f AIInputProvider::getMovementInput() {
    return currentMovement;
}

bool AIInputProvider::isActionTriggered(Action action) {
    // AI can trigger actions based on behavior and conditions
    switch (action) {
        case Action::Attack:
            // Example: Attack if in chase mode and close to target
            return currentBehavior == AIBehaviorType::Chase;
        default:
            return false;
    }
}

float AIInputProvider::getActionStrength(Action action) {
    return isActionTriggered(action) ? 1.0f : 0.0f;
}

void AIInputProvider::update() {
    switch (currentBehavior) {
        case AIBehaviorType::Patrol:
            updatePatrol();
            break;
        case AIBehaviorType::Chase:
            updateChase();
            break;
        case AIBehaviorType::Wander:
            updateWander();
            break;
        case AIBehaviorType::Idle:
            updateIdle();
            break;
    }
}

void AIInputProvider::handleInputSourceChange() {
    // AI doesn't need to handle input source changes
}

void AIInputProvider::setBehavior(AIBehaviorType newBehavior) {
    if (currentBehavior != newBehavior) {
        currentBehavior = newBehavior;
        behaviorTimer.restart();
        spdlog::debug("AI behavior changed to: {}", static_cast<int>(newBehavior));
    }
}

void AIInputProvider::setTarget(const sf::Vector2f& targetPos) {
    targetPosition = targetPos;
}

void AIInputProvider::updatePatrol() {
    // Simple patrol between two points
    static const float PATROL_DURATION = 2.0f;
    float time = behaviorTimer.getElapsedTime().asSeconds();

    if (time > PATROL_DURATION) {
        behaviorTimer.restart();
    }

    // Move left and right
    currentMovement.x = std::sin(time * 3.14159f / PATROL_DURATION);
    currentMovement.y = 0;
}

void AIInputProvider::updateChase() {
    // Calculate direction to target
    sf::Vector2f direction = targetPosition - currentMovement;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0) {
        currentMovement = direction / length;
    } else {
        currentMovement = sf::Vector2f(0.f, 0.f);
    }
}

void AIInputProvider::updateWander() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> angleDist(-3.14159f, 3.14159f);

    // Change direction occasionally
    if (behaviorTimer.getElapsedTime().asSeconds() > 1.0f) {
        wanderAngle += angleDist(gen) * 0.5f; // Small random change
        behaviorTimer.restart();
    }

    // Calculate movement from angle
    currentMovement.x = std::cos(wanderAngle);
    currentMovement.y = std::sin(wanderAngle);
}

void AIInputProvider::updateIdle() {
    currentMovement = sf::Vector2f(0.f, 0.f);
}

} // namespace game
