#include "TestScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

TestScreen::TestScreen(Game& game) : Screen(game) {
    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Create widgets
    resolutionDropdown_ = menuBuilder_.addDropdown("Resolution",
        sf::Vector2f(centerX - 150.f, startY),
        sf::Vector2f(300.f, 40.f),
        getAvailableResolutions());

    applyButton_ = menuBuilder_.addButton("Apply",
        sf::Vector2f(centerX - 100.f, startY + 60.f),
        sf::Vector2f(200.f, 50.f));

    backButton_ = menuBuilder_.addButton("Back",
        sf::Vector2f(centerX - 100.f, startY + 120.f),
        sf::Vector2f(200.f, 50.f));

    // Center all widgets horizontally
    menuBuilder_.centerHorizontally(startY, 20.f);
}

TestScreen::~TestScreen() {
    // Widgets are managed by MenuBuilder
}

void TestScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());

    if (applyButton_->isClicked()) {
        spdlog::info("Applying resolution change");
        // TODO: Implement resolution change
    }
    else if (backButton_->isClicked()) {
        game.getScreenManager().setState(GameState::MainMenu);
    }
}

void TestScreen::render(sf::RenderWindow& window) {
    menuBuilder_.render(window);
}

} // namespace game
