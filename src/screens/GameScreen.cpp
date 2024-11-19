#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include "../game/levels/Level1.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameScreen::GameScreen(Game& game) : game(game) {
    spdlog::info("Initializing game screen");

    // Initialize game objects
    gameObjectManager = std::make_unique<GameObjectManager>();

    // Create player
    auto player = std::make_unique<Player>(400.0f, 300.0f);  // Start at center
    playerPtr = player.get();  // Store raw pointer before moving ownership
    gameObjectManager->addObject(std::move(player));

    // Load level
    Level1::loadLevel(*gameObjectManager);
}

void GameScreen::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        spdlog::info("Opening pause menu");
        ScreenManager::getInstance().pushScreen(std::make_unique<PauseScreen>(game));
    }
}

void GameScreen::update(float deltaTime) {
    auto& inputManager = InputManager::getInstance();

    // Update player movement based on input
    if (playerPtr) {
        sf::Vector2f movement(0.0f, 0.0f);

        if (inputManager.isActionPressed(Action::MoveUp)) {
            movement.y = -1.0f;
            spdlog::debug("GameScreen: MoveUp action detected");
        }
        if (inputManager.isActionPressed(Action::MoveDown)) {
            movement.y = 1.0f;
            spdlog::debug("GameScreen: MoveDown action detected");
        }
        if (inputManager.isActionPressed(Action::MoveLeft)) {
            movement.x = -1.0f;
            spdlog::debug("GameScreen: MoveLeft action detected");
        }
        if (inputManager.isActionPressed(Action::MoveRight)) {
            movement.x = 1.0f;
            spdlog::debug("GameScreen: MoveRight action detected");
        }

        // Normalize movement vector if moving diagonally
        if (movement.x != 0.0f && movement.y != 0.0f) {
            float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement.x /= length;
            movement.y /= length;
            spdlog::debug("Normalized diagonal movement: ({}, {})", movement.x, movement.y);
        }

        if (movement.x != 0.0f || movement.y != 0.0f) {
            sf::Vector2f oldPos = playerPtr->getPosition();
            playerPtr->move(movement.x, movement.y, deltaTime);
            spdlog::debug("GameScreen: Movement command sent: direction({}, {}), from({}, {})",
                         movement.x, movement.y, oldPos.x, oldPos.y);
        }
    }

    // Update all game objects
    gameObjectManager->update(deltaTime);
}

void GameScreen::render(sf::RenderWindow& window) {
    gameObjectManager->render(window);
}

void GameScreen::pause() {
    spdlog::info("Game paused");
}

void GameScreen::resume() {
    spdlog::info("Game resumed");
}

} // namespace game
