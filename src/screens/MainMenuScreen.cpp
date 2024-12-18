#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), gui(game.getWindow()), selectedButtonIndex(0) {
    buttons.push_back(std::make_unique<TGUIButtonWrapper>("Play", sf::Vector2f(300, 200), sf::Vector2f(200, 50)));
    buttons.push_back(std::make_unique<TGUIButtonWrapper>("Quit", sf::Vector2f(300, 300), sf::Vector2f(200, 50)));

    for (auto& button : buttons) {
        gui.add(button->getWidget());
    }
    buttons[selectedButtonIndex]->setSelected(true);
}

void MainMenuScreen::update(float deltaTime) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        buttons[selectedButtonIndex]->setSelected(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setSelected(true);
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }

    // Update buttons and handle clicks
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i]->update(inputManager);
        if (buttons[i]->isClicked()) {
            if (i == 0) {
                spdlog::info("Starting game");
                game.getScreenManager().setState(GameState::Playing);
            } else if (i == 1) {
                spdlog::info("Quitting game");
                game.getScreenManager().setState(GameState::Quit);
            }
        }
    }
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    gui.draw();
}

} // namespace game
