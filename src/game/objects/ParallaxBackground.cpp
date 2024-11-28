#include "ParallaxBackground.hpp"

namespace game {

ParallaxBackground::ParallaxBackground(const sf::Texture& texture, const sf::Vector2f& position, float depth)
    : GameObject(position.x, position.y)
    , depth(depth)
    , initialPosition(position) {
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void ParallaxBackground::update(float deltaTime) {
    // Update logic handled through position updates from Camera
}

void ParallaxBackground::render(sf::RenderWindow& window) const {  // Made const
    window.draw(sprite);
}

void ParallaxBackground::initPhysics(b2WorldId worldId) {
    // ParallaxBackground doesn't need physics
    this->worldId = worldId;
}

void ParallaxBackground::setViewCenter(const sf::Vector2f& center) {
    // Calculate parallax offset based on view center and depth
    // depth values:
    // 0.01 = furthest background (moves slowest)
    // 0.98 = middle background
    // 0.94 = nearest background (moves fastest)

    // Calculate the parallax offset relative to the initial position
    sf::Vector2f viewOffset = center - initialPosition;
    sf::Vector2f parallaxOffset = viewOffset * depth;

    // Update sprite position with parallax effect
    sprite.setPosition(initialPosition + parallaxOffset);
}

} // namespace game
