#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../input/InputManager.hpp"
#include "../game/levels/Level2.hpp"
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

    // Load level
    Level2::loadLevel(*gameObjectManager);
}

void GameScreen::update(float deltaTime) {
    // TODO for testing purpose
    game.getInputManager().isActionJustPressed(Action::Fire);
    game.getInputManager().isActionReleased(Action::Fire);

    if (game.getInputManager().isActionPressed(Action::Pause)) {
        spdlog::info("Opening pause menu");
        game.getScreenManager().setState(GameState::Paused);
    }

    // Update all game objects
    gameObjectManager->update(deltaTime);
}

void GameScreen::render(sf::RenderWindow& window) {
    gameObjectManager->render(window);
}

void GameScreen::pause() { spdlog::info("Game state: paused"); }

void GameScreen::resume() { spdlog::info("Game state: resumed"); }

} // namespace game
