#include "FloorObject.hpp"
#include "../physics/PhysicsWorld.hpp"
#include <box2d/box2d.h>

namespace game {

FloorObject::FloorObject(const sf::Texture& texture, const sf::IntRect& textureRect,
                        const sf::Vector2f& position, b2WorldId worldId)
    : GameObject(position.x, position.y) {
    sprite.setTexture(texture);
    sprite.setTextureRect(textureRect);
    sprite.setPosition(position);

    if (b2World_IsValid(worldId)) {
        initPhysics(worldId);
    }
}

FloorObject::~FloorObject() {
    if (b2Body_IsValid(bodyId)) {
        b2DestroyBody(bodyId);
    }
}

void FloorObject::update(float deltaTime) {
    // Floor objects are static, no update needed
    syncPositionWithPhysics();
}

void FloorObject::render(sf::RenderWindow& window) {  // Remove const to match base class
    window.draw(sprite);
}

void FloorObject::initPhysics(b2WorldId worldId) {
    this->worldId = worldId;

    const float PHYSICS_SCALE = 30.0f; // Define scale factor for Box2D

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position.x = position.x / PHYSICS_SCALE;
    bodyDef.position.y = position.y / PHYSICS_SCALE;

    bodyId = b2CreateBody(worldId, &bodyDef);

    // Create polygon shape for the floor
    float halfWidth = (sprite.getTextureRect().width / 2.0f) / PHYSICS_SCALE;
    float halfHeight = (sprite.getTextureRect().height / 2.0f) / PHYSICS_SCALE;

    // Create box polygon and shape
    b2Polygon polygon = b2MakeBox(halfWidth, halfHeight);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 0.0f;
    shapeDef.friction = 0.3f;

    b2CreatePolygonShape(bodyId, &shapeDef, &polygon);
}

} // namespace game
