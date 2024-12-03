#include "BackgroundObject.hpp"
#include "../TextureManager.hpp"
#include "../Camera.hpp"
#include <spdlog/spdlog.h>

namespace game {

BackgroundObject::BackgroundObject(float x, float y, float depth, const std::string& texturePath, const Camera& camera)
    : GameObject(x, y)
    , depth(depth)
    , initialPosition(x, y)
    , camera(camera) {

    try {
        const sf::Texture& loadedTexture = TextureManager::getInstance().getTexture(texturePath);
        sprite.setTexture(loadedTexture);

        // Set the origin to the center of the sprite for proper positioning
        sf::Vector2u textureSize = loadedTexture.getSize();
        sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

        // Initial position setup
        sprite.setPosition(initialPosition);
    } catch (const std::runtime_error& e) {
        spdlog::error("Failed to set up background object: {}", e.what());
    }
}

void BackgroundObject::update(float deltaTime) {
    // Get camera position directly from stored reference
    sf::Vector2f cameraCenter = camera.getView().getCenter();

    // Calculate parallax offset using the TMX parallaxx value (stored in depth)
    sf::Vector2f parallaxOffset(
        (cameraCenter.x - initialPosition.x) * (1.0f - depth),
        (cameraCenter.y - initialPosition.y) * (1.0f - depth)
    );

    // Update sprite position with parallax effect
    sprite.setPosition(initialPosition + parallaxOffset);
}

void BackgroundObject::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void BackgroundObject::initPhysics(b2WorldId worldId) {
    // Background objects don't need physics
}

} // namespace game
