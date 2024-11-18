#pragma once
#include <vector>
#include <memory>
#include "GameObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

class GameObjectManager {
public:
    void addObject(std::unique_ptr<GameObject> object);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void clear();

private:
    std::vector<std::unique_ptr<GameObject>> objects;
};

} // namespace game
