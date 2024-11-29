#include "LevelLoader.hpp"
#include "objects/Wall.hpp"
#include "objects/BackgroundObject.hpp"
#include <spdlog/spdlog.h>

namespace game {

LevelLoader::LevelLoader(const std::string& mapFilePath)
    : mapFilePath(mapFilePath), map(std::make_unique<tmx::Map>()) {}

bool LevelLoader::loadLevel(GameObjectManager& manager, Camera& camera) {
    if (!map->load(mapFilePath)) {
        spdlog::error("Failed to load map file: {}", mapFilePath);
        return false;
    }

    loadParallaxLayers(manager, camera);
    loadTerrainLayers(manager);
    return true;
}

void LevelLoader::loadParallaxLayers(GameObjectManager& manager, Camera& camera) {
    const auto& layers = map->getLayers();
    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Object) {
            const auto& objectGroup = layer->getLayerAs<tmx::ObjectGroup>();
            if (objectGroup.getName() == "objectgroup") {
                for (const auto& obj : objectGroup.getObjects()) {
                    if (obj.getTileID() != 0) {  // Valid tile GID
                        const auto& tilesets = map->getTilesets();
                        for (const auto& tileset : tilesets) {
                            if (obj.getTileID() >= tileset.getFirstGID() &&
                                obj.getTileID() < tileset.getFirstGID() + tileset.getTileCount()) {
                                const auto* tileData = tileset.getTile(obj.getTileID() - tileset.getFirstGID());
                                if (tileData && !tileData->imagePath.empty()) {
                                    float depth = 1.0f;
                                    for (const auto& prop : obj.getProperties()) {
                                        if (prop.getName() == "depth") {
                                            depth = prop.getFloatValue();
                                            break;
                                        }
                                    }
                                    setupParallaxCamera(camera, depth);
                                    manager.addObject(std::make_unique<BackgroundObject>(
                                        obj.getPosition().x, obj.getPosition().y,
                                        depth, tileData->imagePath, camera));
                                }
                                break;
                            }
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

void LevelLoader::setupParallaxCamera(Camera& camera, float depth) {
    float parallaxFactor;
    if (depth <= 0.01f) {        // Back layer
        parallaxFactor = 0.01f;
    } else if (depth <= 0.94f) { // Front layer
        parallaxFactor = 0.94f;
    } else {                     // Mid layer
        parallaxFactor = 0.98f;
    }
    camera.setParallaxFactor(parallaxFactor);
}

} // namespace game
