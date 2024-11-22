#include "GameObjectManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void GameObjectManager::addObject(std::unique_ptr<GameObject> object) {
    spdlog::debug("Added game object at ({:.1f},{:.1f})", object->getPosition().x, object->getPosition().y);
    objects.push_back(std::move(object));
}

void GameObjectManager::update(float deltaTime) {
    spdlog::debug("GameObjectManager: updating {} objects with dt={:.3f}", objects.size(), deltaTime);
    for (auto& object : objects) {
        object->update(deltaTime);
    }

    // Check for collisions
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i]->getBounds().intersects(objects[j]->getBounds())) {
                spdlog::debug("Collision detected between objects at ({:.1f},{:.1f}) and ({:.1f},{:.1f})",
                    objects[i]->getPosition().x, objects[i]->getPosition().y,
                    objects[j]->getPosition().x, objects[j]->getPosition().y);
                objects[i]->handleCollision(objects[j].get());
                objects[j]->handleCollision(objects[i].get());
            }
        }
    }
}

void GameObjectManager::render(sf::RenderWindow& window) {
    for (const auto& object : objects) {
        object->render(window);
    }
}

void GameObjectManager::clear() {
    spdlog::debug("Clearing all game objects");
    objects.clear();
}

} // namespace game
