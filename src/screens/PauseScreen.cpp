#include "PauseScreen.hpp"
#include "MainMenuScreen.hpp"
#include "../core/Game.hpp"
#include "../ui/GuiBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

PauseScreen::PauseScreen(Game& game) : Screen(game) {
    spdlog::info("Initializing PauseScreen");

    GuiBuilder(game.getGui())
        .addVerticalLayout("pauseLayout")
        .addLabel("Paused")
            .setAutoLayout(tgui::AutoLayout::Top)
        .addButton("Resume", [this, &game]() {
            spdlog::info("Resuming game");
            this->game.getScreenManager().setState(GameState::Playing);
        })
            .preserveAspectRatio(16.0f/9.0f)
            .setAutoLayout(tgui::AutoLayout::Fill)
            .endButton()
        .addButton("Main Menu", [this, &game]() {
            spdlog::info("Returning to main menu");
            this->game.getScreenManager().setState(GameState::MainMenu);
        })
            .preserveAspectRatio(16.0f/9.0f)
            .setAutoLayout(tgui::AutoLayout::Fill)
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
