#include "GameObjectManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void GameObjectManager::addObject(std::unique_ptr<GameObject> object) {
    size_t id = nextId++;
    object->setId(id);
    spdlog::debug("Added object (ID: {}) at ({:.1f},{:.1f})",
        id, object->getPosition().x, object->getPosition().y);
    objects.push_back(std::move(object));
    objectIndex[id] = objects.size() - 1;
}

void GameObjectManager::removeObject(size_t id) {
    auto it = objectIndex.find(id);
    if (it != objectIndex.end()) {
        size_t index = it->second;
        objects.erase(objects.begin() + index);
        objectIndex.erase(it);

        // Update indices for remaining objects
        for (auto& pair : objectIndex) {
            if (pair.second > index) {
                pair.second--;
            }
        }
    }
}

GameObject* GameObjectManager::getObject(size_t id) {
    auto it = objectIndex.find(id);
    return it != objectIndex.end() ? objects[it->second].get() : nullptr;
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
    objectIndex.clear();
    nextId = 0;
}

} // namespace game
