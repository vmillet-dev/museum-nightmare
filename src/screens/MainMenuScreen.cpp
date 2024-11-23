#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../core/GameState.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game) {
    buttons.push_back(std::make_unique<Button>("Play", sf::Vector2f(300, 200), sf::Vector2f(200, 50)));
    buttons.push_back(std::make_unique<Button>("Quit", sf::Vector2f(300, 300), sf::Vector2f(200, 50)));
}

void MainMenuScreen::handleInput(const sf::Event& event) {
    sf::Vector2f mousePosFloat;
    if (event.type == sf::Event::MouseMoved) {
        mousePosFloat = sf::Vector2f(static_cast<float>(event.mouseMove.x),
                                   static_cast<float>(event.mouseMove.y));
    }

    // Update button states
    for (auto& button : buttons) {
        button->handleInput(mousePosFloat, game.getInputManager());
    }

    // Check button states
    if (buttons[0]->isPressed()) {
        spdlog::info("Starting game");
        game.getStateManager().transitionTo(GameState::PLAYING);
    }
    if (buttons[1]->isPressed()) {
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
