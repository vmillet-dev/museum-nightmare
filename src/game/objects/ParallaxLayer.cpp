#include "ParallaxLayer.hpp"
#include <spdlog/spdlog.h>

namespace game {

ParallaxLayer::ParallaxLayer(const tmx::Map& map, const tmx::TileLayer& layer,
                           const std::string& layerName, float parallaxFactor)
    : GameObject(0.0f, 0.0f)
    , mapPtr(&map)
    , layerName(layerName)
    , parallaxFactor(parallaxFactor)
    , vertices(sf::Quads) {

    loadTileLayer(layer);
    initialPosition = position;
    lastCameraPos = sf::Vector2f(0.0f, 0.0f);
}

ParallaxLayer::ParallaxLayer(float parallaxFactor)
    : GameObject(0.0f, 0.0f)
    , mapPtr(nullptr)
    , parallaxFactor(parallaxFactor)
    , vertices(sf::Quads) {

    // Initialize a simple quad for testing
    vertices.resize(4);
    vertices[0].position = sf::Vector2f(0, 0);
    vertices[1].position = sf::Vector2f(100, 0);
    vertices[2].position = sf::Vector2f(100, 100);
    vertices[3].position = sf::Vector2f(0, 100);

    initialPosition = position;
    lastCameraPos = sf::Vector2f(0.0f, 0.0f);
}

void ParallaxLayer::setTexture(const sf::Texture& texture) {
    tilesetTexture = texture;

    // Set up texture coordinates for the test quad
    vertices[0].texCoords = sf::Vector2f(0, 0);
    vertices[1].texCoords = sf::Vector2f(texture.getSize().x, 0);
    vertices[2].texCoords = sf::Vector2f(texture.getSize().x, texture.getSize().y);
    vertices[3].texCoords = sf::Vector2f(0, texture.getSize().y);
}

void ParallaxLayer::loadTileLayer(const tmx::TileLayer& layer) {
    const auto& tilesets = mapPtr->getTilesets();
    if (tilesets.empty()) {
        spdlog::error("No tilesets found in the map!");
        return;
    }

    // Get the first tileset (assuming one tileset per layer for simplicity)
    const auto& tileset = tilesets[0];
    if (!tilesetTexture.loadFromFile(tileset.getImagePath())) {
        spdlog::error("Failed to load tileset texture: {}", tileset.getImagePath());
        return;
    }

    const auto& tiles = layer.getTiles();
    const auto& mapSize = mapPtr->getTileCount();
    const auto& tileSize = mapPtr->getTileSize();

    vertices.resize(mapSize.x * mapSize.y * 4);

    for (std::size_t y = 0; y < mapSize.y; ++y) {
        for (std::size_t x = 0; x < mapSize.x; ++x) {
            const auto tileIndex = y * mapSize.x + x;
            const auto& tile = tiles[tileIndex];

            if (tile.ID == 0) continue; // Skip empty tiles

            // Get the position of this tile in the tileset texture
            unsigned int tu = (tile.ID - tileset.getFirstGID()) % (tileset.getImageSize().x / tileSize.x);
            unsigned int tv = (tile.ID - tileset.getFirstGID()) / (tileset.getImageSize().x / tileSize.x);

            // Get a pointer to the vertices for this tile
            sf::Vertex* quad = &vertices[tileIndex * 4];

            // Define vertices positions
            quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
            quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
            quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

            // Define texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
    }
}

void ParallaxLayer::update(float deltaTime) {
    // No dynamic updates needed for static layers
}

void ParallaxLayer::render(sf::RenderWindow& window) {
    sf::RenderStates states;
    states.texture = &tilesetTexture;
    window.draw(vertices, states);
}

void ParallaxLayer::initPhysics(b2WorldId worldId) {
    // No physics for parallax layers
}

void ParallaxLayer::setParallaxOffset(const sf::Vector2f& cameraPos) {
    sf::Vector2f cameraDelta = cameraPos - lastCameraPos;
    lastCameraPos = cameraPos;

    sf::Vector2f offset;
    float horizontalFactor = std::pow(parallaxFactor, 1.5f);
    offset.x = -cameraPos.x * (1.0f - horizontalFactor) * baseScrollSpeed;

    float verticalFactor = parallaxFactor * 0.25f;
    offset.y = -cameraPos.y * (1.0f - verticalFactor);

    if (parallaxFactor < 0.8f) {
        float floatAmplitude = (1.0f - parallaxFactor) * 5.0f;
        float floatSpeed = 0.5f + (parallaxFactor * 0.5f);
        offset.y += std::sin(cameraPos.x * 0.001f * floatSpeed) * floatAmplitude;

        if (parallaxFactor < 0.4f) {
            offset.y += cameraDelta.x * 0.05f * (1.0f - parallaxFactor);
        }
    }

    updateVertexPositions(offset);
}

void ParallaxLayer::updateVertexPositions(const sf::Vector2f& offset) {
    position = initialPosition + offset;

    if (mapPtr) {
        for (size_t i = 0; i < vertices.getVertexCount(); i += 4) {
            sf::Vector2f basePos = vertices[i].position - position;
            vertices[i].position = basePos + position;
            vertices[i + 1].position = basePos + position + sf::Vector2f(mapPtr->getTileSize().x, 0);
            vertices[i + 2].position = basePos + position + sf::Vector2f(mapPtr->getTileSize().x, mapPtr->getTileSize().y);
            vertices[i + 3].position = basePos + position + sf::Vector2f(0, mapPtr->getTileSize().y);
        }
    } else {
        sf::Vector2f basePos = vertices[0].position - position;
        for (size_t i = 0; i < vertices.getVertexCount(); i++) {
            vertices[i].position = basePos + position;
            if (i == 1 || i == 2) vertices[i].position.x += 100;
            if (i == 2 || i == 3) vertices[i].position.y += 100;
        }
    }
}

} // namespace game
