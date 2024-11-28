#include "LevelLoader.hpp"
#include <spdlog/spdlog.h>
#include <algorithm>

namespace game {

LevelLoader::LevelLoader(const std::string& filePath) : filePath(filePath) {}

bool LevelLoader::loadLevel(GameObjectManager& gameObjectManager) {
    if (!map.load(filePath)) {
        spdlog::error("Failed to load TMX file: {}", filePath);
        return false;
    }

    loadParallaxLayers(gameObjectManager);
    loadObjects(gameObjectManager);
    return true;
}

void LevelLoader::loadParallaxLayers(GameObjectManager& gameObjectManager) {
    const auto& layers = map.getLayers();

    // Sort layers by parallax factor to ensure correct rendering order (background to foreground)
    std::vector<std::pair<const tmx::TileLayer*, float>> sortedLayers;

    for (const auto& layer : layers) {
        if (layer->getType() != tmx::Layer::Type::Tile) {
            continue;
        }

        const auto* tileLayer = dynamic_cast<const tmx::TileLayer*>(layer.get());
        if (!tileLayer) {
            continue;
        }

        // Get parallax factors from layer properties
        float parallaxFactor = 1.0f;
        const auto& properties = layer->getProperties();
        for (const auto& prop : properties) {
            if (prop.getName() == "parallaxFactor" && prop.getType() == tmx::Property::Type::Float) {
                parallaxFactor = prop.getFloatValue();
                break;
            }
        }

        sortedLayers.emplace_back(tileLayer, parallaxFactor);
    }

    // Sort layers by parallax factor (ascending - background first)
    std::sort(sortedLayers.begin(), sortedLayers.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    // Create parallax layers in sorted order
    for (const auto& [tileLayer, parallaxFactor] : sortedLayers) {
        auto parallaxLayer = std::make_unique<ParallaxLayer>(
            map, *tileLayer, tileLayer->getName(), parallaxFactor);
        gameObjectManager.addObject(std::move(parallaxLayer));
    }
}

void LevelLoader::loadObjects(GameObjectManager& gameObjectManager) {
    for (const auto& layer : map.getLayers()) {
        if (layer->getType() != tmx::Layer::Type::Object) {
            continue;
        }

        const auto* objectGroup = dynamic_cast<const tmx::ObjectGroup*>(layer.get());
        if (!objectGroup) {
            continue;
        }

        for (const auto& object : objectGroup->getObjects()) {
            createGameObject(gameObjectManager, object, object.getType());
        }
    }
}

void LevelLoader::createGameObject(GameObjectManager& gameObjectManager,
                                 const tmx::Object& object,
                                 const std::string& type) {
    const auto& position = object.getPosition();
    const auto& size = object.getAABB();

    // Convert TMX objects to game objects based on type
    if (type == "Wall") {
        auto wall = std::make_unique<Wall>(position.x, position.y, size.width, size.height);
        gameObjectManager.addObject(std::move(wall));
    }
    else if (type == "Enemy") {
        auto enemy = std::make_unique<Enemy>(position.x, position.y);
        gameObjectManager.addObject(std::move(enemy));
    }
    else {
        spdlog::warn("Unknown object type in TMX file: {}", type);
    }
}

} // namespace game
