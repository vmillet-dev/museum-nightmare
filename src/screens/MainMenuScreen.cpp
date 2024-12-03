#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include "../ui/MenuBuilder.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), selectedButtonIndex(0) {
    MenuBuilder builder(game);

    // Add Play button with automatic vertical spacing
    builder.setSpacing(100)
           .addButton("Play", 300, 200,
                [this, &game]() {
                    spdlog::info("Starting game");
                    game.getScreenManager().setState(GameState::Playing);
                })
           .addButton("Quit", 300, 300,
                [this, &game]() {
                    spdlog::info("Quitting game");
                    game.getScreenManager().setState(GameState::Quit);
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

    // Update all buttons
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
