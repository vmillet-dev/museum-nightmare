#include "Player.hpp"
#include "../Wall.hpp"
#include "../../../input/providers/PlayerInputProvider.hpp"
#include <spdlog/spdlog.h>

namespace game {

Player::Player(float x, float y)
    : Actor(x, y, std::make_unique<PlayerInputProvider>(), 200.0f) {
    shape.setSize(sf::Vector2f(32.0f, 32.0f));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(16.0f, 16.0f);
    shape.setPosition(position);
    spdlog::info("Player created at position ({}, {})", x, y);
}

void Player::render(sf::RenderWindow& window) {
    windowSize = window.getSize();
    window.draw(shape);
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

} // namespace game
