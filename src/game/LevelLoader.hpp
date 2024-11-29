#pragma once
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <memory>
#include <string>
#include "GameObjectManager.hpp"
#include "Camera.hpp"  // Added back Camera include

namespace game {

class LevelLoader {
public:
    LevelLoader(const std::string& mapFilePath);
    bool loadLevel(GameObjectManager& manager, const Camera& camera);  // Added camera parameter back

private:
    std::string mapFilePath;
    std::unique_ptr<tmx::Map> map;

    void loadParallaxLayers(GameObjectManager& manager, const Camera& camera);  // Added camera parameter back
    void loadTerrainLayers(GameObjectManager& manager);
    GameObject* createGameObjectFromTile(const tmx::TileLayer& layer, uint32_t gid,
                                      float x, float y);
};

} // namespace game
