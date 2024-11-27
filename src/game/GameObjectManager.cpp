#include "GameObjectManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameObjectManager::GameObjectManager() : physicsWorld(std::make_unique<PhysicsWorld>()) {
    spdlog::debug("GameObjectManager initialized with physics world");
}

void GameObjectManager::addObject(std::unique_ptr<GameObject> object) {
    // Initialize physics for the new object
    object->initPhysics(physicsWorld->getWorld());
    objects.push_back(std::move(object));
}

void GameObjectManager::update(float deltaTime) {
    // Update physics world
    physicsWorld->update(deltaTime);

    // Update game objects
    for (auto& object : objects) {
        object->update(deltaTime);
    }
    // Box2D now handles all collision detection and resolution
}

void GameObjectManager::render(sf::RenderWindow& window) {
    for (const auto& object : objects) {
        object->render(window);
    }
}

void GameObjectManager::clear() {
    objects.clear();
    // Physics world will clean up bodies when destroyed
}

} // namespace game
