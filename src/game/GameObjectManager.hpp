#pragma once
#include <vector>
#include <memory>
#include "objects/GameObject.hpp"
#include "physics/PhysicsWorld.hpp"
#include <spdlog/spdlog.h>

namespace game {

class GameObjectManager {
public:
    GameObjectManager();
    void addObject(std::unique_ptr<GameObject> object);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void clear();
    PhysicsWorld& getPhysicsWorld() { return *physicsWorld; }

private:
    std::vector<std::unique_ptr<GameObject>> objects;
    std::unique_ptr<PhysicsWorld> physicsWorld;
};

} // namespace game
