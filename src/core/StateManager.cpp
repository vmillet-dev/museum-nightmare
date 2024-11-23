#include "StateManager.hpp"
#include "Game.hpp"
#include "../screens/MainMenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include "../screens/PauseScreen.hpp"
#include <spdlog/spdlog.h>

namespace game {

StateManager::StateManager(Game& game) : game(game), currentState(GameState::MAIN_MENU) {
    spdlog::info("Initializing state manager");
}

void StateManager::transitionTo(GameState newState) {
    spdlog::info("Transitioning from {} to {}", static_cast<int>(currentState), static_cast<int>(newState));

    switch (newState) {
        case GameState::MAIN_MENU:
            // Clear all screens and push main menu
            while (!game.getScreenManager().isEmpty()) {
                game.getScreenManager().popScreen();
            }
            game.getScreenManager().pushScreen(std::make_unique<MainMenuScreen>(game));
            break;

        case GameState::PLAYING:
            if (currentState == GameState::PAUSED) {
                game.getScreenManager().popScreen();  // Return to game screen
            } else {
                game.getScreenManager().pushScreen(std::make_unique<GameScreen>(game));
            }
            break;

        case GameState::PAUSED:
            game.getScreenManager().pushScreen(std::make_unique<PauseScreen>(game));
            break;
    }

    currentState = newState;
}

} // namespace game
