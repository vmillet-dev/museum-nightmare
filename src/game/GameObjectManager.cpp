#include "GameObjectManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameObjectManager::GameObjectManager() : physicsWorld(std::make_unique<PhysicsWorld>()) {
    spdlog::debug("GameObjectManager initialized with physics world");
}

void GameObjectManager::addObject(std::unique_ptr<GameObject> object) {
    // Initialize physics for the new object
    object->initPhysics(physicsWorld->getWorld());
    objects.push_back(std::move(object));
}

void GameObjectManager::update(float deltaTime) {
    // Update physics world
    physicsWorld->update(deltaTime);

    // Update game objects
    for (auto& object : objects) {
        object->update(deltaTime);
    }
    // Box2D now handles all collision detection and resolution
}

void GameObjectManager::render(sf::RenderWindow& window) {
    for (const auto& object : objects) {
        object->render(window);
    }
}

void GameObjectManager::clear() {
    objects.clear();
    // Physics world will clean up bodies when destroyed
}

void GameObjectManager::createWall(float x, float y, float width, float height) {
    // Create a static body for the wall
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position.x = x;
    bodyDef.position.y = y;

    // Create the body in the physics world
    b2BodyId bodyId = b2CreateBody(physicsWorld->getWorld(), &bodyDef);

    // Create box shape
    b2Polygon boxShape = b2MakeBox(width / 2.0f, height / 2.0f);

    // Create fixture
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 0.0f;  // Static bodies should have zero density
    shapeDef.friction = 0.3f;
    shapeDef.restitution = 0.0f;

    // Add fixture to body
    b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);

    // Create a wall game object
    auto wall = std::make_unique<StaticWall>(bodyId, width, height);
    wall->initPhysics(physicsWorld->getWorld());

    // Add wall to objects list
    objects.push_back(std::move(wall));
}

} // namespace game
