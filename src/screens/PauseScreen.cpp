#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : game(game), m_menuBuilder(&game.getGui()) {
    spdlog::info("Initializing PauseScreen");

    // Configure menu
    m_menuBuilder
        .setLayout(game::LayoutType::Vertical)
        .setSpacing(20.f)
        .setPadding(50.f)
        .setTheme("assets/themes/dark.theme")
        .setResponsive(true);

    // Add title
    m_menuBuilder.addLabel("Paused")
        .setSize("400", "50")
        .build();

    // Add buttons
    m_menuBuilder.addButton("Resume", [this]{
        spdlog::info("Resuming game");
        game.getScreenManager().setState(GameState::Playing);
    })
    .setSize("200", "50")
    .build();

    m_menuBuilder.addButton("Main Menu", [this]{
        spdlog::info("Returning to main menu");
        game.getScreenManager().setState(GameState::MainMenu);
    })
    .setSize("200", "50")
    .build();

    m_menuBuilder.build();
    spdlog::info("PauseScreen initialized");
}

void PauseScreen::handleInput(Game& game) {
    auto& inputManager = game.getInputManager();
    m_menuBuilder.handleInput(inputManager);
}

void PauseScreen::handleEvent(const sf::Event& event) {
    m_menuBuilder.handleEvent(event);
}

void PauseScreen::update(float deltaTime) {
    // No update logic needed as input handling is done in handleInput
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
