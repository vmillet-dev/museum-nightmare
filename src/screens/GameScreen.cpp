#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameScreen::GameScreen(Game& game) : game(game) {
    spdlog::info("Initializing game screen");

    // Initialize game objects and camera
    gameObjectManager = std::make_unique<GameObjectManager>();
    camera = std::make_unique<Camera>(game.getWindow());

    // Initialize physics world
    b2WorldDef worldDef;
    worldDef.gravity = {0.0f, 9.81f};
    worldId = b2CreateWorld(&worldDef);
    if (!b2World_IsValid(worldId)) {  // Updated to use Box2D v3 API
        spdlog::error("Failed to create physics world");
        return;
    }

    // Create player at a better starting position for the larger level
    auto player = std::make_unique<Player>(100.0f, 100.0f, game);
    playerPtr = player.get();  // Store raw pointer before moving ownership
    gameObjectManager->addObject(std::move(player));

    // Set up camera to follow player
    camera->follow(playerPtr);

    // Load level from TMX file
    loadLevel("assets/stage1.tmx");
}

void GameScreen::loadLevel(const std::string& mapFile) {
    LevelLoader levelLoader(mapFile, worldId);
    if (!levelLoader.loadLevel()) {
        spdlog::error("Failed to load level: {}", mapFile);
        return;
    }

    // Get and store parallax layers
    parallaxLayers = levelLoader.getParallaxLayers();

    // Get and add game objects to manager
    auto gameObjects = levelLoader.getGameObjects();
    for (auto& obj : gameObjects) {
        gameObjectManager->addObject(std::move(obj));
    }
}

void GameScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        spdlog::info("Opening pause menu");
        ScreenManager::getInstance().pushScreen(std::make_unique<PauseScreen>(game));
    }
}

void GameScreen::update(float deltaTime) {
    if (b2World_IsValid(worldId)) {  // Updated to use Box2D v3 API
        b2World_Step(worldId, deltaTime, 8);  // Updated to use Box2D v3 API with 3 parameters
    }

    // Update camera and parallax layers
    camera->update(deltaTime);
    camera->updateParallaxLayers(parallaxLayers);

    // Update all game objects
    gameObjectManager->update(deltaTime);
}

void GameScreen::render(sf::RenderWindow& window) {
    // Draw parallax backgrounds first
    for (const auto& layer : parallaxLayers) {
        window.draw(*layer);
    }

    // Draw game objects
    gameObjectManager->render(window);
}

void GameScreen::pause() { spdlog::info("Game state: paused"); }

void GameScreen::resume() { spdlog::info("Game state: resumed"); }

GameScreen::~GameScreen() {
    if (b2World_IsValid(worldId)) {  // Updated to use Box2D v3 API
        b2DestroyWorld(worldId);
    }
}

} // namespace game
