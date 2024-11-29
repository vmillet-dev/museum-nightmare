#include "GameObjectManager.hpp"
#include "objects/ParallaxLayer.hpp"
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

void GameObjectManager::update(float deltaTime, const sf::Vector2f& cameraPosition) {
    // Update physics world
    physicsWorld->update(deltaTime);

    // Update game objects
    for (auto& object : objects) {
        // Update parallax layers with camera position
        if (auto* parallaxLayer = dynamic_cast<ParallaxLayer*>(object.get())) {
            parallaxLayer->setParallaxOffset(cameraPosition);
        }
        object->update(deltaTime);
    }
}

void GameObjectManager::render(sf::RenderWindow& window) {
    // Render parallax layers first
    for (const auto& object : objects) {
        if (dynamic_cast<ParallaxLayer*>(object.get())) {
            object->render(window);
        }
    }

    // Then render other game objects
    for (const auto& object : objects) {
        if (!dynamic_cast<ParallaxLayer*>(object.get())) {
            object->render(window);
        }
    }
}

void GameObjectManager::clear() {
    objects.clear();
    // Physics world will clean up bodies when destroyed
}

} // namespace game
