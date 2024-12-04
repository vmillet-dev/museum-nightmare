#include "ScreenManager.hpp"
#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "PauseScreen.hpp"
#include "SettingsScreen.hpp"
#include "TestScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

ScreenManager::ScreenManager(Game& game) : game(game) {
    registerScreen<MainMenuScreen>(GameState::MainMenu);
    registerScreen<GameScreen>(GameState::Playing);
    registerScreen<PauseScreen>(GameState::Paused);
    registerScreen<SettingsScreen>(GameState::Settings);
    registerScreen<TestScreen>(GameState::Test);

    currentScreen = screens[GameState::MainMenu].get();
    spdlog::info("ScreenManager initialized with MainMenu screen");
}

void ScreenManager::setState(GameState newState) {
    if (currentState == newState) return;

    if (newState == GameState::Quit) {
        game.quit();
        return;
    }

    auto it = screens.find(newState);
    if (it != screens.end()) {
        currentScreen = it->second.get();
        currentState = newState;
        spdlog::info("Screen state changed to: {}", static_cast<int>(newState));
    } else {
        spdlog::error("Attempted to switch to invalid screen state: {}", static_cast<int>(newState));
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
