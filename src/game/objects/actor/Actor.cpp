#include "Actor.hpp"
#include <spdlog/spdlog.h>

namespace game {

Actor::Actor(float x, float y, std::unique_ptr<InputProvider> provider, float speed)
    : GameObject(x, y), inputProvider(std::move(provider)), speed(speed) {}

void Actor::update(float deltaTime) {
    if (inputProvider && b2Body_IsValid(bodyId)) {
        sf::Vector2f movement = inputProvider->getMovementVector();
        if (movement.x != 0.0f || movement.y != 0.0f) {
            // Apply force/impulse to Box2D body instead of direct position change
            b2Vec2 force = {movement.x * speed, movement.y * speed};
            b2Vec2 center = b2Body_GetPosition(bodyId);
            b2Body_ApplyLinearImpulse(bodyId, force, center, true);
        }
        // Sync SFML position with Box2D body position
        syncPositionWithPhysics();
    }
}

void Actor::initPhysics(b2WorldId worldId) {
    this->worldId = worldId;

    // Create body definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {position.x, position.y};
    bodyDef.fixedRotation = true; // Prevent rotation

    // Create the body
    bodyId = b2CreateBody(worldId, &bodyDef);

    // Create shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = DENSITY;
    shapeDef.friction = FRICTION;

    // Create box shape
    float halfSize = ACTOR_SIZE;
    b2Polygon boxShape = b2MakeBox(halfSize, halfSize);

    // Create the fixture with the box shape
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    spdlog::debug("Actor physics initialized at ({}, {})", position.x, position.y);
}

void Actor::move(float x, float y, float deltaTime) {
    if (b2Body_IsValid(bodyId)) {
        b2Vec2 force = {x * speed * deltaTime, y * speed * deltaTime};
        b2Vec2 center = b2Body_GetPosition(bodyId);
        b2Body_ApplyLinearImpulse(bodyId, force, center, true);
        syncPositionWithPhysics();
    }
}

} // namespace game
