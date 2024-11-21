#include "GameObjectManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void GameObjectManager::addObject(std::unique_ptr<GameObject> object, ObjectType type) {
    size_t id = nextId++;
    object->setId(id);
    spdlog::debug("Added {} object (ID: {}) at ({:.1f},{:.1f})",
        static_cast<int>(type), id, object->getPosition().x, object->getPosition().y);

    objects.push_back(std::move(object));
    objectTypes[id] = type;
    typeIndex[type].push_back(id);
}

void GameObjectManager::removeObject(size_t id) {
    auto typeIt = objectTypes.find(id);
    if (typeIt != objectTypes.end()) {
        ObjectType type = typeIt->second;
        auto& typeIds = typeIndex[type];
        typeIds.erase(std::remove(typeIds.begin(), typeIds.end(), id), typeIds.end());
        objectTypes.erase(typeIt);

        // Remove from objects vector
        auto objIt = std::find_if(objects.begin(), objects.end(),
            [id](const auto& obj) { return obj->getId() == id; });
        if (objIt != objects.end()) {
            objects.erase(objIt);
        }
    }
}

GameObject* GameObjectManager::getObject(size_t id) {
    auto it = std::find_if(objects.begin(), objects.end(),
        [id](const auto& obj) { return obj->getId() == id; });
    return it != objects.end() ? it->get() : nullptr;
}

GameObject* GameObjectManager::getObjectByType(ObjectType type) {
    auto& ids = typeIndex[type];
    if (!ids.empty()) {
        return getObject(ids[0]);
    }
    return nullptr;
}

std::vector<GameObject*> GameObjectManager::getObjectsByType(ObjectType type) {
    std::vector<GameObject*> result;
    auto& ids = typeIndex[type];
    for (size_t id : ids) {
        if (auto obj = getObject(id)) {
            result.push_back(obj);
        }
    }
    return result;
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
    objectTypes.clear();
    typeIndex.clear();
    nextId = 0;
}

} // namespace game
