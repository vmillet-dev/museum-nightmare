#include "Wall.hpp"

namespace game {

Wall::Wall(float x, float y, float width, float height) : GameObject(x, y) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(128, 128, 128));  // Gray color
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(position);
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
    // Walls don't need to handle collisions themselves
}

} // namespace game
