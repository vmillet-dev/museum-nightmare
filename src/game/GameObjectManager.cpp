#include "GameObjectManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

void GameObjectManager::addObject(std::unique_ptr<GameObject> object) {
    objects.push_back(std::move(object));
}

void GameObjectManager::update(float deltaTime) {
    for (auto& object : objects) {
        object->update(deltaTime);
    }

    // Check for collisions
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i]->getBounds().intersects(objects[j]->getBounds())) {
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
    objects.clear();
}

} // namespace game
