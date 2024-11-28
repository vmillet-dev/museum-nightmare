#ifndef LEVELLOADER_HPP
#define LEVELLOADER_HPP

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "../objects/GameObject.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

class TextureLoadException : public std::runtime_error {
public:
    explicit TextureLoadException(const std::string& msg) : std::runtime_error(msg) {}
};

class LevelLoader {
public:
    LevelLoader(const std::string& mapFilePath, b2WorldId worldId);
    bool loadLevel();
    std::vector<std::unique_ptr<game::GameObject>> getGameObjects();
    std::vector<std::unique_ptr<game::GameObject>> getParallaxLayers();

private:
    tmx::Map map;
    std::string mapFilePath;
    b2WorldId worldId;
    std::vector<std::unique_ptr<game::GameObject>> gameObjects;
    std::vector<std::unique_ptr<game::GameObject>> parallaxLayers;

    // Texture management
    struct TilesetData {
        sf::Texture texture;
        uint32_t firstGID;
        tmx::Vector2u tileSize;
        uint32_t columns;
    };
    std::vector<TilesetData> tilesets;

    bool loadTextures();
    void parseTileLayers();
    void parseObjectGroups();
    void createParallaxLayer(const tmx::Object& object, float depth);

    // Texture utility methods
    const TilesetData& getTilesetForGID(uint32_t gid) const;
    sf::IntRect getTileTextureRect(const TilesetData& tileset, uint32_t gid) const;
    std::string resolveTexturePath(const std::string& texturePath) const;
};

#endif // LEVELLOADER_HPP
