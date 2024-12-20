#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "../core/Game.hpp"
#include <spdlog/spdlog.h>

namespace game {

MainMenuScreen::MainMenuScreen(Game& game) : game(game), selectedButtonIndex(0) {
    gui.setTarget(game.getWindow());

    m_menuBuilder = std::make_shared<MenuBuilder>(&gui);
    m_container = m_menuBuilder
        ->addButton("playButton", "Play")
        ->addButton("quitButton", "Quit")
        ->setVerticalLayout()
        ->setSpacing(20.f)
        ->setPadding(50.f)
        ->onClick("playButton", [this, &game]{
            spdlog::info("Starting game");
            game.getScreenManager().setState(GameState::Playing);
        })
        ->onClick("quitButton", [this, &game]{
            spdlog::info("Quitting game");
            game.getScreenManager().setState(GameState::Quit);
        })
        ->setTheme("assets/themes/dark.theme")
        ->setResponsive(true)
        ->build();

    gui.add(m_container);

    // Initialize first button as selected
    if (auto button = m_container->get<tgui::Button>("playButton")) {
        button->setFocused(true);
    }
}

void MainMenuScreen::update(float deltaTime) {
    auto& inputManager = game.getInputManager();

    // Handle button navigation
    if (inputManager.isActionJustPressed(Action::MoveDown)) {
        if (auto currentButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "playButton" : "quitButton")) {
            currentButton->setFocused(false);
        }
        selectedButtonIndex = (selectedButtonIndex + 1) % 2;
        if (auto nextButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "playButton" : "quitButton")) {
            nextButton->setFocused(true);
        }
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
    if (inputManager.isActionJustPressed(Action::MoveUp)) {
        if (auto currentButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "playButton" : "quitButton")) {
            currentButton->setFocused(false);
        }
        selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2;
        if (auto nextButton = m_container->get<tgui::Button>(selectedButtonIndex == 0 ? "playButton" : "quitButton")) {
            nextButton->setFocused(true);
        }
        spdlog::debug("Main menu: Selected button {}", selectedButtonIndex);
    }
}

void MainMenuScreen::render(sf::RenderWindow& window) {
    gui.draw();
}

} // namespace game
