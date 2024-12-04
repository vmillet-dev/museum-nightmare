#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : Screen(game), menuBuilder_(game) {
    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();

    // Add buttons with automatic vertical spacing
    menuBuilder_.setSpacing(100)
        .addButton("Play", 300, 200,
            [this, &game]() {
                spdlog::debug("Play button clicked - Button state validation");
                spdlog::info("Play button clicked - Attempting to transition to GameState::Playing");
                game.getScreenManager().setState(GameState::Playing);
                spdlog::debug("State transition completed");
            })
        .addButton("Settings", 300, 300,
            [this, &game]() {
                spdlog::info("Settings button clicked - Transitioning to Settings screen");
                game.getScreenManager().setState(GameState::Settings);
            })
        .addButton("Quit", 300, 400,
            [this, &game]() {
                spdlog::info("Quit button clicked - Attempting to transition to GameState::Quit");
                game.getScreenManager().setState(GameState::Quit);
                spdlog::info("Screen transition completed");
            });

    // Store buttons and selected index
    buttons_ = menuBuilder_.build();
    selectedButtonIndex_ = menuBuilder_.getSelectedIndex();
}

void MainMenuScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    menuBuilder_.render(window);
}

} // namespace game
