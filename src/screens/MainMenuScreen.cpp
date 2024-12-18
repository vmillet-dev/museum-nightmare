#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), gui(game.getWindow()), selectedButtonIndex(0) {
    auto playButton = tgui::Button::create("Play");
    playButton->setPosition({"300", "200"});
    playButton->setSize({"200", "50"});

    auto quitButton = tgui::Button::create("Quit");
    quitButton->setPosition({"300", "300"});
    quitButton->setSize({"200", "50"});

    // Style buttons
    for (auto& button : {playButton, quitButton}) {
        button->getRenderer()->setBackgroundColor(tgui::Color(100, 100, 100));
        button->getRenderer()->setBorderColor(tgui::Color::White);
        button->getRenderer()->setTextColor(tgui::Color::White);
        button->getRenderer()->setBackgroundColorHover(tgui::Color(150, 150, 150));
        button->getRenderer()->setBackgroundColorFocused(tgui::Color(200, 200, 200));
    }

    // Set up button callbacks
    playButton->onPress([this, &game]() {
        spdlog::info("Starting game");
        game.getScreenManager().setState(GameState::Playing);
    });

    quitButton->onPress([this, &game]() {
        spdlog::info("Quitting game");
        game.getScreenManager().setState(GameState::Quit);
    });

    buttons = {playButton, quitButton};
    for (auto& button : buttons) {
        gui.add(button);
    }
    buttons[selectedButtonIndex]->setFocused(true);
}

void MainMenuScreen::update(float deltaTime) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex + 1) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        buttons[selectedButtonIndex]->setFocused(false);
        selectedButtonIndex = (selectedButtonIndex - 1 + buttons.size()) % buttons.size();
        buttons[selectedButtonIndex]->setFocused(true);
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    gui.draw();
}

} // namespace game
