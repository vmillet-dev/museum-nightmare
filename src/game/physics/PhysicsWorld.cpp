#include "PhysicsWorld.hpp"
#include "ContactListener.hpp"
#include <spdlog/spdlog.h>

namespace game {

PhysicsWorld::PhysicsWorld() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, GRAVITY_Y};
    worldId = b2CreateWorld(&worldDef);
    spdlog::debug("PhysicsWorld initialized with gravity {}", GRAVITY_Y);
}

PhysicsWorld::~PhysicsWorld() {
    if (b2World_IsValid(worldId)) {
        b2DestroyWorld(worldId);
    }
}

void PhysicsWorld::update(float deltaTime) {
    if (!b2World_IsValid(worldId)) return;

    // Step the physics simulation
    b2World_Step(worldId, deltaTime, VELOCITY_ITERATIONS);

    // Process any contact events that occurred during the step
    ContactListener::processContactEvents(worldId);
}

b2BodyId PhysicsWorld::createBody(const b2BodyDef* bodyDef) {
    if (!b2World_IsValid(worldId)) {
        return b2_nullBodyId;
    }
    return b2CreateBody(worldId, bodyDef);
}

void PhysicsWorld::destroyBody(b2BodyId bodyId) {
    if (b2World_IsValid(worldId) && b2Body_IsValid(bodyId)) {
        b2DestroyBody(bodyId);
    }
}

} // namespace game
