#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : Screen(game) {
    spdlog::info("Initializing pause screen");

    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Create buttons with consistent sizing
    const float buttonWidth = 200.f;
    const float buttonHeight = 50.f;

    // Create menu buttons using MenuBuilder
    resumeButton = menuBuilder_.addButton("Resume",
        sf::Vector2f(centerX - buttonWidth/2, startY),
        sf::Vector2f(buttonWidth, buttonHeight));

    mainMenuButton = menuBuilder_.addButton("Main Menu",
        sf::Vector2f(centerX - buttonWidth/2, startY),
        sf::Vector2f(buttonWidth, buttonHeight));

    // Use layout helper to align buttons vertically with spacing
    menuBuilder_.alignVertically(centerX - buttonWidth/2, 20.f);

    // Load font for pause text
    if (!font.loadFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font!");
    }

    // Create pause text
    pauseText.setFont(font);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);

    // Center the pause text
    sf::FloatRect textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    pauseText.setPosition(centerX, startY - 40.f);  // Position above buttons
}

void PauseScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());

    // Handle button clicks
    if (resumeButton->isClicked()) {
        spdlog::info("Resuming game");
        game.getScreenManager().setState(GameState::Playing);
    }

    if (mainMenuButton->isClicked()) {
        spdlog::info("Returning to main menu");
        game.getScreenManager().setState(GameState::MainMenu);
    }
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause menu elements
    window.draw(pauseText);
    menuBuilder_.render(window);
}

} // namespace game
