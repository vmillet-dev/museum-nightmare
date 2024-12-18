#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game), gui(game.getWindow()) {
    spdlog::info("Initializing pause screen");

    // Create buttons
    buttons.push_back(std::make_unique<TGUIButtonWrapper>("Resume", sf::Vector2f(400, 250), sf::Vector2f(200, 50)));
    buttons.push_back(std::make_unique<TGUIButtonWrapper>("Main Menu", sf::Vector2f(400, 350), sf::Vector2f(200, 50)));

    // Add buttons to GUI
    for (auto& button : buttons) {
        gui.add(button->getWidget());
    }
    buttons[selectedButtonIndex]->setSelected(true);

    // Load font and setup pause text
    if (!font.loadFromFile("assets/arial.ttf")) {
        spdlog::error("Failed to load font!");
    }

    pauseText.setFont(font);
    pauseText.setString("Paused");
    pauseText.setCharacterSize(50);
    pauseText.setFillColor(sf::Color::White);

    // Center the pause text
    sf::FloatRect textBounds = pauseText.getLocalBounds();
    pauseText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    pauseText.setPosition(400, 150);
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

    // Update buttons and handle clicks
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->update(inputManager);
        if (buttons[i]->isClicked()) {
            if (i == 0) {
                spdlog::info("Resuming game");
                game.getScreenManager().setState(GameState::Playing);
            } else if (i == 1) {
                spdlog::info("Returning to main menu");
                game.getScreenManager().setState(GameState::MainMenu);
            }
        }
    }
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Draw pause text
    window.draw(pauseText);

    // Draw GUI elements
    gui.draw();
}

} // namespace game
