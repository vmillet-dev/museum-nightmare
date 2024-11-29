#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Types.hpp>
#include <memory>

namespace game {

class ParallaxLayer : public GameObject {
public:
    // Constructor for TMX-based layers
    ParallaxLayer(const tmx::Map& map, const tmx::TileLayer& layer,
                 const std::string& layerName, const sf::Vector2f& parallaxFactors);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;

    const sf::Vector2f& getParallaxFactors() const { return parallaxFactors; }
    void setParallaxOffset(const sf::Vector2f& cameraPos);
    const std::string& getName() const { return layerName; }

private:
    const tmx::Map* mapPtr;
    std::string layerName;
    sf::Vector2f parallaxFactors;
    sf::VertexArray vertices;
    sf::Texture tilesetTexture;
    sf::Vector2f initialPosition;
    sf::Vector2f lastCameraPos;

    void loadTileLayer(const tmx::TileLayer& layer);
    void updateVertexPositions(const sf::Vector2f& offset);
};

} // namespace game
