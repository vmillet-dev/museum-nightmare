#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : Screen(game), menuBuilder_(game) {
    spdlog::info("Initializing pause screen");

    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Create menu buttons using MenuBuilder with method chaining
    menuBuilder_.setSpacing(100)
        .addButton("Resume", 300, startY,
            [this, &game]() {
                spdlog::info("Resuming game");
                game.getScreenManager().setState(GameState::Playing);
            })
        .addButton("Main Menu", 300, startY + 100,
            [this, &game]() {
                spdlog::info("Returning to main menu");
                game.getScreenManager().setState(GameState::MainMenu);
            });

    // Store buttons and selected index
    buttons_ = menuBuilder_.build();
    selectedButtonIndex_ = menuBuilder_.getSelectedIndex();

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
