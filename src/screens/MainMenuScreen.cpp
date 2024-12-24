#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), m_guiBuilder(game.getGui()) {
    spdlog::info("Initializing MainMenuScreen");

    m_guiBuilder
        .addVerticalLayout("mainLayout")
        .addLabel("Museum Nightmare")
        .addButton("Play", [this, &game]() {
            spdlog::info("Starting game");
            game.getScreenManager().setState(GameState::Playing);
        })
        .setTextSize(20)
        .endButton()
        .addButton("Quit", [this, &game]() {
            spdlog::info("Quitting game");
            game.getScreenManager().setState(GameState::Quit);
        })
        .setTextSize(20)
        .endButton()
        .build();

    spdlog::info("MainMenuScreen initialized");
}

void MainMenuScreen::handleInput(Game& game) {
    // Input handling is now managed by TGUI
}

void MainMenuScreen::handleEvent(const sf::Event& event) {
    game.getGui().handleEvent(event);
}

void MainMenuScreen::update(float deltaTime) {
    // No update logic needed as input handling is done by TGUI
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    spdlog::debug("MainMenu rendering - Window size: {}x{}",
        window.getSize().x, window.getSize().y);
}

} // namespace game
