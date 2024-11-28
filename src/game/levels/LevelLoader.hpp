#pragma once
#include <string>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <SFML/Graphics.hpp>
#include "../GameObjectManager.hpp"
#include "../objects/ParallaxLayer.hpp"
#include "../objects/Wall.hpp"
#include "../objects/actor/Enemy.hpp"
#include <spdlog/spdlog.h>

namespace game {

class LevelLoader {
public:
    LevelLoader(const std::string& filePath);
    bool loadLevel(GameObjectManager& gameObjectManager);

private:
    tmx::Map map;
    std::string filePath;

    void loadParallaxLayers(GameObjectManager& gameObjectManager);
    void loadObjects(GameObjectManager& gameObjectManager);
    void createGameObject(GameObjectManager& gameObjectManager,
                         const tmx::Object& object,
                         const std::string& type);
};

} // namespace game
