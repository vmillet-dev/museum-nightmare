#include "Player.hpp"
#include "Wall.hpp"
#include <spdlog/spdlog.h>
#include "../../input/InputManager.hpp"

namespace game {

Player::Player(float x, float y) : GameObject(x, y), speed(200.0f), isMoving(false) {
    shape.setSize(sf::Vector2f(32.0f, 32.0f));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(16.0f, 16.0f);
    shape.setPosition(position);
    spdlog::info("Player created at position ({}, {})", x, y);

    bindInputActions();
}

void Player::bindInputActions() {
    auto& inputManager = InputManager::getInstance();

    inputManager.bindAction(Action::MoveLeft, [this]() { handleMoveAction(Action::MoveLeft); });
    inputManager.bindAction(Action::MoveRight, [this]() { handleMoveAction(Action::MoveRight); });
    inputManager.bindAction(Action::MoveUp, [this]() { handleMoveAction(Action::MoveUp); });
    inputManager.bindAction(Action::MoveDown, [this]() { handleMoveAction(Action::MoveDown); });

    inputManager.bindAction(Action::Confirm, [this]() { handleInteractAction(Action::Confirm); });
    inputManager.bindAction(Action::Cancel, [this]() { handleInteractAction(Action::Cancel); });
}

void Player::handleMoveAction(Action action) {
    float dx = 0.0f, dy = 0.0f;

    switch (action) {
        case Action::MoveLeft:  dx = -1.0f; break;
        case Action::MoveRight: dx = 1.0f;  break;
        case Action::MoveUp:    dy = -1.0f; break;
        case Action::MoveDown:  dy = 1.0f;  break;
        default: break;
    }

    if (dx != 0.0f || dy != 0.0f) {
        isMoving = true;
        move(dx, dy, 1.0f/60.0f);
    }
}

void Player::handleInteractAction(Action action) {
    switch (action) {
        case Action::Confirm:
            spdlog::debug("Player confirmed action at position ({}, {})", position.x, position.y);
            break;
        case Action::Cancel:
            spdlog::debug("Player cancelled action at position ({}, {})", position.x, position.y);
            break;
        default:
            break;
    }
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
        spdlog::debug("Pos:({:.1f},{:.1f})->({:.1f},{:.1f}) Vel:({:.1f},{:.1f})",
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

        sf::Vector2f oldPos = position;
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
        spdlog::debug("Collision resolved: from({:.1f},{:.1f}) to({:.1f},{:.1f})", oldPos.x, oldPos.y, position.x, position.y);
    }
}

void Player::move(float dx, float dy, float deltaTime) {
    sf::Vector2f newVelocity(dx, dy);
    if (dx != 0 && dy != 0) {
        float length = std::sqrt(dx * dx + dy * dy);
        newVelocity.x /= length;
        newVelocity.y /= length;
        spdlog::debug("Normalized movement: ({:.2f},{:.2f})", newVelocity.x, newVelocity.y);
    }
    velocity = newVelocity;
}

} // namespace game
