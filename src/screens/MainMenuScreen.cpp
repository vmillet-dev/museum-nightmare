#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../ui/MenuBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), selectedButtonIndex(0) {
    MenuBuilder builder(game);

    // Add buttons with automatic vertical spacing
    builder.setSpacing(100)
           .addButton("Play", 300, 200,
                [this, &game]() {
                    spdlog::debug("Play button clicked - Button state validation");
                    spdlog::info("Play button clicked - Attempting to transition to GameState::Playing");
                    game.getScreenManager().setState(GameState::Playing);
                    spdlog::debug("State transition completed");
                })
           .addButton("Settings", 300, 300,
                [this, &game]() {
                    spdlog::info("Settings button clicked - Transitioning to Settings screen");
                    game.getScreenManager().setState(GameState::Settings);
                })
           .addButton("Quit", 300, 400,
                [this, &game]() {
                    spdlog::info("Quit button clicked - Attempting to transition to GameState::Quit");
                    game.getScreenManager().setState(GameState::Quit);
                    spdlog::info("Screen transition completed");
                });

    buttons = builder.build();
    selectedButtonIndex = builder.getSelectedIndex();
}

void MainMenuScreen::update(float deltaTime) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
    }

    // Update all buttons - callbacks will be triggered inside Button::update
    for (auto& button : buttons) {
        button->update(inputManager);
    }
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button->render(window);
    }
}

} // namespace game
