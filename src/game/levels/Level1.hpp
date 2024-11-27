#pragma once
#include "../objects/Wall.hpp"
#include "../GameObjectManager.hpp"
#include <memory>
#include "../../../core/const.hpp"

namespace game {

class Level1 {
public:
    static void loadLevel(GameObjectManager& gameObjects) {
        // Create border walls
        // Top wall
        gameObjects.addObject(std::make_unique<Wall>(400.0f, Constants::Physics::WALL_HALF_THICKNESS, 800.0f, Constants::Physics::WALL_THICKNESS));
        // Bottom wall
        gameObjects.addObject(std::make_unique<Wall>(400.0f, 584.0f, 800.0f, Constants::Physics::WALL_THICKNESS));
        // Left wall
        gameObjects.addObject(std::make_unique<Wall>(Constants::Physics::WALL_HALF_THICKNESS, 300.0f, Constants::Physics::WALL_THICKNESS, 600.0f));
        // Right wall
        gameObjects.addObject(std::make_unique<Wall>(784.0f, 300.0f, Constants::Physics::WALL_THICKNESS, 600.0f));

        // Add some obstacles
        gameObjects.addObject(std::make_unique<Wall>(300.0f, 200.0f, Constants::Physics::WALL_THICKNESS * 2, Constants::Physics::WALL_THICKNESS * 2));
        gameObjects.addObject(std::make_unique<Wall>(500.0f, 400.0f, Constants::Physics::WALL_THICKNESS * 2, Constants::Physics::WALL_THICKNESS * 2));
    }
};

} // namespace game
