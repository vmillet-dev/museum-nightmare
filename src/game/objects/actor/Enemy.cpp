#include "Enemy.hpp"
#include "Player.hpp"
#include <spdlog/spdlog.h>

namespace game {

Enemy::Enemy(float x, float y)
    : Actor(x, y, std::make_unique<RandomAIInputProvider>(), 150.0f) {
    shape.setSize(sf::Vector2f(Constants::Physics::ACTOR_SIZE * 2, Constants::Physics::ACTOR_SIZE * 2));
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(Constants::Physics::ACTOR_SIZE, Constants::Physics::ACTOR_SIZE);
    shape.setPosition(x, y);
    spdlog::info("Enemy created at position ({}, {})", x, y);
}

void Enemy::render(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(shape);
}

void Enemy::initPhysics(b2WorldId worldId) {
    // Call parent's initPhysics first
    Actor::initPhysics(worldId);

    // Customize enemy's physics properties
    if (b2Body_IsValid(bodyId)) {
        // Get all shapes
        const int32_t MAX_SHAPES = 1;
        b2ShapeId shapes[MAX_SHAPES];
        int32_t shapeCount = b2Body_GetShapes(bodyId, shapes, MAX_SHAPES);

        if (shapeCount > 0) {
            b2ShapeId shapeId = shapes[0];
            if (b2Shape_IsValid(shapeId)) {
                b2Shape_SetDensity(shapeId, Constants::Physics::ENEMY_DENSITY);
                b2Shape_SetFriction(shapeId, Constants::Physics::ENEMY_FRICTION);

                // Update mass data
                b2MassData massData = b2Body_GetMassData(bodyId);
                b2Body_SetMassData(bodyId, massData);
            }
        }

        // Set user data for collision handling
        b2Body_SetUserData(bodyId, this);
    }

    spdlog::debug("Enemy physics initialized");
}

} // namespace game
