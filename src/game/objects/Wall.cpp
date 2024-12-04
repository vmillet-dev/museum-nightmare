#include "Wall.hpp"
#include "../../core/const.hpp"
#include "../TextureManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

Wall::Wall(float x, float y, float width, float height,
           const std::string& texturePath, const sf::IntRect& textureRect)
    : GameObject(x, y), width(width), height(height) {
    try {
        const sf::Texture& texture = TextureManager::getInstance().getTexture(texturePath);
        sprite.setTexture(texture);
        sprite.setTextureRect(textureRect);
        sprite.setOrigin(width / 2, height / 2);
        sprite.setPosition(position);
        // Set scale to match the desired tile size while preserving texture rect proportions
        float scaleX = width / static_cast<float>(textureRect.width);
        float scaleY = height / static_cast<float>(textureRect.height);
        sprite.setScale(scaleX, scaleY);
    } catch (const std::runtime_error& e) {
        spdlog::error("Failed to load wall texture: {}", e.what());
    }
    spdlog::debug("Wall created: pos({:.1f},{:.1f}) size({:.1f},{:.1f}) texRect({},{},{},{})",
                  x, y, width, height, textureRect.left, textureRect.top,
                  textureRect.width, textureRect.height);
}

void Wall::update(float deltaTime) {
    // Walls are static, no update needed
    // Position is managed by Box2D
    if (b2Body_IsValid(bodyId)) {
        syncPositionWithPhysics();
        sprite.setPosition(position);
    }
}

void Wall::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Wall::initPhysics(b2WorldId worldId) {
    this->worldId = worldId;

    // Create body definition
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;  // Walls are static
    bodyDef.position = {position.x, position.y};

    // Create the body
    bodyId = b2CreateBody(worldId, &bodyDef);

    // Create shape definition
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.friction = Constants::Physics::WALL_FRICTION;

    // Create box polygon
    float hx = width / 2.0f;
    float hy = height / 2.0f;
    b2Polygon boxShape = b2MakeBox(hx, hy);

    // Create the fixture
    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    spdlog::debug("Wall physics initialized at ({}, {})", position.x, position.y);
}

} // namespace game
