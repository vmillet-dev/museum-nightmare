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
    sf::Vector2f newPosition = position + (velocity * deltaTime);

    // Store old position for collision resolution
    sf::Vector2f oldPosition = position;

    // Update position
    position = newPosition;
    shape.setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
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
    position.x += dx * speed * deltaTime;
    position.y += dy * speed * deltaTime;
    shape.setPosition(position);
    spdlog::debug("Player moved to ({}, {})", position.x, position.y);
}

} // namespace game
