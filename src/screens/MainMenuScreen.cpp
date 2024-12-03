#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : Screen(game) {
    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Create buttons with consistent sizing
    const float buttonWidth = 200.f;
    const float buttonHeight = 50.f;

    // Create menu buttons
    playButton_ = menuBuilder_.addButton("Play",
        sf::Vector2f(centerX - buttonWidth/2, startY),
        sf::Vector2f(buttonWidth, buttonHeight));

    settingsButton_ = menuBuilder_.addButton("Settings",
        sf::Vector2f(centerX - buttonWidth/2, startY),
        sf::Vector2f(buttonWidth, buttonHeight));

    testButton_ = menuBuilder_.addButton("Test Widgets",
        sf::Vector2f(centerX - buttonWidth/2, startY),
        sf::Vector2f(buttonWidth, buttonHeight));

    quitButton_ = menuBuilder_.addButton("Quit",
        sf::Vector2f(centerX - buttonWidth/2, startY),
        sf::Vector2f(buttonWidth, buttonHeight));

    // Use layout helper to align buttons vertically with spacing
    menuBuilder_.alignVertically(centerX - buttonWidth/2, 20.f);
}

void MainMenuScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());

    // Handle button clicks
    if (playButton_->isClicked()) {
        spdlog::info("Starting game");
        game.getScreenManager().setState(GameState::Playing);
    }
    else if (settingsButton_->isClicked()) {
        spdlog::info("Opening settings");
        game.getScreenManager().setState(GameState::Settings);
    }
    else if (testButton_->isClicked()) {
        spdlog::info("Opening test screen");
        game.getScreenManager().setState(GameState::Test);
    }
    else if (quitButton_->isClicked()) {
        spdlog::info("Quitting game");
        game.getScreenManager().setState(GameState::Quit);
    }
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    menuBuilder_.render(window);
}

} // namespace game
