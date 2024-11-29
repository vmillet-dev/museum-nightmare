#pragma once

#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Types.hpp>
#include <string>
#include <memory>
#include "../GameObjectManager.hpp"

namespace game {

class LevelLoader {
public:
    explicit LevelLoader(const std::string& tmxFile);
    bool loadLevel(GameObjectManager& gameObjectManager);

private:
    tmx::Map map;
    std::string tmxFilePath;
    std::string basePath;  // Store base directory for asset loading

    bool loadParallaxLayers(GameObjectManager& gameObjectManager);
    bool loadTileLayer(const tmx::TileLayer& layer, GameObjectManager& gameObjectManager);
    sf::Vector2f getParallaxFactors(const tmx::Layer& layer) const;
    std::string getLayerProperty(const tmx::Layer& layer, const std::string& propertyName) const;
};

} // namespace game
