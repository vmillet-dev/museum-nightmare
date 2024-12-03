#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include "../ui/MenuBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game), selectedButtonIndex(0) {
    spdlog::info("Initializing pause screen");

    // Load font for pause text
    if (!font.loadFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font!");
    }

    // Setup pause text
    pauseText.setFont(font);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);

    // Center the pause text
    sf::FloatRect textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    pauseText.setPosition(400, 150);

    // Create menu using MenuBuilder
    MenuBuilder builder(game);
    builder.setSpacing(100)
           .addButton("Resume", 400, 250,
                [this, &game]() {
                    spdlog::info("Resuming game");
                    game.getScreenManager().setState(GameState::Playing);
                })
           .addButton("Main Menu", 400, 350,
                [this, &game]() {
                    spdlog::info("Returning to main menu");
                    game.getScreenManager().setState(GameState::MainMenu);
                });

    buttons = builder.build();
    selectedButtonIndex = builder.getSelectedIndex();
}

void PauseScreen::update(float deltaTime) {
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

    // Update all buttons
    for (auto& button : buttons) {
        button->update(inputManager);
    }
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
