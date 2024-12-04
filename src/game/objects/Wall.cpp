#include "Wall.hpp"
#include "../../core/const.hpp"
#include <spdlog/spdlog.h>

namespace game {

Wall::Wall(b2BodyId bodyId, float width, float height)
    : GameObject(0, 0), width(width), height(height) {
    this->bodyId = bodyId;
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(128, 128, 128));  // Gray color
    shape.setOrigin(width / 2, height / 2);

    // Get initial position from physics body
    b2Vec2 pos = b2Body_GetPosition(bodyId);
    position = sf::Vector2f(pos.x, pos.y);
    shape.setPosition(position);

    spdlog::debug("Wall created with size({:.1f},{:.1f})", width, height);
}

void Wall::update(float deltaTime) {
    // Walls are static, no update needed
    // Position is managed by Box2D
    if (b2Body_IsValid(bodyId)) {
        syncPositionWithPhysics();
        shape.setPosition(position);
    }
}

void Wall::render(sf::RenderWindow& window) {
    window.draw(shape);
}

void Wall::initPhysics(b2WorldId worldId) {
    // Physics body is already created and passed in constructor
    this->worldId = worldId;
    spdlog::debug("Wall physics initialized with existing bodyId");
}

} // namespace game
