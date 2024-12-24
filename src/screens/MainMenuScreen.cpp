#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../ui/GuiBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : Screen(game) {
    spdlog::info("Initializing MainMenuScreen");

    GuiBuilder(game.getGui())
        .addVerticalLayout("mainLayout")
        .addLabel("Museum Nightmare")
            .setAutoLayout(tgui::AutoLayout::Top)
        .addButton("Play", [this, &game]() {
            spdlog::info("Starting game");
            this->game.getScreenManager().setState(GameState::Playing);
        })
            .preserveAspectRatio(16.0f/9.0f)
            .setAutoLayout(tgui::AutoLayout::Fill)
            .endButton()
        .addButton("Quit", [this, &game]() {
            spdlog::info("Quitting game");
            this->game.getScreenManager().setState(GameState::Quit);
        })
            .preserveAspectRatio(16.0f/9.0f)
            .setAutoLayout(tgui::AutoLayout::Fill)
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
