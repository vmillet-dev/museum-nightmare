#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
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

    // Add buttons to vector and set initial selection
    buttons.push_back(std::move(resumeButton));
    buttons.push_back(std::move(mainMenuButton));
    buttons[selectedButtonIndex]->setSelected(true);
}

void PauseScreen::handleInput(const sf::Event& event) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
    }

    // Update all buttons with input manager
    for (auto& button : buttons) {
        button->update(inputManager);
    }

    // Handle button clicks
    if (buttons[0]->isClicked()) {  // Resume button
        spdlog::info("Resuming game");
        game.getScreenManager().setState(GameState::Playing);
    }

    if (buttons[1]->isClicked()) {  // Main Menu button
        spdlog::info("Returning to main menu");
        game.getScreenManager().setState(GameState::MainMenu);
    }
}

void PauseScreen::update(float deltaTime) {
    // No update needed for pause screen
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause menu elements
    window.draw(pauseText);
    for (auto& button : buttons) {
        button->render(window);
    }
}

} // namespace game
