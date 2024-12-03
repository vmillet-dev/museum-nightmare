#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), selectedButtonIndex(0) {
    buttons.push_back(std::make_unique<Button>("Play", sf::Vector2f(300, 200), sf::Vector2f(200, 50)));
    buttons.push_back(std::make_unique<Button>("Quit", sf::Vector2f(300, 300), sf::Vector2f(200, 50)));
    // Set initial button selection
    buttons[selectedButtonIndex]->setSelected(true);
}

void MainMenuScreen::update(float deltaTime) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }

    // Update all buttons with input manager
    for (auto& button : buttons) {
        button->update(inputManager);
    }

    // Play button clicked
    if (buttons[0]->isClicked()) {
        spdlog::info("Starting game");
        game.getScreenManager().setState(GameState::Playing);
    }

    // Quit button clicked
    if (buttons[1]->isClicked()) {
        spdlog::info("Quitting game");
        game.getScreenManager().setState(GameState::Quit);
    }
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button->render(window);
    }
}

} // namespace game
