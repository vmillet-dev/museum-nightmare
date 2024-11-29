#pragma once
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <memory>
#include <string>
#include "Camera.hpp"
#include "GameObjectManager.hpp"

namespace game {

class LevelLoader {
public:
    LevelLoader(const std::string& mapFilePath);
    bool loadLevel(GameObjectManager& manager, Camera& camera);

private:
    std::string mapFilePath;
    std::unique_ptr<tmx::Map> map;

    void loadParallaxLayers(GameObjectManager& manager, Camera& camera);
    void loadTerrainLayers(GameObjectManager& manager);
    GameObject* createGameObjectFromTile(const tmx::TileLayer& layer, uint32_t gid,
                                       float x, float y);
    void setupParallaxCamera(Camera& camera, float depth);
};

} // namespace game
