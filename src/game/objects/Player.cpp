#include "Player.hpp"
#include "Wall.hpp"
#include <spdlog/spdlog.h>

namespace game {

Player::Player(float x, float y) : GameObject(x, y), speed(200.0f) {
    shape.setSize(sf::Vector2f(32.0f, 32.0f));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(16.0f, 16.0f);
    shape.setPosition(position);
    spdlog::info("Player created at position ({}, {})", x, y);
}

void Player::update(float deltaTime) {
    // Calculate new position based on velocity
    sf::Vector2f newPosition = position + (velocity * speed * deltaTime);

    // Store old position for logging
    sf::Vector2f oldPosition = position;

    // Clamp position to window bounds (using default window size if not set)
    float halfWidth = shape.getSize().x / 2;
    float halfHeight = shape.getSize().y / 2;
    float maxX = (windowSize.x > 0) ? static_cast<float>(windowSize.x) : 800.0f;
    float maxY = (windowSize.y > 0) ? static_cast<float>(windowSize.y) : 600.0f;

    newPosition.x = std::max(halfWidth, std::min(newPosition.x, maxX - halfWidth));
    newPosition.y = std::max(halfHeight, std::min(newPosition.y, maxY - halfHeight));

    // Update position
    position = newPosition;
    shape.setPosition(position);

    if (oldPosition != position) {
        spdlog::debug("Player moved from ({}, {}) to ({}, {}), velocity: ({}, {})",
                     oldPosition.x, oldPosition.y,
                     position.x, position.y,
                     velocity.x, velocity.y);
    }
}

void Player::render(sf::RenderWindow& window) {
    // Update window size
    windowSize = window.getSize();
    window.draw(shape);
}

void Player::setVelocity(const sf::Vector2f& newVelocity) {
    velocity = newVelocity;
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

void Player::handleCollision(GameObject* other) {
    if (auto* wall = dynamic_cast<Wall*>(other)) {
        sf::FloatRect playerBounds = getBounds();
        sf::FloatRect wallBounds = wall->getBounds();

        // Calculate overlap
        float overlapLeft = playerBounds.left + playerBounds.width - wallBounds.left;
        float overlapRight = wallBounds.left + wallBounds.width - playerBounds.left;
        float overlapTop = playerBounds.top + playerBounds.height - wallBounds.top;
        float overlapBottom = wallBounds.top + wallBounds.height - playerBounds.top;

        // Find smallest overlap
        float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

        // Resolve collision
        if (minOverlap == overlapLeft) {
            position.x = wallBounds.left - (playerBounds.width / 2);
        } else if (minOverlap == overlapRight) {
            position.x = wallBounds.left + wallBounds.width + (playerBounds.width / 2);
        } else if (minOverlap == overlapTop) {
            position.y = wallBounds.top - (playerBounds.height / 2);
        } else if (minOverlap == overlapBottom) {
            position.y = wallBounds.top + wallBounds.height + (playerBounds.height / 2);
        }

        // Update shape position
        shape.setPosition(position);
    }
}

void Player::move(float dx, float dy, float deltaTime) {
    sf::Vector2f newVelocity(dx, dy);

    // Normalize velocity if moving diagonally
    if (dx != 0 && dy != 0) {
        float length = std::sqrt(dx * dx + dy * dy);
        newVelocity.x /= length;
        newVelocity.y /= length;
    }

    velocity = newVelocity;
    spdlog::debug("Player velocity updated to ({}, {})", velocity.x, velocity.y);
}

} // namespace game
