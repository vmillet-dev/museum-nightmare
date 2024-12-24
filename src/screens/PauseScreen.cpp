#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game), m_guiBuilder(game.getGui()) {
    spdlog::info("Initializing PauseScreen");

    m_guiBuilder
        .addVerticalLayout("mainLayout")
        .addLabel("Paused")
        .addButton("Resume", [this]() {
            spdlog::info("Resuming game");
            game.getScreenManager().setState(GameState::Playing);
        })
        .setTextSize(20)
        .endButton()
        .addButton("Main Menu", [this]() {
            spdlog::info("Returning to main menu");
            game.getScreenManager().setState(GameState::MainMenu);
        })
        .setTextSize(20)
        .endButton()
        .build();

    spdlog::info("PauseScreen initialized");
}

void PauseScreen::handleInput(Game& game) {
    // Input handling is now managed by TGUI
}

void PauseScreen::handleEvent(const sf::Event& event) {
    game.getGui().handleEvent(event);
}

void PauseScreen::update(float deltaTime) {
    // No update logic needed as input handling is done by TGUI
}

void PauseScreen::render(sf::RenderWindow& window) {
    // Draw semi-transparent background
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    spdlog::debug("PauseScreen rendering - Window size: {}x{}",
        window.getSize().x, window.getSize().y);
}

} // namespace game
