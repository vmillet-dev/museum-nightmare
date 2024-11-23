#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include "ScreenManager.hpp"
#include "../core/GameState.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game) {
    spdlog::info("Initializing pause screen");

    // Create resume button
    resumeButton = std::make_unique<Button>(
        "Resume",
        sf::Vector2f(400, 250),
        sf::Vector2f(200, 50)
    );

    // Create main menu button
    mainMenuButton = std::make_unique<Button>(
        "Main Menu",
        sf::Vector2f(400, 350),
        sf::Vector2f(200, 50)
    );

    // Load font
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
    pauseText.setPosition(400, 150);
}

void PauseScreen::handleInput(const sf::Event& event) {
    sf::Vector2f mousePosFloat;
    if (event.type == sf::Event::MouseMoved) {
        mousePosFloat = sf::Vector2f(static_cast<float>(event.mouseMove.x),
                                   static_cast<float>(event.mouseMove.y));
    }

    // Update button states
    resumeButton->handleInput(mousePosFloat, game.getInputManager());
    mainMenuButton->handleInput(mousePosFloat, game.getInputManager());

    // Check button states
    if (resumeButton->isPressed()) {
        spdlog::info("Resuming game");
        game.getStateManager().transitionTo(GameState::PLAYING);
    }
    if (mainMenuButton->isPressed()) {
        spdlog::info("Returning to main menu");
        game.getStateManager().transitionTo(GameState::MAIN_MENU);
    }
}

void PauseScreen::update(float deltaTime) {
    // No update needed for buttons in our implementation
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause menu elements
    window.draw(pauseText);
    resumeButton->render(window);
    mainMenuButton->render(window);
}

} // namespace game
