#include "Player.hpp"
#include "../Wall.hpp"
#include "../../../input/providers/PlayerInputProvider.hpp"
#include "../../../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

Player::Player(float x, float y, Game& game)
    : Actor(x, y, std::make_unique<PlayerInputProvider>(game.getInputManager()), 200.0f) {
    shape.setSize(sf::Vector2f(Constants::Physics::ACTOR_SIZE * 2, Constants::Physics::ACTOR_SIZE * 2));
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(Constants::Physics::ACTOR_SIZE, Constants::Physics::ACTOR_SIZE);
    shape.setPosition(position);
    spdlog::info("Player created at position ({}, {})", x, y);
}

void Player::render(sf::RenderWindow& window) {
    windowSize = window.getSize();
    shape.setPosition(position);
    window.draw(shape);
}

void Player::initPhysics(b2WorldId worldId) {
    // Call parent's initPhysics first
    Actor::initPhysics(worldId);

    // Customize player's physics properties
    if (b2Body_IsValid(bodyId)) {
        // Get all shapes
        const int32_t MAX_SHAPES = 1;
        b2ShapeId shapes[MAX_SHAPES];
        int32_t shapeCount = b2Body_GetShapes(bodyId, shapes, MAX_SHAPES);

        if (shapeCount > 0) {
            b2ShapeId shapeId = shapes[0];
            if (b2Shape_IsValid(shapeId)) {
                b2Shape_SetDensity(shapeId, Constants::Physics::PLAYER_DENSITY);
                b2Shape_SetFriction(shapeId, Constants::Physics::PLAYER_FRICTION);

                // Update mass data
                b2MassData massData = b2Body_GetMassData(bodyId);
                b2Body_SetMassData(bodyId, massData);
            }
        }

        // Set user data for collision handling
        b2Body_SetUserData(bodyId, this);
    }

    spdlog::debug("Player physics initialized");
}

} // namespace game
