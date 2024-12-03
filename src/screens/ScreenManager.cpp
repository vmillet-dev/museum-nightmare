#include "ScreenManager.hpp"
#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "PauseScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

ScreenManager::ScreenManager(Game& game) : game(game) {
    registerScreen<MainMenuScreen>(GameState::MainMenu);
    registerScreen<GameScreen>(GameState::Playing);
    registerScreen<PauseScreen>(GameState::Paused);

    currentScreen = screens[GameState::MainMenu].get(); //TODO
    spdlog::info("ScreenManager initialized with MainMenu screen");
}

void ScreenManager::setState(GameState newState) {
    spdlog::debug("Attempting to set state from {} to {}",
        static_cast<int>(currentState), static_cast<int>(newState));

    if (currentState == newState) {
        spdlog::debug("State unchanged, already in state {}", static_cast<int>(currentState));
        return;
    }

    if (newState == GameState::Quit) {
        spdlog::info("Quitting game from state {}", static_cast<int>(currentState));
        game.quit();
        return;
    }

    auto it = screens.find(newState);
    if (it != screens.end()) {
        currentScreen = it->second.get();
        currentState = newState;
        spdlog::info("Screen transition successful: {} -> {}",
            static_cast<int>(currentState), static_cast<int>(newState));
    } else {
        spdlog::error("Screen transition failed: Invalid state {} requested from state {}",
            static_cast<int>(newState), static_cast<int>(currentState));
    }
}

void ScreenManager::update(float deltaTime) {
    if (currentScreen) {
        currentScreen->update(deltaTime);
    }
}

void ScreenManager::render(sf::RenderWindow& window) {
    if (currentScreen) {
        currentScreen->render(window);
    }
}

} // namespace game
