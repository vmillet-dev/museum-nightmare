#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), m_menuBuilder(&game.getGui()) {
    spdlog::info("Initializing MainMenuScreen");

    // Configure menu
    m_menuBuilder
        .setLayout(game::LayoutType::Vertical)
        .setSpacing(20.f)
        .setPadding(50.f)
        .setTheme("assets/themes/dark.theme")
        .setResponsive(true);

    // Add title
    m_menuBuilder.addLabel("Museum Nightmare")
        .setSize("400", "50")
        .build();

    // Add buttons
    m_menuBuilder.addButton("Play", [this, &game]{
        spdlog::info("Starting game");
        game.getScreenManager().setState(GameState::Playing);
    })
    .setSize("200", "50")
    .build();

    m_menuBuilder.addButton("Quit", [this, &game]{
        spdlog::info("Quitting game");
        game.getScreenManager().setState(GameState::Quit);
    })
    .setSize("200", "50")
    .build();

    m_menuBuilder.build();
    spdlog::info("MainMenuScreen initialized");
}

void MainMenuScreen::handleInput(Game& game) {
    auto& inputManager = game.getInputManager();
    m_menuBuilder.handleInput(inputManager);
}

void MainMenuScreen::handleEvent(const sf::Event& event) {
    m_menuBuilder.handleEvent(event);
}

void MainMenuScreen::update(float deltaTime) {
    // No update logic needed as input handling is done in handleInput
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    spdlog::debug("MainMenu rendering - Window size: {}x{}",
        window.getSize().x, window.getSize().y);
}

} // namespace game
