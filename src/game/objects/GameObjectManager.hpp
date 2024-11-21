#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "GameObject.hpp"
#include <spdlog/spdlog.h>
#include <SFML/Graphics.hpp>

namespace game {

class GameObjectManager {
public:
    static GameObjectManager& getInstance() {
        static GameObjectManager instance;
        return instance;
    }

    void addObject(std::unique_ptr<GameObject> object);
    void removeObject(size_t id);
    GameObject* getObject(size_t id);

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void clear();

    // Collision handling
    void checkCollisions();
    std::vector<GameObject*> getObjectsInRange(const sf::FloatRect& bounds);

private:
    GameObjectManager() = default;
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

    std::vector<std::unique_ptr<GameObject>> objects;
    std::unordered_map<size_t, size_t> objectIndex; // Maps ID to vector index
    size_t nextId = 0;

    // Helper methods for collision detection
    bool checkCollision(GameObject* obj1, GameObject* obj2);
    void resolveCollision(GameObject* obj1, GameObject* obj2);
};

} // namespace game
