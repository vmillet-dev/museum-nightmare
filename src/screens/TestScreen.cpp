#include "TestScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

TestScreen::TestScreen(Game& game) : Screen(game), menuBuilder_(game) {
    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Create resolution dropdown
    resolutionDropdown_ = menuBuilder_.addDropdown("Resolution",
        sf::Vector2f(centerX - 150.f, startY),
        sf::Vector2f(300.f, 40.f),
        getAvailableResolutions());

    // Create menu with automatic spacing
    menuBuilder_.setSpacing(100)
        .addButton("Apply", 300, startY + 60,
            [this, &game]() {
                spdlog::info("Applying resolution change");
                // TODO: Implement resolution change
            })
        .addButton("Back", 300, startY + 160,
            [this, &game]() {
                game.getScreenManager().setState(GameState::MainMenu);
            });

    // Store buttons and selected index
    buttons_ = menuBuilder_.build();
    selectedButtonIndex_ = menuBuilder_.getSelectedIndex();
}

void TestScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());
}

void TestScreen::render(sf::RenderWindow& window) {
    menuBuilder_.render(window);
}

} // namespace game
