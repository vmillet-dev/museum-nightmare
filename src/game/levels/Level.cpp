#include "Level.hpp"
#include "ParallaxLayer.hpp"
#include <spdlog/spdlog.h>

namespace game {

Level::Level(const std::string& tmxFilePath, GameObjectManager& manager)
    : gameObjectManager(manager)
    , tmxPath(tmxFilePath)
    , backgroundVertices(sf::Quads)
    , terrainVertices(sf::Quads)
{
}

bool Level::load() {
    if (!map.load(tmxPath)) {
        spdlog::error("Failed to load TMX file: {}", tmxPath);
        return false;
    }

    loadTileLayers();
    loadParallaxLayers();
    return true;
}

void Level::update(float deltaTime, const sf::View& view) {
    // Update parallax layers based on view position
    for (auto& layer : parallaxLayers) {
        layer->update(view);
    }
}

void Level::render(sf::RenderWindow& window, const sf::View& view) {
    // Render parallax backgrounds first
    for (const auto& layer : parallaxLayers) {
        layer->render(window);
    }

    // Get current texture for the layers
    const auto& tilesets = map.getTilesets();
    if (!tilesets.empty()) {
        sf::RenderStates states;
        states.texture = &getTilesetTexture(tilesets[0]);

        // Render background tiles
        window.draw(backgroundVertices, states);

        // Render terrain tiles
        window.draw(terrainVertices, states);
    }
}

sf::Texture& Level::getTilesetTexture(const tmx::Tileset& tileset) {
    const std::string& texturePath = tileset.getImagePath();

    // Check if texture is already loaded
    auto it = textureCache.find(texturePath);
    if (it != textureCache.end()) {
        return it->second;
    }

    // Load new texture
    sf::Texture& texture = textureCache[texturePath];
    if (!texture.loadFromFile(texturePath)) {
        spdlog::error("Failed to load tileset texture: {}", texturePath);
    }
    return texture;
}

void Level::loadTileLayers() {
    const auto& layers = map.getLayers();
    for (const auto& layer : layers) {
        if (layer->getType() != tmx::Layer::Type::Tile) {
            continue;
        }

        const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        const auto& tiles = tileLayer.getTiles();
        const auto layerSize = tileLayer.getSize();
        const auto& tileSize = map.getTileSize();

        // Choose the target vertex array based on layer name
        sf::VertexArray& targetArray = (layer->getName() == "background") ?
            backgroundVertices : terrainVertices;

        for (unsigned int y = 0; y < layerSize.y; ++y) {
            for (unsigned int x = 0; x < layerSize.x; ++x) {
                const auto tileIndex = x + y * layerSize.x;
                const auto& tile = tiles[tileIndex];

                if (tile.ID == 0) {
                    continue; // Skip empty tiles
                }

                // Find the tileset for this tile
                const auto& tilesets = map.getTilesets();
                const tmx::Tileset* tileset = nullptr;

                for (const auto& ts : tilesets) {
                    if (tile.ID >= ts.getFirstGID() && tile.ID < ts.getFirstGID() + ts.getTileCount()) {
                        tileset = &ts;
                        break;
                    }
                }

                if (!tileset) {
                    continue;
                }

                createTileQuad(targetArray, tileLayer, {x, y}, *tileset, tile.ID);

                // Setup collision for terrain layer
                if (layer->getName() == "terrain") {
                    setupCollision({x, y}, tileSize);
                }
            }
        }
    }
}

void Level::createTileQuad(sf::VertexArray& vertices, const tmx::TileLayer& layer,
                          const tmx::Vector2u& tilePos, const tmx::Tileset& tileset,
                          uint32_t tileId) {
    // Calculate world position
    float worldX = tilePos.x * map.getTileSize().x;
    float worldY = tilePos.y * map.getTileSize().y;

    // Calculate texture coordinates
    uint32_t localId = tileId - tileset.getFirstGID();
    uint32_t tu = localId % (tileset.getImageSize().x / tileset.getTileSize().x);
    uint32_t tv = localId / (tileset.getImageSize().x / tileset.getTileSize().x);

    float texLeft = tu * tileset.getTileSize().x;
    float texTop = tv * tileset.getTileSize().y;
    float texRight = texLeft + tileset.getTileSize().x;
    float texBottom = texTop + tileset.getTileSize().y;

    // Add quad vertices
    vertices.append(sf::Vertex(
        sf::Vector2f(worldX, worldY),
        sf::Vector2f(texLeft, texTop)));
    vertices.append(sf::Vertex(
        sf::Vector2f(worldX + tileset.getTileSize().x, worldY),
        sf::Vector2f(texRight, texTop)));
    vertices.append(sf::Vertex(
        sf::Vector2f(worldX + tileset.getTileSize().x, worldY + tileset.getTileSize().y),
        sf::Vector2f(texRight, texBottom)));
    vertices.append(sf::Vertex(
        sf::Vector2f(worldX, worldY + tileset.getTileSize().y),
        sf::Vector2f(texLeft, texBottom)));
}

void Level::setupCollision(const tmx::Vector2u& tilePos, const tmx::Vector2u& tileSize) {
    // Create a static body for collision using Box2D through GameObjectManager
    // Position is in pixels, convert to meters for Box2D (1m = 32px)
    float posX = (tilePos.x * tileSize.x + tileSize.x / 2.0f) / 32.0f;
    float posY = (tilePos.y * tileSize.y + tileSize.y / 2.0f) / 32.0f;
    float width = tileSize.x / 32.0f;
    float height = tileSize.y / 32.0f;

    gameObjectManager.createWall(posX, posY, width, height);
}

void Level::loadParallaxLayers() {
    const std::vector<std::pair<std::string, float>> bgLayers = {
        {"assets/tiles/FreeCuteTileset/BG1.png", 0.01f},  // Furthest background
        {"assets/tiles/FreeCuteTileset/BG2.png", 0.98f},  // Middle background
        {"assets/tiles/FreeCuteTileset/BG3.png", 0.94f}   // Nearest background
    };

    for (const auto& [path, factor] : bgLayers) {
        auto layer = std::make_unique<ParallaxLayer>(path, factor);
        parallaxLayers.push_back(std::move(layer));
    }
}

} // namespace game
