#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game) {
    buttons.push_back(std::make_unique<Button>("Play", sf::Vector2f(300, 200), sf::Vector2f(200, 50)));
    buttons.push_back(std::make_unique<Button>("Quit", sf::Vector2f(300, 300), sf::Vector2f(200, 50)));
}

void MainMenuScreen::handleInput(const sf::Event& event) {
    auto& inputManager = game.getInputManager();

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
        shouldQuit = true;
    }
}

void MainMenuScreen::update(float deltaTime) {}

void MainMenuScreen::render(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button->render(window);
    }
}

} // namespace game
