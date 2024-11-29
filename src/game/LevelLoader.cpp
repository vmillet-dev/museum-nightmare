#include "LevelLoader.hpp"
#include "objects/Wall.hpp"
#include "objects/BackgroundObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

LevelLoader::LevelLoader(const std::string& mapFilePath)
    : mapFilePath(mapFilePath), map(std::make_unique<tmx::Map>()) {}

bool LevelLoader::loadLevel(GameObjectManager& manager, const Camera& camera) {  // Added camera parameter
    if (!map->load(mapFilePath)) {
        spdlog::error("Failed to load map file: {}", mapFilePath);
        return false;
    }

    loadParallaxLayers(manager, camera);  // Pass camera to loadParallaxLayers
    loadTerrainLayers(manager);
    return true;
}

void LevelLoader::loadParallaxLayers(GameObjectManager& manager, const Camera& camera) {  // Added camera parameter
    const auto& layers = map->getLayers();
    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Object) {
            const auto& objectGroup = layer->getLayerAs<tmx::ObjectGroup>();
            std::string groupName = objectGroup.getName();

            // Check if this is a parallax layer by name
            if (groupName.find("-parallax") == std::string::npos) {
                continue;
            }

            float parallaxFactor = 1.0f;
            // Get parallaxx property from the objectGroup
            for (const auto& prop : objectGroup.getProperties()) {
                if (prop.getName() == "parallaxx") {
                    parallaxFactor = prop.getFloatValue();
                    break;
                }
            }

            for (const auto& obj : objectGroup.getObjects()) {
                if (obj.getTileID() != 0) {  // Valid tile GID
                    const auto& tilesets = map->getTilesets();
                    for (const auto& tileset : tilesets) {
                        if (obj.getTileID() >= tileset.getFirstGID() &&
                            obj.getTileID() < tileset.getFirstGID() + tileset.getTileCount()) {
                            const auto* tileData = tileset.getTile(obj.getTileID() - tileset.getFirstGID());
                            if (tileData && !tileData->imagePath.empty()) {
                                manager.addObject(std::make_unique<BackgroundObject>(
                                    obj.getPosition().x, obj.getPosition().y,
                                    parallaxFactor, tileData->imagePath, camera));  // Pass camera reference
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

void LevelLoader::loadTerrainLayers(GameObjectManager& manager) {
    const auto& layers = map->getLayers();
    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();
            const auto& size = map->getTileCount();
            const auto& tileSize = map->getTileSize();

            for (unsigned int y = 0; y < size.y; ++y) {
                for (unsigned int x = 0; x < size.x; ++x) {
                    const auto& tile = tiles[y * size.x + x];
                    if (tile.ID != 0) {  // Non-empty tile
                        if (auto gameObj = createGameObjectFromTile(tileLayer, tile.ID,
                            x * tileSize.x, y * tileSize.y)) {
                            manager.addObject(std::unique_ptr<GameObject>(gameObj));
                        }
                    }
                }
            }
        }
    }
}

GameObject* LevelLoader::createGameObjectFromTile(const tmx::TileLayer& layer, uint32_t gid,
                                                float x, float y) {
    const auto& tilesets = map->getTilesets();
    const auto& tileSize = map->getTileSize();

    for (const auto& tileset : tilesets) {
        if (gid >= tileset.getFirstGID() && gid < tileset.getFirstGID() + tileset.getTileCount()) {
            const auto* tileData = tileset.getTile(gid - tileset.getFirstGID());
            if (!tileData) continue;

            bool isWall = false;
            for (const auto& prop : tileData->properties) {
                if (prop.getName() == "type" && prop.getStringValue() == "wall") {
                    isWall = true;
                    break;
                }
            }

            if (isWall) {
                return new Wall(x, y, tileSize.x, tileSize.y);
            }
            // Add more object types here as needed
        }
    }
    return nullptr;
}

} // namespace game
