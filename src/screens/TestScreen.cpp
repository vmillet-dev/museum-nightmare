#include "TestScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

TestScreen::TestScreen(Game& game) : Screen(game) {
    // Get window size for centering
    sf::Vector2u windowSize = game.getWindow().getSize();
    float centerX = windowSize.x / 2.f;
    float startY = windowSize.y * 0.3f;

    // Create resolution dropdown
    const float dropdownWidth = 300.f;
    const float dropdownHeight = 40.f;
    resolutionDropdown_ = new Dropdown("Resolution",
        sf::Vector2f(centerX - dropdownWidth / 2, startY),
        sf::Vector2f(dropdownWidth, dropdownHeight),
        getAvailableResolutions());

    // Create back button
    const float buttonWidth = 200.f;
    const float buttonHeight = 50.f;
    const float spacing = 20.f;
    backButton_ = menuBuilder_.addButton("Back",
        sf::Vector2f(centerX, startY + dropdownHeight + spacing * 2),
        sf::Vector2f(buttonWidth, buttonHeight));
}

TestScreen::~TestScreen() {
    delete resolutionDropdown_;
    // Note: backButton_ is managed by menuBuilder_
}

void TestScreen::update(float deltaTime) {
    menuBuilder_.update(game.getInputManager());
    resolutionDropdown_->update(game.getInputManager());

    if (backButton_->isClicked()) {
        game.getScreenManager().setState(GameState::MainMenu);
    }
}

void TestScreen::render(sf::RenderWindow& window) {
    menuBuilder_.render(window);
    resolutionDropdown_->render(window);
}

} // namespace game
