#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/LayerGroup.hpp>
#include <SFML/Graphics.hpp>
#include "../GameObjectManager.hpp"
#include "ParallaxLayer.hpp"  // Changed: Include instead of forward declare
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace game {

class Level {
public:
    Level(const std::string& tmxFilePath, GameObjectManager& manager);
    ~Level() = default;

    bool load();
    void update(float deltaTime, const sf::View& view);
    void render(sf::RenderWindow& window, const sf::View& view);

private:
    tmx::Map map;
    GameObjectManager& gameObjectManager;
    std::string tmxPath;

    // Texture cache to avoid reloading
    std::unordered_map<std::string, sf::Texture> textureCache;

    // Layer storage using VertexArrays for efficient rendering
    sf::VertexArray backgroundVertices;
    sf::VertexArray terrainVertices;
    std::vector<std::unique_ptr<ParallaxLayer>> parallaxLayers;

    // Helper methods
    void loadTileLayers();
    void loadParallaxLayers();
    sf::Texture& getTilesetTexture(const tmx::Tileset& tileset);
    void createTileQuad(sf::VertexArray& vertices, const tmx::TileLayer& layer,
                       const tmx::Vector2u& tilePos, const tmx::Tileset& tileset,
                       uint32_t tileId);
    void setupCollision(const tmx::Vector2u& tilePos, const tmx::Vector2u& tileSize);
};

} // namespace game
