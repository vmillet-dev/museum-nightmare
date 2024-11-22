#pragma once
#include "../objects/Wall.hpp"
#include "../GameObjectManager.hpp"
#include <vector>
#include <memory>

namespace game {

class Level1 {
public:
    static void loadLevel(GameObjectManager& gameObjects) {
        // Create border walls
        // Top wall
        gameObjects.addObject(std::make_unique<Wall>(400.0f, 16.0f, 800.0f, 32.0f));
        // Bottom wall
        gameObjects.addObject(std::make_unique<Wall>(400.0f, 584.0f, 800.0f, 32.0f));
        // Left wall
        gameObjects.addObject(std::make_unique<Wall>(16.0f, 300.0f, 32.0f, 600.0f));
        // Right wall
        gameObjects.addObject(std::make_unique<Wall>(784.0f, 300.0f, 32.0f, 600.0f));

        // Add some obstacles
        gameObjects.addObject(std::make_unique<Wall>(300.0f, 200.0f, 64.0f, 64.0f));
        gameObjects.addObject(std::make_unique<Wall>(500.0f, 400.0f, 64.0f, 64.0f));
    }
};

} // namespace game
