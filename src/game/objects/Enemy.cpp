#include "Enemy.hpp"
#include "Player.hpp"
#include <spdlog/spdlog.h>

namespace game {

Enemy::Enemy(float x, float y)
    : Actor(x, y, std::make_unique<RandomAIInputProvider>(), 150.0f) {
    shape.setSize(sf::Vector2f(32.0f, 32.0f));
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(16.0f, 16.0f);
    shape.setPosition(x, y);
    spdlog::info("Enemy created at position ({}, {})", x, y);
}

void Enemy::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}

void Enemy::handleCollision(GameObject* other) {
    // Basic collision response - move away from collision
    if (auto* player = dynamic_cast<Player*>(other)) {
        sf::Vector2f moveBack = getPosition() - other->getPosition();
        float length = std::sqrt(moveBack.x * moveBack.x + moveBack.y * moveBack.y);
        if (length > 0) {
            moveBack /= length;
            setPosition(getPosition() + moveBack * 5.0f);
        }
    }
}

} // namespace game
