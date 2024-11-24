#pragma once
#include <vector>
#include <memory>
#include "objects/GameObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

class GameObjectManager {
public:
    void addObject(std::unique_ptr<GameObject> object);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void clear();

    // Add getter for objects
    const std::vector<std::unique_ptr<GameObject>>& getObjects() const { return objects; }

private:
    std::vector<std::unique_ptr<GameObject>> objects;
};

} // namespace game
