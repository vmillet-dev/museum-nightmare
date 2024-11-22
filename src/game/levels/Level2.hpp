#pragma once
#include "../GameObjectManager.hpp"
#include "../objects/Wall.hpp"
#include "../objects/actor/Enemy.hpp"
#include <memory>

namespace game {

class Level2 {
public:
    static void loadLevel(GameObjectManager& manager) {
        // Create outer walls
        manager.addObject(std::make_unique<Wall>(0.0f, 0.0f, 800.0f, 20.0f));      // Top
        manager.addObject(std::make_unique<Wall>(0.0f, 580.0f, 800.0f, 20.0f));    // Bottom
        manager.addObject(std::make_unique<Wall>(0.0f, 0.0f, 20.0f, 600.0f));      // Left
        manager.addObject(std::make_unique<Wall>(780.0f, 0.0f, 20.0f, 600.0f));    // Right

        // Create inner walls for maze-like structure
        manager.addObject(std::make_unique<Wall>(200.0f, 100.0f, 20.0f, 200.0f));
        manager.addObject(std::make_unique<Wall>(400.0f, 300.0f, 200.0f, 20.0f));
        manager.addObject(std::make_unique<Wall>(600.0f, 100.0f, 20.0f, 300.0f));
        manager.addObject(std::make_unique<Wall>(200.0f, 400.0f, 300.0f, 20.0f));

        // Add enemy
        manager.addObject(std::make_unique<Enemy>(600.0f, 500.0f));
    }
};

} // namespace game
