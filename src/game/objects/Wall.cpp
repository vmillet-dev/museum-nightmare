#include "Wall.hpp"
#include <spdlog/spdlog.h>

namespace game {

Wall::Wall(float x, float y, float width, float height)
    : GameObject(x, y), width(width), height(height) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(128, 128, 128));  // Gray color
    shape.setOrigin(width / 2, height / 2);
    shape.setPosition(position);
    spdlog::debug("Wall created: pos({:.1f},{:.1f}) size({:.1f},{:.1f})", x, y, width, height);
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
    this->worldId = worldId;

    // Create body definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;  // Walls are static
    bodyDef.position = {position.x, position.y};

    // Create the body
    bodyId = b2CreateBody(worldId, &bodyDef);

    // Create shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.friction = WALL_FRICTION;

    // Create box polygon
    float hx = width / 2.0f;
    float hy = height / 2.0f;
    b2Polygon boxShape = b2MakeBox(hx, hy);

    // Create the fixture
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    spdlog::debug("Wall physics initialized at ({}, {})", position.x, position.y);
}

} // namespace game
