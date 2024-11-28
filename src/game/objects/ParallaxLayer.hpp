#pragma once
#include <SFML/Config.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <memory>

namespace game {

class ParallaxLayer : public GameObject {
public:
    // Constructor for TMX-based layers
    ParallaxLayer(const tmx::Map& map, const tmx::TileLayer& layer, const std::string& layerName, float parallaxFactor);

    // Constructor for testing
    explicit ParallaxLayer(float parallaxFactor);

    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void initPhysics(b2WorldId worldId) override;

    float getParallaxFactor() const { return parallaxFactor; }
    void setParallaxOffset(const sf::Vector2f& cameraPos);
    void setBaseScrollSpeed(float speed) { baseScrollSpeed = speed; }

    // Added for testing
    void setTexture(const sf::Texture& texture);

private:
    const tmx::Map* mapPtr;  // Changed to pointer to make it optional
    std::string layerName;
    float parallaxFactor;
    float baseScrollSpeed = 2.0f;  // Base speed for parallax movement
    sf::VertexArray vertices;
    sf::Texture tilesetTexture;
    sf::Vector2f initialPosition;
    sf::Vector2f lastCameraPos;    // Track camera movement for smooth transitions

    void loadTileLayer(const tmx::TileLayer& layer);
    void updateVertexPositions(const sf::Vector2f& offset);
};

} // namespace game
