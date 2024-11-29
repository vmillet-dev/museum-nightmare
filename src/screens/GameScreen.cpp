#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include "../game/levels/LevelLoader.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameScreen::GameScreen(Game& game) : game(game) {
    spdlog::info("Initializing game screen");
    gameObjectManager = std::make_unique<GameObjectManager>();

    // Set up the initial view with a fixed size for consistent parallax testing
    sf::View view(sf::FloatRect(0, 0, 800.f, 600.f));
    view.setCenter(cameraPosition);
    game.getWindow().setView(view);

    spdlog::info("View initialized with size {}x{} at position ({}, {})",
                 view.getSize().x, view.getSize().y, cameraPosition.x, cameraPosition.y);
}

bool GameScreen::loadLevel(const std::string& levelPath) {
    spdlog::info("Loading level from: {}", levelPath);
    levelLoader = std::make_unique<LevelLoader>(levelPath);
    return levelLoader->loadLevel(*gameObjectManager);
}

void GameScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Escape:
                spdlog::info("Opening pause menu");
                ScreenManager::getInstance().pushScreen(std::make_unique<PauseScreen>(game));
                break;
            default:
                break;
        }
    }
}

void GameScreen::update(float deltaTime) {
    // Update camera position based on keyboard input for testing parallax
    const float CAMERA_SPEED = 300.0f;
    sf::Vector2f oldPosition = cameraPosition;

    // Direct keyboard state check for testing
    bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
    bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

    // For testing in headless mode, simulate keyboard input based on time
    static float testTimer = 0.0f;
    testTimer += deltaTime;
    if (testTimer < 2.0f) {
        rightPressed = true;  // Move right for 2 seconds
    } else if (testTimer < 4.0f) {
        leftPressed = true;   // Move left for 2 seconds
    } else {
        testTimer = 0.0f;     // Reset cycle
    }

    spdlog::info("Keyboard state - Left (Q): {}, Right (D): {}", leftPressed, rightPressed);

    if (leftPressed) { // Left
        cameraPosition.x -= CAMERA_SPEED * deltaTime;
        spdlog::info("Camera moving left: old=({}, {}), new=({}, {}), delta={}, dt={}",
                    oldPosition.x, oldPosition.y,
                    cameraPosition.x, cameraPosition.y,
                    cameraPosition.x - oldPosition.x,
                    deltaTime);
    }
    if (rightPressed) { // Right
        cameraPosition.x += CAMERA_SPEED * deltaTime;
        spdlog::info("Camera moving right: old=({}, {}), new=({}, {}), delta={}, dt={}",
                    oldPosition.x, oldPosition.y,
                    cameraPosition.x, cameraPosition.y,
                    cameraPosition.x - oldPosition.x,
                    deltaTime);
    }

    // Update view center with new camera position
    sf::View view = game.getWindow().getView();
    view.setCenter(cameraPosition);
    game.getWindow().setView(view);

    spdlog::info("GameScreen::update - Camera position: ({}, {}), dt: {}",
                cameraPosition.x, cameraPosition.y, deltaTime);
    gameObjectManager->update(deltaTime, cameraPosition);
}

void GameScreen::updateCamera() {
    // Create a view for the camera
    sf::View view = game.getWindow().getView();
    view.setCenter(cameraPosition);
    game.getWindow().setView(view);
}

void GameScreen::render(sf::RenderWindow& window) {
    gameObjectManager->render(window);
}

void GameScreen::pause() {
    spdlog::info("Game state: paused");
}

void GameScreen::resume() {
    spdlog::info("Game state: resumed");
}

} // namespace game
