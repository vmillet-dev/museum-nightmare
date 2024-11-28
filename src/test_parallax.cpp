#include "game/level/LevelLoader.hpp"
#include "game/Camera.hpp"
#include "game/objects/GameObject.hpp"
#include "game/physics/PhysicsConstants.hpp"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <memory>

using namespace game;

class TestObject : public game::GameObject {
public:
    TestObject() : GameObject(400.0f, 300.0f) {
        shape.setSize(sf::Vector2f(32, 32));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(400, 300);
    }

    void update(float deltaTime) override {
        // Handle movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            shape.move(-5, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            shape.move(5, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            shape.move(0, -5);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            shape.move(0, 5);
    }

    void render(sf::RenderWindow& window) const override {
        window.draw(shape);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(shape, states);
    }

    void initPhysics(b2WorldId worldId) override {
        // Simple physics initialization
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.x = shape.getPosition().x / game::PHYSICS_SCALE;
        bodyDef.position.y = shape.getPosition().y / game::PHYSICS_SCALE;
        bodyId = b2CreateBody(worldId, &bodyDef);

        // Create box shape
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        shapeDef.friction = 0.3f;

        // Create a box polygon (half-width and half-height)
        b2Polygon boxShape = b2MakeBox(16.0f / game::PHYSICS_SCALE, 16.0f / game::PHYSICS_SCALE);

        // Create the shape with the polygon
        b2CreatePolygonShape(bodyId, &shapeDef, &boxShape);
    }

    sf::Vector2f getPosition() const {
        return shape.getPosition();
    }

private:
    sf::RectangleShape shape;
    b2BodyId bodyId{b2_nullBodyId};
};

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Parallax Test");
    window.setFramerateLimit(60);

    // Create physics world
    b2WorldDef worldDef;
    worldDef.gravity = {0.0f, 9.81f};
    b2WorldId worldId = b2CreateWorld(&worldDef);
    if (!b2World_IsValid(worldId)) {
        return -1;
    }

    // Load level
    LevelLoader levelLoader("assets/levels/test_parallax.tmx", worldId);
    if (!levelLoader.loadLevel()) {
        return -1;
    }

    // Get game objects and parallax layers
    auto gameObjects = levelLoader.getGameObjects();
    auto parallaxLayers = levelLoader.getParallaxLayers();

    // Create camera
    Camera camera(window);

    // Create test object for movement
    auto player = std::make_unique<TestObject>();
    player->initPhysics(worldId);  // Initialize physics for the player
    camera.follow(player.get());

    // Game loop
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        // Update physics (Box2D v3 only takes 3 parameters)
        b2World_Step(worldId, deltaTime, 8);

        // Update player
        player->update(deltaTime);

        // Update camera
        camera.update(deltaTime);
        camera.updateParallaxLayers(parallaxLayers);

        // Clear window
        window.clear(sf::Color::Black);

        // Draw parallax layers
        for (const auto& layer : parallaxLayers) {
            window.draw(*layer);
        }

        // Draw game objects
        for (const auto& obj : gameObjects) {
            window.draw(*obj);
        }

        // Draw player
        window.draw(*player);

        // Display
        window.display();
    }

    // Cleanup
    b2DestroyWorld(worldId);
    return 0;
}
