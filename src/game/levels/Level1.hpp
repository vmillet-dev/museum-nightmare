#pragma once
#include "../objects/Wall.hpp"
#include "../objects/GameObjectManager.hpp"
#include "GridLevelLoader.hpp"
#include <vector>
#include <memory>

namespace game {

class Level1 {
public:
    static void loadLevel(GameObjectManager& gameObjects) {
        try {
            GridLevelLoader::loadLevelFromFile("assets/levels/level1.txt", gameObjects);
        } catch (const std::exception& e) {
            spdlog::error("Failed to load grid level: {}", e.what());
            // Fallback to hardcoded level if file loading fails
            loadHardcodedLevel(gameObjects);
        }
    }

private:
    static void loadHardcodedLevel(GameObjectManager& gameObjects) {
        // Create border walls
        gameObjects.addObject(std::make_unique<Wall>(400.0f, 16.0f, 800.0f, 32.0f));
        gameObjects.addObject(std::make_unique<Wall>(400.0f, 584.0f, 800.0f, 32.0f));
        gameObjects.addObject(std::make_unique<Wall>(16.0f, 300.0f, 32.0f, 600.0f));
        gameObjects.addObject(std::make_unique<Wall>(784.0f, 300.0f, 32.0f, 600.0f));

        // Add some obstacles
        gameObjects.addObject(std::make_unique<Wall>(300.0f, 200.0f, 64.0f, 64.0f));
        gameObjects.addObject(std::make_unique<Wall>(500.0f, 400.0f, 64.0f, 64.0f));

        spdlog::warn("Using hardcoded level as fallback");
    }
};

} // namespace game
