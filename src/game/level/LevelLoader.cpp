#include "LevelLoader.hpp"
#include "../objects/ParallaxBackground.hpp"
#include "../objects/FloorObject.hpp"
#include <stdexcept>
#include <filesystem>
#include <spdlog/spdlog.h>

LevelLoader::LevelLoader(const std::string& mapFilePath, b2WorldId worldId)
    : mapFilePath(mapFilePath)
    , worldId(worldId) {
}

bool LevelLoader::loadLevel() {
    try {
        if (!map.load(mapFilePath)) {
            spdlog::error("Failed to load TMX map: {}", mapFilePath);
            return false;
        }

        if (!loadTextures()) {
            spdlog::error("Failed to load textures for map: {}", mapFilePath);
            return false;
        }

        if (!b2World_IsValid(worldId)) {
            spdlog::error("No physics world provided to LevelLoader");
            return false;
        }

        parseObjectGroups(); // Handle parallax backgrounds first
        parseTileLayers();  // Then handle terrain tiles

        return true;
    } catch (const std::exception& e) {
        spdlog::error("Error loading level: {}", e.what());
        return false;
    }
}

bool LevelLoader::loadTextures() {
    const auto& tmxTilesets = map.getTilesets();
    for (const auto& tileset : tmxTilesets) {
        if (tileset.getImagePath().empty()) {
            continue;
        }

        TilesetData data;
        data.firstGID = tileset.getFirstGID();
        data.tileSize = tileset.getTileSize();
        data.columns = tileset.getColumnCount();

        std::string texturePath = resolveTexturePath(tileset.getImagePath());
        if (!data.texture.loadFromFile(texturePath)) {
            spdlog::error("Failed to load texture: {}", texturePath);
            return false;
        }

        tilesets.push_back(std::move(data));
    }
    return !tilesets.empty();
}

std::string LevelLoader::resolveTexturePath(const std::string& texturePath) const {
    std::filesystem::path mapDir = std::filesystem::path(mapFilePath).parent_path();
    return (mapDir / texturePath).string();
}

const LevelLoader::TilesetData& LevelLoader::getTilesetForGID(uint32_t gid) const {
    // Find the tileset that contains this gid
    auto it = std::find_if(tilesets.rbegin(), tilesets.rend(),
        [gid](const TilesetData& tileset) {
            return tileset.firstGID <= gid;
        });

    if (it == tilesets.rend()) {
        throw TextureLoadException("No tileset found for GID: " + std::to_string(gid));
    }

    return *it;
}

sf::IntRect LevelLoader::getTileTextureRect(const TilesetData& tileset, uint32_t gid) const {
    const uint32_t localId = gid - tileset.firstGID;
    const int x = (localId % tileset.columns) * tileset.tileSize.x;
    const int y = (localId / tileset.columns) * tileset.tileSize.y;
    return sf::IntRect(x, y, tileset.tileSize.x, tileset.tileSize.y);
}

void LevelLoader::parseObjectGroups() {
    for (const auto& layer : map.getLayers()) {
        if (layer->getType() == tmx::Layer::Type::Object) {
            auto objectGroup = static_cast<const tmx::ObjectGroup*>(layer.get());

            float parallaxFactor = 1.0f;
            const auto& properties = objectGroup->getProperties();
            for (const auto& prop : properties) {
                if (prop.getName() == "parallaxx") {
                    parallaxFactor = prop.getFloatValue();
                    break;
                }
            }

            if (parallaxFactor != 0.01f && parallaxFactor != 0.98f && parallaxFactor != 0.94f) {
                continue;
            }

            for (const auto& object : objectGroup->getObjects()) {
                const auto gid = object.getTileID();
                if (gid == 0) continue;

                try {
                    const auto& tilesetData = getTilesetForGID(gid);
                    sf::Vector2f position(object.getPosition().x, object.getPosition().y);

                    auto background = std::make_unique<game::ParallaxBackground>(
                        tilesetData.texture,
                        position,
                        parallaxFactor
                    );

                    parallaxLayers.push_back(std::move(background));
                    spdlog::info("Created parallax background with factor: {}", parallaxFactor);
                } catch (const TextureLoadException& e) {
                    spdlog::error("Failed to create parallax background: {}", e.what());
                }
            }
        }
    }

    if (parallaxLayers.size() != 3) {
        spdlog::error("Expected exactly 3 parallax layers (0.01, 0.98, 0.94), found {}", parallaxLayers.size());
    }
}

void LevelLoader::parseTileLayers() {
    const auto& layers = map.getLayers();
    const auto tileSize = map.getTileSize();

    for (const auto& layer : layers) {
        if (layer->getType() != tmx::Layer::Type::Tile) {
            continue;
        }

        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        const auto& tiles = tileLayer.getTiles();

        for (std::size_t y = 0; y < tileLayer.getSize().y; ++y) {
            for (std::size_t x = 0; x < tileLayer.getSize().x; ++x) {
                const auto tileIndex = x + y * tileLayer.getSize().x;
                const auto gid = tiles[tileIndex].ID;

                if (gid == 0) {
                    continue;
                }

                try {
                    const auto& tilesetData = getTilesetForGID(gid);
                    sf::Vector2f position(x * tileSize.x, y * tileSize.y);
                    sf::IntRect textureRect = getTileTextureRect(tilesetData, gid);

                    auto floorObject = std::make_unique<game::FloorObject>(
                        tilesetData.texture,
                        textureRect,
                        position,
                        worldId
                    );

                    gameObjects.push_back(std::move(floorObject));
                } catch (const TextureLoadException& e) {
                    spdlog::error("Failed to create floor object at ({}, {}): {}", x, y, e.what());
                }
            }
        }
    }
}

void LevelLoader::createParallaxLayer(const tmx::Object& object, float depth) {
    if (object.getTileID() == 0) {
        return;
    }

    try {
        const auto& tilesetData = getTilesetForGID(object.getTileID());
        sf::Vector2f position(object.getPosition().x, object.getPosition().y);

        auto background = std::make_unique<game::ParallaxBackground>(
            tilesetData.texture,
            position,
            depth
        );

        parallaxLayers.push_back(std::move(background));
    } catch (const TextureLoadException& e) {
        spdlog::error("Failed to create parallax layer: {}", e.what());
    }
}

std::vector<std::unique_ptr<game::GameObject>> LevelLoader::getGameObjects() {
    return std::move(gameObjects);
}

std::vector<std::unique_ptr<game::GameObject>> LevelLoader::getParallaxLayers() {
    return std::move(parallaxLayers);
}
