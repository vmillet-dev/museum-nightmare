#include "LevelLoader.hpp"
#include "../objects/ParallaxLayer.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>

namespace game {

LevelLoader::LevelLoader(const std::string& tmxFile) : tmxFilePath(tmxFile) {
    spdlog::info("Initializing LevelLoader with file: {}", tmxFile);

    std::filesystem::path mapPath(tmxFile);
    auto baseDir = mapPath.parent_path();
    spdlog::info("Base directory for assets: {}", baseDir.string());

    // Store the base directory for later use with relative paths
    basePath = baseDir.string();

    if (!map.load(mapPath.string())) {
        spdlog::error("Failed to load TMX file: {}", tmxFile);
        return;
    }

    const auto& tilesets = map.getTilesets();
    spdlog::info("Successfully loaded TMX file: {} with {} tilesets", tmxFile, tilesets.size());

    // Log detailed tileset information
    for (const auto& tileset : tilesets) {
        spdlog::info("Tileset: {} (First GID: {})", tileset.getName(), tileset.getFirstGID());
        spdlog::info("Image path: {}", tileset.getImagePath());
        spdlog::info("Tile dimensions: {}x{}", tileset.getTileSize().x, tileset.getTileSize().y);
        spdlog::info("Total tiles: {}, Columns: {}", tileset.getTileCount(), tileset.getColumnCount());
    }
}

bool LevelLoader::loadLevel(GameObjectManager& gameObjectManager) {
    spdlog::info("Starting level loading process");

    const auto& tilesets = map.getTilesets();
    if (tilesets.empty()) {
        spdlog::error("No tilesets found in the map!");
        return false;
    }

    spdlog::info("Loading parallax layers...");
    if (!loadParallaxLayers(gameObjectManager)) {
        spdlog::error("Failed to load parallax layers");
        return false;
    }

    return true;
}

bool LevelLoader::loadParallaxLayers(GameObjectManager& gameObjectManager) {
    const std::vector<std::string> layerOrder = {"Background", "Middle", "Foreground"};
    const auto& layers = map.getLayers();
    spdlog::info("Loading {} layers from TMX file", layers.size());

    // Store layer properties for debugging
    for (const auto& layer : layers) {
        spdlog::info("Found layer: {} (Type: {})",
                     layer->getName(),
                     layer->getType() == tmx::Layer::Type::Tile ? "Tile" : "Other");

        if (layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();
            auto parallaxFactors = getParallaxFactors(*layer);

            // Count non-empty tiles
            size_t nonEmptyTiles = 0;
            for (const auto& tile : tiles) {
                if (tile.ID != 0) nonEmptyTiles++;
            }

            spdlog::info("Layer '{}' properties:", layer->getName());
            spdlog::info("- Size: {}x{}", tileLayer.getSize().x, tileLayer.getSize().y);
            spdlog::info("- Parallax factors: ({}, {})", parallaxFactors.x, parallaxFactors.y);
            spdlog::info("- Total tiles: {}", tiles.size());
            spdlog::info("- Non-empty tiles: {}", nonEmptyTiles);
            spdlog::info("- Offset: ({}, {})", layer->getOffset().x, layer->getOffset().y);
            spdlog::info("- Opacity: {}", layer->getOpacity());

            // Log tile IDs for verification
            std::string tileIds;
            for (size_t i = 0; i < std::min(size_t(10), tiles.size()); ++i) {
                if (tiles[i].ID != 0) {
                    tileIds += std::to_string(tiles[i].ID) + " ";
                }
            }
            spdlog::info("- First few tile IDs: {}", tileIds);
        }
    }

    // Load layers in specific order
    for (const auto& layerName : layerOrder) {
        bool layerFound = false;
        for (const auto& layer : layers) {
            if (layer->getType() == tmx::Layer::Type::Tile && layer->getName() == layerName) {
                spdlog::info("Loading {} layer", layerName);
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                if (!loadTileLayer(tileLayer, gameObjectManager)) {
                    return false;
                }
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            spdlog::warn("Layer '{}' not found in TMX file", layerName);
        }
    }
    return true;
}

bool LevelLoader::loadTileLayer(const tmx::TileLayer& layer, GameObjectManager& gameObjectManager) {
    // Get the tileset information
    const auto& tilesets = map.getTilesets();
    if (tilesets.empty()) {
        spdlog::error("No tilesets available for layer: {}", layer.getName());
        return false;
    }

    // Get tileset information for debugging
    for (const auto& tileset : tilesets) {
        spdlog::debug("Tileset: {} (First GID: {})", tileset.getName(), tileset.getFirstGID());
        spdlog::debug("Image path: {}", tileset.getImagePath());
        spdlog::debug("Tile size: {}x{}", tileset.getTileSize().x, tileset.getTileSize().y);
        spdlog::debug("Tile count: {}", tileset.getTileCount());
        spdlog::debug("Columns: {}", tileset.getColumnCount());
    }

    // Create a new ParallaxLayer with appropriate factors
    auto parallaxFactors = getParallaxFactors(layer);
    spdlog::info("Creating ParallaxLayer '{}' with factors ({}, {})",
                 layer.getName(), parallaxFactors.x, parallaxFactors.y);

    auto parallaxLayer = std::make_unique<ParallaxLayer>(map, layer, layer.getName(), parallaxFactors);
    gameObjectManager.addObject(std::move(parallaxLayer));
    return true;
}

sf::Vector2f LevelLoader::getParallaxFactors(const tmx::Layer& layer) const {
    sf::Vector2f factors(1.0f, 1.0f);
    const std::string& name = layer.getName();

    // Get parallax factors from TMX file
    if (layer.getType() == tmx::Layer::Type::Tile) {
        const auto& tileLayer = layer.getLayerAs<tmx::TileLayer>();
        const auto& offset = layer.getOffset();

        // TMX stores parallax factors in the offset
        if (name == "Background") {
            factors = sf::Vector2f(0.5f, 0.75f);
        } else if (name == "Middle") {
            factors = sf::Vector2f(1.0f, 1.0f);
        } else if (name == "Foreground") {
            factors = sf::Vector2f(2.0f, 2.0f);
        }

        spdlog::debug("Layer '{}' parallax factors from TMX: ({}, {})",
                     name, factors.x, factors.y);
    }

    return factors;
}

std::string LevelLoader::getLayerProperty(const tmx::Layer& layer, const std::string& propertyName) const {
    const auto& properties = layer.getProperties();
    for (const auto& prop : properties) {
        if (prop.getName() == propertyName) {
            return prop.getStringValue();
        }
    }
    return "";
}

} // namespace game
