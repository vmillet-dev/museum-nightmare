#include "ParallaxLayer.hpp"
#include <spdlog/spdlog.h>
#include <cmath>

namespace game {

ParallaxLayer::ParallaxLayer(const std::string& texturePath, float factor)
    : parallaxFactor(factor)
    , initialPosition(0.f, 0.f)
{
    if (!texture.loadFromFile(texturePath)) {
        spdlog::error("Failed to load parallax texture: {}", texturePath);
        return;
    }

    texture.setRepeated(true);  // Enable texture repetition
    sprite.setTexture(texture);

    // Set the sprite to cover the entire view area
    sprite.setScale(2.0f, 2.0f);  // Scale up the 320x192 background
    initialPosition = sprite.getPosition();
}

void ParallaxLayer::update(const sf::View& view) {
    // Calculate new position based on view center and parallax factor
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    // Calculate offset with wrapping for infinite scrolling
    float offsetX = std::fmod((viewCenter.x - initialPosition.x) * parallaxFactor, texture.getSize().x * 2.0f);
    float offsetY = std::fmod((viewCenter.y - initialPosition.y) * parallaxFactor, texture.getSize().y * 2.0f);

    // Position the sprite relative to the view
    sprite.setPosition(viewCenter - viewSize / 2.0f + sf::Vector2f(offsetX, offsetY));
}

void ParallaxLayer::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

} // namespace game
