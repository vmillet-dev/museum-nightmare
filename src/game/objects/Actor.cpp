#include "Actor.hpp"
#include "../states/ActorStates.hpp"
#include <spdlog/spdlog.h>
#include <cmath>

namespace game {

Actor::Actor(float x, float y) : GameObject(x, y), deltaTime(0.0f) {
    shape.setSize(sf::Vector2f(32.0f, 32.0f));
    shape.setOrigin(16.0f, 16.0f);
    shape.setPosition(position);

    // Start in idle state
    handleStateTransition(ActorStateType::Idle);
}

void Actor::update(float dt) {
    deltaTime = dt;  // Store deltaTime for use in processInput
    if (inputProvider) {
        inputProvider->update();
        processInput();
    }

    if (currentState) {
        currentState->update(this, dt);
    }

    updatePhysics(dt);
    shape.setPosition(position);
}

void Actor::render(sf::RenderWindow& window) {
    window.draw(shape);
}

void Actor::handleCollision(GameObject* other) {
    // Basic collision response, can be overridden by derived classes
    sf::FloatRect bounds = getBounds();
    sf::FloatRect otherBounds = other->getBounds();

    // Calculate overlap
    float overlapLeft = bounds.left + bounds.width - otherBounds.left;
    float overlapRight = otherBounds.left + otherBounds.width - bounds.left;
    float overlapTop = bounds.top + bounds.height - otherBounds.top;
    float overlapBottom = otherBounds.top + otherBounds.height - bounds.top;

    // Find smallest overlap
    float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

    // Resolve collision
    if (minOverlap == overlapLeft) {
        position.x = otherBounds.left - (bounds.width / 2);
        velocity.x = 0;
    } else if (minOverlap == overlapRight) {
        position.x = otherBounds.left + otherBounds.width + (bounds.width / 2);
        velocity.x = 0;
    } else if (minOverlap == overlapTop) {
        position.y = otherBounds.top - (bounds.height / 2);
        velocity.y = 0;
    } else if (minOverlap == overlapBottom) {
        position.y = otherBounds.top + otherBounds.height + (bounds.height / 2);
        velocity.y = 0;
    }
}

sf::FloatRect Actor::getBounds() const {
    return shape.getGlobalBounds();
}

void Actor::setInputProvider(std::unique_ptr<IInputProvider> provider) {
    inputProvider = std::move(provider);
}

void Actor::setState(std::unique_ptr<ActorState> newState) {
    if (currentState) {
        currentState->exit(this);
    }
    currentState = std::move(newState);
    if (currentState) {
        currentState->enter(this);
    }
}

ActorStateType Actor::getCurrentStateType() const {
    return currentState ? currentState->getType() : ActorStateType::Idle;
}

void Actor::setProperties(const ActorProperties& props) {
    properties = props;
}

const ActorProperties& Actor::getProperties() const {
    return properties;
}

void Actor::handleStateTransition(ActorStateType newState) {
    // Base implementation - derived classes can override to customize state transitions
    switch (newState) {
        case ActorStateType::Idle:
            setState(std::make_unique<IdleState>());
            break;
        case ActorStateType::Walking:
            setState(std::make_unique<WalkingState>());
            break;
        // Add other states as needed
        default:
            spdlog::warn("Unhandled state transition to: {}", static_cast<int>(newState));
            break;
    }
}

void Actor::updatePhysics(float deltaTime) {
    // Apply velocity
    position += velocity * deltaTime;

    // Apply gravity if jumping/falling
    if (getCurrentStateType() == ActorStateType::Jumping) {
        velocity.y += properties.gravity * deltaTime;
    }
}

void Actor::processInput() {
    if (!inputProvider) return;

    // Get movement input
    sf::Vector2f input = inputProvider->getMovementInput();

    // Calculate target velocity based on input
    sf::Vector2f targetVelocity = input * properties.maxSpeed;

    // Smoothly interpolate current velocity towards target
    float acceleration = input.x != 0 || input.y != 0 ? properties.acceleration : properties.deceleration;

    velocity.x = moveTowards(velocity.x, targetVelocity.x, acceleration * deltaTime);
    velocity.y = moveTowards(velocity.y, targetVelocity.y, acceleration * deltaTime);
}

// Utility function for smooth movement
float Actor::moveTowards(float current, float target, float maxDelta) {
    if (std::abs(target - current) <= maxDelta) {
        return target;
    }
    return current + std::copysign(maxDelta, target - current);
}

} // namespace game
