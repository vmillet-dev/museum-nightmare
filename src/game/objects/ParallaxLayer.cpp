#include "ParallaxLayer.hpp"
#include <spdlog/spdlog.h>
#include <filesystem>

namespace game {

ParallaxLayer::ParallaxLayer(const tmx::Map& map, const tmx::TileLayer& layer,
                           const std::string& name, const sf::Vector2f& factors)
    : GameObject(0.0f, 0.0f),
      mapPtr(&map),
      layerName(name),
      parallaxFactors(factors),
      vertices(sf::Quads) {
    spdlog::info("Creating ParallaxLayer '{}' with factors ({}, {})",
                 name, factors.x, factors.y);
    loadTileLayer(layer);
}

void ParallaxLayer::update(float deltaTime) {
    // Update is handled through setParallaxOffset when camera moves
}

void ParallaxLayer::render(sf::RenderWindow& window) {
    if (!tilesetTexture.isRepeated()) {
        spdlog::debug("Setting tileset texture as repeatable for layer '{}'", layerName);
        tilesetTexture.setRepeated(true);
    }

    sf::RenderStates states;
    states.texture = &tilesetTexture;
    window.draw(vertices, states);
}

void ParallaxLayer::initPhysics(b2WorldId worldId) {
    // No physics needed for parallax layers
}

void ParallaxLayer::setParallaxOffset(const sf::Vector2f& cameraPos) {
    sf::Vector2f delta = cameraPos - lastCameraPos;
    lastCameraPos = cameraPos;

    // Calculate parallax offset based on factors
    sf::Vector2f offset;
    offset.x = -delta.x * (1.0f - parallaxFactors.x); // Invert the effect for more intuitive behavior
    offset.y = -delta.y * (1.0f - parallaxFactors.y);

    spdlog::debug("Layer '{}': Camera pos ({}, {}), Delta ({}, {}), Factors ({}, {}), Offset ({}, {})",
                 layerName, cameraPos.x, cameraPos.y, delta.x, delta.y,
                 parallaxFactors.x, parallaxFactors.y, offset.x, offset.y);

    updateVertexPositions(offset);
}

void ParallaxLayer::loadTileLayer(const tmx::TileLayer& layer) {
    spdlog::info("Loading tile layer '{}' for parallax effect", layerName);

    const auto& tilesets = mapPtr->getTilesets();
    if (tilesets.empty()) {
        spdlog::error("No tilesets found in the map!");
        return;
    }

    const auto& tileset = tilesets[0];
    spdlog::info("Using tileset: {} (First GID: {})", tileset.getName(), tileset.getFirstGID());

    std::string imagePath = tileset.getImagePath();
    std::filesystem::path tsxPath(imagePath);
    spdlog::info("Original tileset image path: {}", imagePath);

    // Try to find the tileset image relative to the TMX file location and assets directory
    const std::vector<std::string> pathsToTry = {
        imagePath,                                    // Direct path from TSX
        "assets/levels/" + tsxPath.filename().string(), // In assets/levels with filename only
        "../assets/levels/" + tsxPath.filename().string(), // Relative to build directory
        tsxPath.filename().string(),                  // Just the filename in current directory
        "tilesheet.png"                              // Hardcoded fallback for testing
    };

    bool loaded = false;
    for (const auto& path : pathsToTry) {
        spdlog::debug("Trying to load tileset from: {}", path);
        if (tilesetTexture.loadFromFile(path)) {
            spdlog::info("Successfully loaded tileset from: {}", path);
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        spdlog::error("Failed to load tileset texture from any path");
        return;
    }

    const auto& tiles = layer.getTiles();
    const auto& mapSize = mapPtr->getTileCount();
    const auto& tileSize = mapPtr->getTileSize();

    spdlog::info("Map properties - Size: {}x{}, Tile size: {}x{}",
                 mapSize.x, mapSize.y, tileSize.x, tileSize.y);

    std::vector<std::size_t> nonEmptyTileIndices;
    for (std::size_t i = 0; i < tiles.size(); ++i) {
        if (tiles[i].ID != 0) {
            nonEmptyTileIndices.push_back(i);
        }
    }
    spdlog::info("Layer '{}' has {} non-empty tiles out of {}",
                 layerName, nonEmptyTileIndices.size(), tiles.size());

    vertices.resize(nonEmptyTileIndices.size() * 4);
    const int TILESET_COLUMNS = tileset.getColumnCount();
    const auto tilesetTileSize = tileset.getTileSize();

    spdlog::info("Tileset properties - Columns: {}, Tile size: {}x{}",
                 TILESET_COLUMNS, tilesetTileSize.x, tilesetTileSize.y);

    std::size_t currentVertex = 0;
    for (std::size_t tileIndex : nonEmptyTileIndices) {
        std::size_t x = tileIndex % mapSize.x;
        std::size_t y = tileIndex / mapSize.x;
        uint32_t tileId = tiles[tileIndex].ID;

        unsigned int tu = ((tileId - tileset.getFirstGID()) % TILESET_COLUMNS);
        unsigned int tv = ((tileId - tileset.getFirstGID()) / TILESET_COLUMNS);

        sf::Vertex* quad = &vertices[currentVertex];
        currentVertex += 4;

        quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
        quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
        quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
        quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

        quad[0].texCoords = sf::Vector2f(tu * tilesetTileSize.x, tv * tilesetTileSize.y);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tilesetTileSize.x, tv * tilesetTileSize.y);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tilesetTileSize.x, (tv + 1) * tilesetTileSize.y);
        quad[3].texCoords = sf::Vector2f(tu * tilesetTileSize.x, (tv + 1) * tilesetTileSize.y);
    }

    spdlog::info("Successfully created {} vertices for layer '{}'",
                 vertices.getVertexCount(), layerName);

    initialPosition = sf::Vector2f(0, 0);
    lastCameraPos = sf::Vector2f(0, 0);
}

void ParallaxLayer::updateVertexPositions(const sf::Vector2f& offset) {
    for (std::size_t i = 0; i < vertices.getVertexCount(); i++) {
        vertices[i].position += offset;
    }
}

} // namespace game
