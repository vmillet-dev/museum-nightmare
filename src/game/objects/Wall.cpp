#include "Wall.hpp"
#include <spdlog/spdlog.h>

namespace game {

Wall::Wall(float x, float y, float width, float height) : GameObject(x, y) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(128, 128, 128));  // Gray color
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(position);
    spdlog::debug("Wall created: pos({:.1f},{:.1f}) size({:.1f},{:.1f})", x, y, width, height);
}

void Wall::update(float deltaTime) {
    // Walls are static, no update needed
}

void Wall::render(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Wall::getBounds() const {
    return shape.getGlobalBounds();
}

void Wall::handleCollision(GameObject* other) {
    //if (other) { spdlog::debug("Wall collision at ({:.1f},{:.1f})", position.x, position.y); }
}

} // namespace game
