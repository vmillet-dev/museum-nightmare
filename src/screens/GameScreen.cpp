#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include <spdlog/spdlog.h>

namespace game {

GameScreen::GameScreen(Game& game) : game(game) {
    spdlog::info("Initializing game screen");

    // Initialize game objects
    gameObjectManager = std::make_unique<GameObjectManager>();

    // Create player at a better starting position for the larger level
    auto player = std::make_unique<Player>(100.0f, 100.0f, game);
    playerPtr = player.get();  // Store raw pointer before moving ownership
    gameObjectManager->addObject(std::move(player));

    // Load TMX level
    level = std::make_unique<Level>("assets/maps/stage1.tmx", *gameObjectManager);
    if (!level->load()) {
        spdlog::error("Failed to load level from TMX file");
    }
}

void GameScreen::update(float deltaTime) {
    if (game.getInputManager().isActionPressed(Action::Pause)) {
        spdlog::info("Opening pause menu");
        game.getScreenManager().setState(GameState::Paused);
    }

    // Update level and game objects
    if (level) {
        // Set up view based on player position
        sf::View view = game.getWindow().getDefaultView();
        if (playerPtr) {
            view.setCenter(playerPtr->getPosition());
        }
        level->update(deltaTime, view);
    }

    // Update all game objects
    gameObjectManager->update(deltaTime);
}

void GameScreen::render(sf::RenderWindow& window) {
    // Set up view based on player position
    sf::View view = window.getDefaultView();
    if (playerPtr) {
        view.setCenter(playerPtr->getPosition());
        window.setView(view);
    }

    // Render level first (background and terrain)
    if (level) {
        level->render(window, view);
    }

    // Render game objects (player, enemies, etc.)
    gameObjectManager->render(window);
}

void GameScreen::pause() { spdlog::info("Game state: paused"); }

void GameScreen::resume() { spdlog::info("Game state: resumed"); }

} // namespace game
