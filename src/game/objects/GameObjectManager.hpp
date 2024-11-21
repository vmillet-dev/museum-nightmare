#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include "GameObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

enum class ObjectType {
    Player,
    Enemy,
    Wall,
    Door,
    Generic
};

class GameObjectManager {
public:
    static GameObjectManager& getInstance() {
        static GameObjectManager instance;
        return instance;
    }

    void addObject(std::unique_ptr<GameObject> object, ObjectType type);
    void removeObject(size_t id);
    GameObject* getObject(size_t id);
    GameObject* getObjectByType(ObjectType type);
    std::vector<GameObject*> getObjectsByType(ObjectType type);

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void clear();

private:
    GameObjectManager() = default;
    GameObjectManager(const GameObjectManager&) = delete;
    GameObjectManager& operator=(const GameObjectManager&) = delete;

    std::vector<std::unique_ptr<GameObject>> objects;
    std::unordered_map<size_t, ObjectType> objectTypes;
    std::unordered_map<ObjectType, std::vector<size_t>> typeIndex;
    size_t nextId = 0;
};

} // namespace game
